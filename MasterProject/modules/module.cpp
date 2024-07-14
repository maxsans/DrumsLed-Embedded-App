#include "module.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

#define MODULE_TIMEOUT 3000 // ms

module::module(char* ip)
{
    // Constructor
    m_ip = (char*)malloc(strlen(ip) + 1);
    strcpy(m_ip, ip);
    m_connected = true;
    m_lastSyncTime = GetTickCount64();
}

void module::process()
{
    // Process the module
    // Check if the module is still connected
    checkTime();
}

void module::sync()
{
    // Sync the module
    if (!m_connected)
    {
        printf("Module reconnected, ip: %s\n", m_ip);
        m_connected = true;
    }
    m_lastSyncTime = GetTickCount64();
}

void module::checkTime()
{
    // Check if the module is still connected
    if (GetTickCount64() - m_lastSyncTime > MODULE_TIMEOUT)
    {
        if (m_connected)
        {
            printf("Module disconnected, ip: %s\n", m_ip);
            m_connected = false;
        }
    }
}

bool module::isConnected()
{
    // Check if the module is connected
    return m_connected;
}

void module::setIp(char* ip)
{
    // Set the ip
    if (m_ip != NULL)
    {
        free(m_ip);
    }
    m_ip = (char*)malloc(strlen(ip) + 1);
    strcpy(m_ip, ip);
}

char* module::getIp()
{
    // Get the ip
    return m_ip;
}
