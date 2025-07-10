#include "interComParser.hpp"
#include "api/udp/udp.hpp"
#include "api/tcp/tcp.hpp"
#include "network/interCom/interMsg/interMsg.hpp"
#include "tools/logStream/logStream.hpp"

#include "network/interCom/interMsgList/interMsgGeneric/interMsgGeneric.hpp"

std::map<InterMsgId, InterComParser::CallbackInfo> InterComParser::m_callbacks = {};
std::map<std::pair<Client, InterMsgId>, InterComParser::CallbackInfo> InterComParser::m_clientCallbacks = {};

InterComParser::InterComParser() : m_periodicCall(PERIODIC_CALL_INTERVAL_MS, InterComParser::periodicCallBack, this)
{
}

void InterComParser::processMessage(char msg[MAX_MESSAGE_SIZE], uint32_t msgSize, const char *ipAddress, uint16_t port, const char *macAddress)
{
    // Create an generic InterMsg from the received data
    Ipv4 l_ip(ipAddress);
    Client l_client(l_ip, macAddress);
    InterMsgGeneric l_msgGeneric(l_client, msg, msgSize);
    // Get the message ID
    InterMsgId l_msgId = l_msgGeneric.getId();

    // Check for client-specific callbacks first
    auto clientCallbackKey = std::make_pair(l_client, l_msgId);
    auto clientIt = m_clientCallbacks.find(clientCallbackKey);
    if (clientIt != m_clientCallbacks.end())
    {
        // If client-specific callback is found, call it
        InterMsg &l_msg = l_msgGeneric;
        clientIt->second.callback(l_msg.getClient(), l_msg);
        return;
    }

    // Find the general callbacks of the message ID
    auto it = m_callbacks.find(l_msgId);
    if (it != m_callbacks.end())
    {
        // If callbacks are found, call them with the message
        InterMsg &l_msg = l_msgGeneric;
        it->second.callback(l_msg.getClient(), l_msg);
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
    m_callbacks[msgId] = CallbackInfo(callback);
}

void InterComParser::registerCallback(InterMsgId msgId, MessageReceivedCallback callback, void *object)
{
    // Register the callback for the specified message ID with object pointer
    m_callbacks[msgId] = CallbackInfo(callback, object);
}

void InterComParser::registerCallback(const Client &client, InterMsgId msgId, MessageReceivedCallback callback)
{
    // Register the callback for the specified client and message ID
    auto key = std::make_pair(client, msgId);
    m_clientCallbacks[key] = CallbackInfo(callback);
}

void InterComParser::registerCallback(const Client &client, InterMsgId msgId, MessageReceivedCallback callback, void *object)
{
    // Register the callback for the specified client and message ID with object pointer
    auto key = std::make_pair(client, msgId);
    m_clientCallbacks[key] = CallbackInfo(callback, object);
}
