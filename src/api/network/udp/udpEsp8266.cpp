#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "string.h"
#include "udp.hpp"
#include <vector>

#define MAX_UDP_MESSAGES 10

/**
 * @brief Simple message buffer structure to avoid alignment issues
 */
typedef struct
{
    char client_ip[16];
    char mac_addr[18];
    char data[1024];
    int data_len;
    bool valid;
} __attribute__((packed)) udp_message_buffer_t;

/**
 * @brief Circular buffer for UDP messages
 */
static udp_message_buffer_t udp_msg_buffer[MAX_UDP_MESSAGES];
static int udp_buffer_write_idx = 0;
static int udp_buffer_read_idx = 0;
static int udp_buffer_count = 0;

static int udp_socket = -1;
static bool udp_task_running = false;
static bool udp_socket_initialized = false;

/**
 * @brief Mutex to protect access to the incomplete messages vector
 */
static SemaphoreHandle_t udp_incomplet_msgs_mutex = nullptr;

static void udp_recv_task(void *pvParameters)
{
    char buffer[1024];
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (udp_task_running)
    {
        // Wait for WiFi connection before initializing socket
        if (!udp_socket_initialized)
        {
            wifi_ap_record_t ap_info;
            if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK)
            {
                printf("WiFi connected, waiting for IP assignment...\n");
                vTaskDelay(
                    2000
                    / portTICK_PERIOD_MS); // Wait 2 seconds for IP assignment

                // Verify we have a valid IP address
                tcpip_adapter_ip_info_t ip_info;
                if (tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info)
                        == ESP_OK
                    && ip_info.ip.addr != 0)
                {
                    char ip_str[16];
                    inet_ntoa_r(ip_info.ip, ip_str, 16);
                    printf("IP address assigned: %s\n", ip_str);

                    // WiFi is connected and IP assigned, initialize UDP socket
                    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
                    if (udp_socket >= 0)
                    {
                        struct sockaddr_in server_addr;
                        memset(&server_addr, 0, sizeof(server_addr));
                        server_addr.sin_family = AF_INET;
                        server_addr.sin_addr.s_addr = INADDR_ANY;
                        server_addr.sin_port = htons(UDP_DEFAULT_PORT);

                        if (bind(udp_socket,
                                 (struct sockaddr *)&server_addr,
                                 sizeof(server_addr))
                            >= 0)
                        {
                            udp_socket_initialized = true;
                            printf("UDP socket created and bound after WiFi "
                                   "connection\n");
                        }
                        else
                        {
                            close(udp_socket);
                            udp_socket = -1;
                        }
                    }
                }
                else
                {
                    printf("No IP address assigned yet, retrying...\n");
                }
            }

            if (!udp_socket_initialized)
            {
                vTaskDelay(1000
                           / portTICK_PERIOD_MS); // Wait 1 second before retry
                continue;
            }
        }

        int len = recvfrom(udp_socket,
                           buffer,
                           sizeof(buffer) - 1,
                           0,
                           (struct sockaddr *)&client_addr,
                           &client_addr_len);

        if (len > 0)
        {
            buffer[len] = '\0';
            char client_ip[16];
            inet_ntoa_r(client_addr.sin_addr, client_ip, sizeof(client_ip));

            // Dummy MAC address for ESP8266
            static const char dummy_mac[] = "00:00:00:00:00:00";

            // Store in circular buffer instead of vector
            if (xSemaphoreTake(udp_incomplet_msgs_mutex, portMAX_DELAY)
                == pdTRUE)
            {
                if (udp_buffer_count < MAX_UDP_MESSAGES)
                {
                    udp_message_buffer_t *msg
                        = &udp_msg_buffer[udp_buffer_write_idx];
                    strncpy(
                        msg->client_ip, client_ip, sizeof(msg->client_ip) - 1);
                    msg->client_ip[sizeof(msg->client_ip) - 1] = '\0';
                    strcpy(msg->mac_addr, dummy_mac);
                    memcpy(msg->data, buffer, len);
                    msg->data_len = len;
                    msg->valid = true;

                    udp_buffer_write_idx
                        = (udp_buffer_write_idx + 1) % MAX_UDP_MESSAGES;
                    udp_buffer_count++;
                }
                xSemaphoreGive(udp_incomplet_msgs_mutex);
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void udp_init()
{
    // Create mutex for protecting incomplete messages vector
    udp_incomplet_msgs_mutex = xSemaphoreCreateMutex();

    // Start receive task that will wait for WiFi and then initialize
    udp_task_running = true;
    xTaskCreate(udp_recv_task, "udp_recv", 2048, NULL, 5, NULL);

    printf("UDP task started, waiting for WiFi connection\n");
}

void udp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    if (udp_socket < 0 || !udp_socket_initialized)
        return;

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    inet_aton(ip, &dest_addr.sin_addr);

    sendto(udp_socket,
           data,
           len,
           0,
           (struct sockaddr *)&dest_addr,
           sizeof(dest_addr));
}

void udp_send_broadcast(const char *data, int16_t len, int16_t port)
{
    if (udp_socket < 0 || !udp_socket_initialized)
        return;

    // Enable broadcast
    int broadcast = 1;
    setsockopt(
        udp_socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    struct sockaddr_in broadcast_addr;
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(port);
    broadcast_addr.sin_addr.s_addr = INADDR_BROADCAST;

    sendto(udp_socket,
           data,
           len,
           0,
           (struct sockaddr *)&broadcast_addr,
           sizeof(broadcast_addr));
}

std::vector<IncompletMsg> udp_recv()
{
    std::vector<IncompletMsg> incompletMsgs;

    if (xSemaphoreTake(udp_incomplet_msgs_mutex, portMAX_DELAY) == pdTRUE)
    {
        // Convert buffer messages to IncompletMsg objects
        while (udp_buffer_count > 0)
        {
            udp_message_buffer_t *msg = &udp_msg_buffer[udp_buffer_read_idx];
            if (msg->valid)
            {
                // Create objects carefully to avoid alignment issues
                Ipv4 l_ip(msg->client_ip);
                MacAddr l_mac(msg->mac_addr);
                Client l_client(l_ip, l_mac);

                // Copy data_len to avoid packed field reference issue
                int data_len_copy = msg->data_len;
                incompletMsgs.emplace_back(l_client, msg->data, data_len_copy);

                msg->valid = false;
            }

            udp_buffer_read_idx = (udp_buffer_read_idx + 1) % MAX_UDP_MESSAGES;
            udp_buffer_count--;
        }

        xSemaphoreGive(udp_incomplet_msgs_mutex);
    }

    return incompletMsgs;
}

void udp_get_host_ip(char *ip)
{
    tcpip_adapter_ip_info_t ip_info;
    if (tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info) == ESP_OK)
    {
        inet_ntoa_r(ip_info.ip, ip, 16);
    }
    else
    {
        strcpy(ip, "0.0.0.0");
    }
}

void udp_get_host_mac(char *mac)
{
    uint8_t mac_addr[6];
    if (esp_wifi_get_mac(WIFI_IF_STA, mac_addr) == ESP_OK)
    {
        sprintf(mac,
                "%02X:%02X:%02X:%02X:%02X:%02X",
                mac_addr[0],
                mac_addr[1],
                mac_addr[2],
                mac_addr[3],
                mac_addr[4],
                mac_addr[5]);
    }
    else
    {
        strcpy(mac, "00:00:00:00:00:00");
    }
}
