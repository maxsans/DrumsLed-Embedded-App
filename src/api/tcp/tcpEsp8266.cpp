#include "tcp.hpp"
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_wifi.h"
#include "esp_system.h"

#define TCP_RECV_BUF_SIZE 2048

typedef struct
{
    char data[TCP_RECV_BUF_SIZE];
    int head;
    int tail;
    SemaphoreHandle_t mutex;
} tcp_ring_buffer_t;

static tcp_ring_buffer_t tcp_recv_buf;
static int listen_sock = -1;
static int client_sock = -1;
static TaskHandle_t tcp_recv_task_handle = NULL;

static void tcp_recv_task(void *pvParameters)
{
    char rx_buffer[256];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (1)
    {
        // Accept a new client if not connected
        if (client_sock < 0)
        {
            client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &addr_len);
            if (client_sock < 0)
            {
                vTaskDelay(pdMS_TO_TICKS(100));
                continue;
            }
        }

        int len = recv(client_sock, rx_buffer, sizeof(rx_buffer), 0);
        if (len > 0)
        {
            xSemaphoreTake(tcp_recv_buf.mutex, portMAX_DELAY);
            for (int i = 0; i < len; ++i)
            {
                int next_head = (tcp_recv_buf.head + 1) % TCP_RECV_BUF_SIZE;
                if (next_head != tcp_recv_buf.tail)
                { // buffer not full
                    tcp_recv_buf.data[tcp_recv_buf.head] = rx_buffer[i];
                    tcp_recv_buf.head = next_head;
                }
            }
            xSemaphoreGive(tcp_recv_buf.mutex);
        }
        else if (len == 0)
        {
            // Connection closed by client
            close(client_sock);
            client_sock = -1;
        }
        else
        {
            // Error
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}

void tcp_init()
{
    memset(&tcp_recv_buf, 0, sizeof(tcp_recv_buf));
    tcp_recv_buf.mutex = xSemaphoreCreateMutex();

    struct sockaddr_in server_addr;
    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (listen_sock >= 0)
    {
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(TCP_DEFAULT_PORT);

        int yes = 1;
        setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
        bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
        listen(listen_sock, 1); // Only allow 1 client for simplicity
        xTaskCreate(tcp_recv_task, "tcp_recv_task", 4096, NULL, 5, &tcp_recv_task_handle);
    }
    else
    {
        // Handle socket creation error
        printf("Failed to create socket\n");
    }
}

void tcp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    // Check if tcp_init() has been called
    if (tcp_recv_buf.mutex == NULL)
    {
        // tcp_init() has not been called yet
        return;
    }

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0)
    {
        printf("tcp_send: failed to create socket\n");
        return;
    }

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &dest_addr.sin_addr.s_addr) <= 0)
    {
        printf("tcp_send: invalid IP address: %s\n", ip);
        close(sock);
        return;
    }

    if (connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
    {
        printf("tcp_send: failed to connect to %s:%d\n", ip, port);
        close(sock);
        return;
    }

    int32_t total_sent = 0;
    while (total_sent < len)
    {
        int sent = send(sock, data + total_sent, len - total_sent, 0);
        if (sent <= 0)
        {
            printf("tcp_send: error sending data: %d\n", errno);
            break;
        }
        total_sent += sent;
    }

    close(sock);
}

uint32_t tcp_recv(char *data, int16_t len, char *ip, int16_t *port, char *mac)
{
    // Check if tcp_init() has been called
    if (tcp_recv_buf.mutex == NULL)
    {
        // tcp_init() has not been called yet
        return 0; // Socket not available
    }

    uint32_t read_len = 0;

    xSemaphoreTake(tcp_recv_buf.mutex, portMAX_DELAY);
    while (tcp_recv_buf.tail != tcp_recv_buf.head && read_len < len)
    {
        data[read_len++] = tcp_recv_buf.data[tcp_recv_buf.tail];
        tcp_recv_buf.tail = (tcp_recv_buf.tail + 1) % TCP_RECV_BUF_SIZE;
    }
    xSemaphoreGive(tcp_recv_buf.mutex);

    // Fill ip and port if possible
    if (client_sock >= 0)
    {
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        if (getpeername(client_sock, (struct sockaddr *)&addr, &addrlen) == 0)
        {
            if (ip)
                strcpy(ip, inet_ntoa(addr.sin_addr));
            if (port)
                *port = ntohs(addr.sin_port);
        }
        else
        {
            if (ip)
                ip[0] = '\0';
            if (port)
                *port = 0;
        }
    }
    else
    {
        if (ip)
            ip[0] = '\0';
        if (port)
            *port = 0;
    }

    // The MAC address is not needed on esp8266 module
    if (mac)
    {
        // So set it to a default value
        strcpy(mac, "00:00:00:00:00:00");
    }

    return read_len;
}

void tcp_get_host_ip(char *ip)
{
    tcpip_adapter_ip_info_t ip_info;
    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
    sprintf(ip, IPSTR, IP2STR(&ip_info.ip));
}

void tcp_get_host_mac(char *mac)
{
    uint8_t mac_addr[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac_addr);
    sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac_addr[0], mac_addr[1], mac_addr[2],
            mac_addr[3], mac_addr[4], mac_addr[5]);
}
