#include "interComParser.hpp"
#include "api/tcp/tcp.hpp"
#include "api/udp/udp.hpp"
#include "network/interCom/interMsg/interMsg.hpp"
#include "tools/async/async.hpp"
#include "tools/logStream/logStream.hpp"

#include "network/interCom/interMsgList/interMsgGeneric/interMsgGeneric.hpp"

std::map<InterMsgId, InterComParser::CallbackInfo> InterComParser::m_callbacks
    = {};
std::map<std::pair<Client, InterMsgId>, InterComParser::CallbackInfo>
    InterComParser::m_clientCallbacks = {};

InterComParser::InterComParser()
{
    // Initialize the TCP and UDP connections
    // Call the processMessage method with the received data
    // Asynchronously process the message to avoid blocking the UDP task
    // And memory access issues
    udp_init([this](const char *data,
                    int16_t len,
                    const char *ip,
                    int16_t port,
                    const char *mac) {
        std::string ipCopy(ip);
        std::string macCopy(mac);
        Async::registerAsync([this, data, len, ipCopy, port, macCopy]() {
            this->processMessage(
                data, len, ipCopy.c_str(), port, macCopy.c_str());
        });
    });
    tcp_init([this](const char *data,
                    int16_t len,
                    const char *ip,
                    int16_t port,
                    const char *mac) {
        std::string ipCopy(ip);
        std::string macCopy(mac);
        Async::registerAsync([this, data, len, ipCopy, port, macCopy]() {
            this->processMessage(
                data, len, ipCopy.c_str(), port, macCopy.c_str());
        });
    });
}

void InterComParser::processMessage(const char *data,
                                    int16_t len,
                                    const char *ip,
                                    int16_t port,
                                    const char *mac)
{
    // Create an generic InterMsg from the received data
    Ipv4 l_ip(ip);
    MacAddr l_mac(mac);
    Client l_client(l_ip, l_mac);
    InterMsgGeneric l_msgGeneric(l_client, const_cast<char *>(data), len);
    // Get the message ID
    InterMsgId l_msgId = l_msgGeneric.getId();

    // Check for client-specific callbacks first
    auto clientCallbackKey = std::make_pair(l_client, l_msgId);
    auto clientIt = m_clientCallbacks.find(clientCallbackKey);
    if (clientIt != m_clientCallbacks.end())
    {
        // If client-specific callback is found, call it
        InterMsg &l_msg = l_msgGeneric;
        clientIt->second.callback(
            l_msg.getClient(), l_msg, clientIt->second.object);
        return;
    }

    // Find the general callbacks of the message ID
    auto it = m_callbacks.find(l_msgId);
    if (it != m_callbacks.end())
    {
        // If callbacks are found, call them with the message
        InterMsg &l_msg = l_msgGeneric;
        it->second.callback(l_msg.getClient(), l_msg, it->second.object);
    }
    else
    {
        // If no callbacks are found, log a warning
        LogStream::cout << "No callback registered for message ID: "
                        << l_msgId.rawValue() << LogStream::endl;
    }
}

void InterComParser::registerCallback(InterMsgId msgId,
                                      MessageReceivedCallback callback)
{
    // Register the callback for the specified message ID
    m_callbacks[msgId] = CallbackInfo(callback);
}

void InterComParser::registerCallback(InterMsgId msgId,
                                      MessageReceivedCallback callback,
                                      void *object)
{
    // Register the callback for the specified message ID with object pointer
    m_callbacks[msgId] = CallbackInfo(callback, object);
}

void InterComParser::registerCallback(const Client &client,
                                      InterMsgId msgId,
                                      MessageReceivedCallback callback)
{
    // Register the callback for the specified client and message ID
    auto key = std::make_pair(client, msgId);
    m_clientCallbacks[key] = CallbackInfo(callback);
}

void InterComParser::registerCallback(const Client &client,
                                      InterMsgId msgId,
                                      MessageReceivedCallback callback,
                                      void *object)
{
    // Register the callback for the specified client and message ID with object pointer
    auto key = std::make_pair(client, msgId);
    m_clientCallbacks[key] = CallbackInfo(callback, object);
}

void InterComParser::registerCallback(InterMsgId msgId,
                                      void (*callback)(const Client &,
                                                       InterMsg &))
{
    // Wrap the static callback into a std::function with void* ignored
    m_callbacks[msgId]
        = CallbackInfo([callback](const Client &client, InterMsg &msg, void *) {
              callback(client, msg);
          });
}
