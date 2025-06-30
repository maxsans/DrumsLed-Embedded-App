#include "interComParser.hpp"
#include "api/udp/udp.h"
#include "api/tcp/tcp.h"
#include "network/interCom/interMsg/interMsg.hpp"
#include "tools/logStream/logStream.h"

#include "network/interCom/interMsgList/interMsgGeneric/interMsgGeneric.hpp"

std::map<InterMsgId, InterComParser::MessageReceivedCallback> InterComParser::m_callbacks = {};

InterComParser::InterComParser() : m_periodicCall(PERIODIC_CALL_INTERVAL_MS, InterComParser::periodicCallBack, this)
{
}

void InterComParser::processMessage(char msg[MAX_MESSAGE_SIZE], uint32_t msgSize, const char *ipAddress, uint16_t port, const char *macAddress)
{
    // Create an generic InterMsg from the received data
    Ipv4 l_ip(ipAddress);
    InterMsgGeneric l_msgGeneric(Client(l_ip, macAddress), msg, msgSize);
    // Get the message ID
    InterMsgId l_msgId = l_msgGeneric.getId();
    // Find the callbacks of the message ID
    auto it = m_callbacks.find(l_msgId);
    if (it != m_callbacks.end())
    {
        // If callbacks are found, call them with the message
        InterMsg &l_msg = l_msgGeneric;
        it->second(l_msg.getClient(), l_msg);
    }
    else
    {
        // If no callbacks are found, log a warning
        LogStream::cout << "No callback registered for message ID: " << l_msgId.rawValue();
    }
}

void InterComParser::checkIncomingMessages()
{
    // Try to receive messages from UDP and TCP
    // Udp
    char l_udpData[MAX_MESSAGE_SIZE];
    char l_udpIp[16]; // Buffer for IP address
    char l_udpMac[18]; // Buffer for MAC address
    int16_t l_udpPort;
    uint32_t l_udpBytesReceived = udp_recv(l_udpData, MAX_MESSAGE_SIZE, l_udpIp, &l_udpPort, l_udpMac);
    if (l_udpBytesReceived > 0)
    {
        processMessage(l_udpData, l_udpBytesReceived, l_udpIp, l_udpPort, l_udpMac);
    }
    // Tcp
    char l_tcpData[MAX_MESSAGE_SIZE];
    char l_tcpIp[16]; // Buffer for IP address
    char l_tcpMac[18]; // Buffer for MAC address
    int16_t l_tcpPort;
    uint32_t l_tcpBytesReceived = tcp_recv(l_tcpData, MAX_MESSAGE_SIZE, l_tcpIp, &l_tcpPort, l_tcpMac);
    if (l_tcpBytesReceived > 0)
    {
        processMessage(l_tcpData, l_tcpBytesReceived, l_tcpIp, l_tcpPort, l_tcpMac);
    }
}

void InterComParser::periodicCall()
{
    checkIncomingMessages();
}

void InterComParser::periodicCallBack(void *object)
{
    InterComParser *parser = static_cast<InterComParser *>(object);
    parser->periodicCall();
}

void InterComParser::registerCallback(InterMsgId msgId, MessageReceivedCallback callback)
{
    // Register the callback for the specified message ID
    m_callbacks[msgId] = callback;
}
