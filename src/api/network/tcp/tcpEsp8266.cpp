#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "tcp.hpp"
#include "tcpip_adapter.h"
#include <vector>

#define TAG "TCP_ESP8266"
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024
#define WIFI_CONNECTED_BIT BIT0
#define MAX_TCP_MESSAGES 10

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
} __attribute__((packed)) tcp_message_buffer_t;

/**
 * @brief Circular buffer for TCP messages
 */
static tcp_message_buffer_t tcp_msg_buffer[MAX_TCP_MESSAGES];
static int tcp_buffer_write_idx = 0;
static int tcp_buffer_read_idx = 0;
static int tcp_buffer_count = 0;

static EventGroupHandle_t wifi_event_group;
static int server_socket = -1;
static TaskHandle_t tcp_server_task_handle = nullptr;

/**
 * @brief Mutex to protect access to the incomplete messages buffer
 */
static SemaphoreHandle_t tcp_incomplet_msgs_mutex = nullptr;

static void tcp_server_task(void *pvParameters)
{
    // Wait for WiFi connection
    ESP_LOGI(TAG, "Waiting for WiFi connection...");
    xEventGroupWaitBits(
        wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "WiFi connected, starting TCP server");

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        ESP_LOGE(TAG, "Failed to create socket");
        vTaskDelete(NULL);
        return;
    }

    // Set socket options
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind socket
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(TCP_DEFAULT_PORT);

    if (bind(
            server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))
        < 0)
    {
        ESP_LOGE(TAG, "Failed to bind socket");
        close(server_socket);
        vTaskDelete(NULL);
        return;
    }

    // Listen for connections
    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        ESP_LOGE(TAG, "Failed to listen on socket");
        close(server_socket);
        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI(TAG, "TCP server listening on port %d", TCP_DEFAULT_PORT);

    // Accept and handle connections
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(
            server_socket, (struct sockaddr *)&client_addr, &client_len);

        if (client_socket < 0)
        {
            ESP_LOGE(TAG, "Failed to accept connection");
            continue;
        }

        char client_ip[16];
        inet_ntoa_r(client_addr.sin_addr, client_ip, sizeof(client_ip));
        ESP_LOGI(TAG,
                 "Client connected from %s:%d",
                 client_ip,
                 ntohs(client_addr.sin_port));

        // Handle client data
        char buffer[BUFFER_SIZE];
        while (1)
        {
            int bytes_received
                = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0)
            {
                break; // Client disconnected or error
            }

            buffer[bytes_received] = '\0';

            // Dummy MAC address for ESP8266
            static const char dummy_mac[] = "00:00:00:00:00:00";

            // Store in circular buffer instead of vector
            if (xSemaphoreTake(tcp_incomplet_msgs_mutex, portMAX_DELAY)
                == pdTRUE)
            {
                if (tcp_buffer_count < MAX_TCP_MESSAGES)
                {
                    tcp_message_buffer_t *msg
                        = &tcp_msg_buffer[tcp_buffer_write_idx];
                    strncpy(
                        msg->client_ip, client_ip, sizeof(msg->client_ip) - 1);
                    msg->client_ip[sizeof(msg->client_ip) - 1] = '\0';
                    strcpy(msg->mac_addr, dummy_mac);
                    memcpy(msg->data, buffer, bytes_received);
                    msg->data_len = bytes_received;
                    msg->valid = true;

                    tcp_buffer_write_idx
                        = (tcp_buffer_write_idx + 1) % MAX_TCP_MESSAGES;
                    tcp_buffer_count++;
                }
                xSemaphoreGive(tcp_incomplet_msgs_mutex);
            }
        }

        close(client_socket);
        ESP_LOGI(TAG, "Client disconnected");
    }

    vTaskDelete(NULL);
}

static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
        case SYSTEM_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG,
                     "WiFi connected, IP: " IPSTR,
                     IP2STR(&event->event_info.got_ip.ip_info.ip));
            xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "WiFi disconnected");
            xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
            break;
        default:
            break;
    }
    return ESP_OK;
}

void tcp_init()
{
    // Create mutex for protecting incomplete messages vector
    tcp_incomplet_msgs_mutex = xSemaphoreCreateMutex();

    // Initialize WiFi event group
    wifi_event_group = xEventGroupCreate();

    // Set event handler
    ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));

    // Create TCP server task
    xTaskCreate(
        tcp_server_task, "tcp_server", 4096, NULL, 5, &tcp_server_task_handle);

    ESP_LOGI(TAG, "TCP module initialized");
}

std::vector<IncompletMsg> tcp_recv()
{
    std::vector<IncompletMsg> incompletMsgs;

    if (xSemaphoreTake(tcp_incomplet_msgs_mutex, portMAX_DELAY) == pdTRUE)
    {
        // Convert buffer messages to IncompletMsg objects
        while (tcp_buffer_count > 0)
        {
            tcp_message_buffer_t *msg = &tcp_msg_buffer[tcp_buffer_read_idx];
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

            tcp_buffer_read_idx = (tcp_buffer_read_idx + 1) % MAX_TCP_MESSAGES;
            tcp_buffer_count--;
        }

        xSemaphoreGive(tcp_incomplet_msgs_mutex);
    }

    return incompletMsgs;
}

void tcp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    if (!data || len <= 0)
    {
        return;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        ESP_LOGE(TAG, "Failed to create client socket");
        return;
    }

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &dest_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
    {
        ESP_LOGE(TAG, "Failed to connect to %s:%d", ip, port);
        close(sock);
        return;
    }

    send(sock, data, len, 0);
    close(sock);
}

void tcp_get_host_ip(char *ip)
{
    if (!ip)
        return;

    tcpip_adapter_ip_info_t ip_info;
    if (tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info) == ESP_OK)
    {
        sprintf(ip, IPSTR, IP2STR(&ip_info.ip));
    }
    else
    {
        strcpy(ip, "0.0.0.0");
    }
}

void tcp_get_host_mac(char *mac)
{
    if (!mac)
        return;

    uint8_t mac_addr[6];
    if (esp_wifi_get_mac(WIFI_IF_STA, mac_addr) == ESP_OK)
    {
        sprintf(mac,
                "%02x:%02x:%02x:%02x:%02x:%02x",
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
