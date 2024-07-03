#include "module.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

module::module()
{
    // Constructor
    m_ip = NULL;
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
