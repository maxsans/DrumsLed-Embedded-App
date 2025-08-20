#include "interComParser.hpp"
#include "api/tcp/tcp.hpp"
#include "api/udp/udp.hpp"
#include "network/interCom/interMsg/interMsg.hpp"
#include "tools/logStream/logStream.hpp"
#include "tools/os/async/async.hpp"

#include "network/interCom/interMsgList/interMsgGeneric/interMsgGeneric.hpp"

std::map<InterMsgId, InterComParser::CallbackInfo> InterComParser::m_callbacks
    = {};
std::map<std::pair<Client, InterMsgId>, InterComParser::CallbackInfo>
    InterComParser::m_clientCallbacks = {};
std::vector<IncompletMsg> InterComParser::m_incompletMsgs = {};

InterComParser::InterComParser()
{
    // Initialize the TCP and UDP connections
    // Call the processIncomingData method with the received data
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
            this->processIncomingData(
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
            this->processIncomingData(
                data, len, ipCopy.c_str(), port, macCopy.c_str());
        });
    });
}

void InterComParser::processIncomingData(const char *data,
                                         int16_t len,
                                         const char *ip,
                                         int16_t port,
                                         const char *mac)
{
    if (len <= 0 || data == nullptr)
    {
        return;
    }

    Ipv4 l_ip(ip);
    MacAddr l_mac(mac);
    Client l_client(l_ip, l_mac);

    // First, check if an incomplete msg is already buffered from this client
    IncompletMsg *l_incompleteMsg = nullptr;
    uint32_t incompletMsgIndex = 0;
    for (auto &it : m_incompletMsgs)
    {
        if (it.getClient() == l_client)
        {
            // If an incomplete message is found, append the new data to it
            it.appendData(data, len);
            l_incompleteMsg = &it;
            incompletMsgIndex = &it - &m_incompletMsgs[0];
            break;
        }
    }

    // Then, create a new IncompletMsg if none was found
    if (l_incompleteMsg == nullptr)
    {
        m_incompletMsgs.emplace_back(l_client, data, len);
        l_incompleteMsg = &m_incompletMsgs.back();
        incompletMsgIndex = m_incompletMsgs.size() - 1;
    }

    // If the incomplete message is complete, process it
    if (l_incompleteMsg->isComplete())
    {
        // Process the complete message
        this->processMessage(l_incompleteMsg->getData(),
                             l_incompleteMsg->getSize(),
                             l_incompleteMsg->getClient());
        // Remove the incomplete message from the vector
        m_incompletMsgs.erase(m_incompletMsgs.begin() + incompletMsgIndex);
    }
}

void InterComParser::processMessage(const char *data,
                                    uint32_t len,
                                    const Client &client)
{
    // Create an generic InterMsg from the received data
    InterMsgGeneric l_msgGeneric(client, const_cast<char *>(data), len);
    // Get the message ID
    InterMsgId l_msgId = l_msgGeneric.getId();

    // Check for client-specific callbacks first
    auto clientCallbackKey = std::make_pair(client, l_msgId);
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
    //else
    //{
    //    // If no callbacks are found, log a warning
    //    LogStream::cout << "No callback registered for message ID: "
    //                    << l_msgId.rawValue() << LogStream::endl;
    //}
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
