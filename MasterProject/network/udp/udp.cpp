#include "udp.h"

#include "udpParser.h"

#include <iphlpapi.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define PORT 9999
#define ARP_TABLE_SIZE 65536

udp::udp()
{

}

void  udp::init()
{
    struct sockaddr_in server;
    WSADATA wsa;

    slen = sizeof(si_other) ;

    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );

    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done");

    // get the IP and mac address of the machine and store it in the host
    char ac[80];
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR)
    {
        printf("Error %d when getting local host name.\n", WSAGetLastError());
    }
    else
    {
        struct hostent *phe = gethostbyname(ac);
        for (uint16_t i = 0; phe->h_addr_list[i] != 0; ++i)
        {
            if (i > 0)
            {
                // multiple IP addresses are not supported
                break;
            }
            // Get the IP address of the machine
            IPv4 l_localIp(inet_ntoa(*(struct in_addr *)phe->h_addr_list[i]));
            // Get the MAC address of the machine
            IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information for up to 16 NICs
            DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer
            DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
            if (dwStatus != ERROR_SUCCESS)
            {
                printf("GetAdaptersInfo failed with error: %d\n", dwStatus);
            }
            else
            {
                // Get the MAC address of the machine
                uint8_t *l_mac = (uint8_t *)AdapterInfo->Address;
                MacAddr l_localMac(*l_mac,
                                   *(l_mac + 1),
                                   *(l_mac + 2),
                                   *(l_mac + 3),
                                   *(l_mac + 4),
                                   *(l_mac + 5));
                m_host = Client(l_localIp, l_localMac);
                printf("Local IP : %s, Local MAC : %s\n", l_localIp.getIpString().c_str(), l_localMac.getMacString().c_str());
            }
        }
    }
}

void udp::send(Client client, char *msg, uint32_t msgSize)
{
    //setup address structure
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(client.getIP().getIpString().c_str());

    //send the message
    if (sendto(s, msg, msgSize , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
    {
        printf("sendto() failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

void udp::recv()
{
    DWORD read_timeout = 1;  // it is milliseconds!
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char const*)&read_timeout, sizeof read_timeout) != 0)
    {
        printf("setsockopt failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    //try to receive some data, this is a blocking call with 1ms of timeout
    if ((recv_len = recvfrom(s, m_udpPacket.getData(), BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) != SOCKET_ERROR)
    {
        // Get the IP address of the packet
        IPv4 l_packetIp(inet_ntoa(si_other.sin_addr));
        // Get the mac address of the packet by searching in the ARP table
        // Get the ARP table
        PMIB_IPNETTABLE m_ipNetTable = (PMIB_IPNETTABLE)malloc(sizeof(MIB_IPNETTABLE) + ARP_TABLE_SIZE * sizeof(MIB_IPNETROW));
        DWORD l_size = ARP_TABLE_SIZE;
        uint32_t l_IpNetTableError = GetIpNetTable(m_ipNetTable, &l_size, FALSE);
        // If the ARP table is too small, reallocate it
        if (l_IpNetTableError == ERROR_INSUFFICIENT_BUFFER)
        {
            free(m_ipNetTable);
            m_ipNetTable = (PMIB_IPNETTABLE)malloc(sizeof(MIB_IPNETTABLE) + (l_size + ARP_TABLE_SIZE) * sizeof(MIB_IPNETROW));
            l_IpNetTableError = GetIpNetTable(m_ipNetTable, &l_size, FALSE);
        }
        if (l_IpNetTableError != NO_ERROR && l_IpNetTableError != ERROR_NO_DATA)
        {
            printf("GetIpNetTable failed with error code : %d", l_IpNetTableError);
            exit(EXIT_FAILURE);
        }
        // Search the ARP table for the IP address of the packet
        MacAddr l_packetMac(0, 0, 0, 0, 0, 0);
        for (uint32_t i = 0; i < m_ipNetTable->dwNumEntries; ++i)
        {
            if (m_ipNetTable->table[i].dwAddr == inet_addr(l_packetIp.getIpString().c_str()))
            {
                l_packetMac = MacAddr(m_ipNetTable->table[i].bPhysAddr[0],
                                      m_ipNetTable->table[i].bPhysAddr[1],
                                      m_ipNetTable->table[i].bPhysAddr[2],
                                      m_ipNetTable->table[i].bPhysAddr[3],
                                      m_ipNetTable->table[i].bPhysAddr[4],
                                      m_ipNetTable->table[i].bPhysAddr[5]);
                break;
            }
        }
        Client l_packetClient(l_packetIp, l_packetMac);
        // ignore packets from machine itself
        // avoid the broadcast packet from itself
        // So compare the IP address of the packet with the IP address of the machine
        bool l_ignore = false;
        if (l_packetIp == m_host.getIP())
        {
            l_ignore = true;
        }
        // Also ignore the ip that not begin like the ip of the machine (mask <=16)
        if (l_packetIp.getIp1() != m_host.getIP().getIp1()
         || l_packetIp.getIp2() != m_host.getIP().getIp2() )
        {
            l_ignore = true;
        }

        if (l_ignore)
        {
            // Set the packet to NULL
            m_udpPacket.setData(NULL, 0);
        }
        else
        {
            // Set the length of the packet
            m_udpPacket.setLen(recv_len);
            // Set the client of the packet
            m_udpPacket.setClient(l_packetClient);
        }
        assert(l_packetMac != MacAddr(0, 0, 0, 0, 0, 0) || l_ignore);
    }
    else
    {
        // Set the packet to NULL
        m_udpPacket.setData(NULL, 0);
    }
}

void udp::sendbroadcast(char *msg, uint32_t msgSize)
{
    char broadcast = '1';
    if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        printf("Error in setting Broadcast option");
        exit(EXIT_FAILURE);
    }

    //setup address structure
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.S_un.S_addr = INADDR_BROADCAST;

    //send the message
    if (sendto(s, msg, msgSize , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
    {
        printf("sendto() failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

udpPacket *udp::getPacket()
{
    // Receive the packet
    recv();
    // Return the packet received
    return &m_udpPacket;
}

void udp::close()
{
    closesocket(s);
    WSACleanup();
}

udp g_udp;
