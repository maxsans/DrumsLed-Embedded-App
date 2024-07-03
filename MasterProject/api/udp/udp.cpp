#include "udp.h"

#include "udpParser.h"

#include <stdio.h>
#include <stdint.h>
#include <winsock2.h>

#define PORT 9999

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
}

void udp::send(char *ip_addr, char *msg, uint32_t msgSize)
{
    //setup address structure
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(ip_addr);

    //send the message
    if (sendto(s, msg, msgSize , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
    {
        printf("sendto() failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

void udp::recv()
{
    //clear the buffer by filling null, it might have previously received data
    memset(buf,'\0', BUFLEN);

    DWORD read_timeout = 1;  // it is milliseconds!
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char const*)&read_timeout, sizeof read_timeout) != 0)
    {
        printf("setsockopt failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    //try to receive some data, this is a blocking call with 1ms of timeout
    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) != SOCKET_ERROR)
    {
        char *l_packetIp;
        l_packetIp = (char *)malloc(strlen(inet_ntoa(si_other.sin_addr)) + 1);
        strcpy(l_packetIp, inet_ntoa(si_other.sin_addr));
        // ignore packets from machine itself
        // avoid the broadcast packet from itself
        // So get the IP address of the machine and compare it with the IP address of the packet
        bool l_ignore = false;
        char ac[80];
        if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR)
        {
            printf("Error %d when getting local host name.\n", WSAGetLastError());
            l_ignore = true;
        }
        else
        {
            struct hostent *phe = gethostbyname(ac);
            for (int i = 0; phe->h_addr_list[i] != 0; ++i)
            {
                char *l_localIp = inet_ntoa(*(struct in_addr *)phe->h_addr_list[i]);

                if (strcmp(l_localIp, l_packetIp) == 0)
                {
                    // ignore the packet
                    l_ignore = true;
                    // printf("Packet ignored\n");
                    break;
                }
            }
        }

        if (l_ignore == false)
        {
            // printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
            // printf("Data: %s\n" , buf);

            //  Now, parse the data and do something with it
            g_udpParser.parseUdp(buf, inet_ntoa(si_other.sin_addr));
        }
        if (l_packetIp != NULL)
        {
            free(l_packetIp);
        }
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

void udp::close()
{
    closesocket(s);
    WSACleanup();
}

udp g_udp;
