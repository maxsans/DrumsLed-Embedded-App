#include "udp.h"

#include <stdio.h>
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

void udp::send(char *ip_addr, char *msg)
{
    //setup address structure
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(ip_addr);

    //send the message
    if (sendto(s, msg, strlen(msg) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
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

    //try to receive some data, this is a blocking call
    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) != SOCKET_ERROR)
    {
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);

        //  Now, parse the data and do something with it
        // TODO

    }

}

void udp::sendbroadcast(char *msg)
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

    // si_other.sin_family = AF_INET;
    // si_other.sin_port = htons(PORT);
    // // si_other.sin_addr.s_addr = INADDR_BROADCAST; // this isq equiv to 255.255.255.255
    // // better use subnet broadcast (for our subnet is 172.30.255.255)
    // si_other.sin_addr.s_addr = inet_addr("172.30.255.255");

    //send the message
    if (sendto(s, msg, strlen(msg) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
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
