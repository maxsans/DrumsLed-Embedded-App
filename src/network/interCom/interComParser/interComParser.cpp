#include "interComParser.hpp"
#include "api/tcp/tcp.hpp"
#include "api/udp/udp.hpp"
#include "network/interCom/interMsg/interMsg.hpp"
#include "network/interCom/interMsgData/interMsgHeader/interMsgHeader.hpp"
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

    // Process all complete messages that might be concatenated
    this->processCompleteMessages(l_incompleteMsg, incompletMsgIndex);
}

void InterComParser::processCompleteMessages(IncompletMsg *incompleteMsg,
                                             uint32_t incompletMsgIndex)
{
    if (incompleteMsg == nullptr)
        return;

    const char *buffer = incompleteMsg->getData();
    uint32_t totalSize = incompleteMsg->getSize();
    uint32_t processedBytes = 0;
    uint32_t messagesProcessed = 0;

    // Process all concatenated messages in the buffer
    while (processedBytes < totalSize)
    {
        // Check if we have enough data for at least a header
        if (totalSize - processedBytes < InterMsgHeader::getHeaderSize())
        {
            // Not enough data for a complete header, break and wait for more data
            LogStream::cout << "Incomplete header from "
                            << incompleteMsg->getClient().getIP().getIpString()
                            << " (" << (totalSize - processedBytes)
                            << " bytes remaining)" << LogStream::endl;
            break;
        }

        // Create a header from the current position in the buffer
        InterMsgHeader header(buffer + processedBytes);
        uint32_t expectedMsgSize
            = InterMsgHeader::getHeaderSize() + header.getPrivSize();

        // Validate message size to prevent buffer overflows
        if (expectedMsgSize > MAX_MESSAGE_SIZE)
        {
            LogStream::cout << "Message size too large from "
                            << incompleteMsg->getClient().getIP().getIpString()
                            << " (" << expectedMsgSize
                            << " bytes, max allowed: " << MAX_MESSAGE_SIZE
                            << ")" << LogStream::endl;
            // Remove the corrupted message buffer
            m_incompletMsgs.erase(m_incompletMsgs.begin() + incompletMsgIndex);
            return;
        }

        // Check if we have enough data for the complete message
        if (totalSize - processedBytes < expectedMsgSize)
        {
            // Not enough data for the complete message, break and wait for more data
            LogStream::cout << "Incomplete message from "
                            << incompleteMsg->getClient().getIP().getIpString()
                            << " (" << (totalSize - processedBytes)
                            << " bytes available, " << expectedMsgSize
                            << " bytes needed)" << LogStream::endl;
            break;
        }

        // We have a complete message, process it
        this->processMessage(buffer + processedBytes,
                             expectedMsgSize,
                             incompleteMsg->getClient());
        processedBytes += expectedMsgSize;
        messagesProcessed++;
    }

    // Log if we processed multiple concatenated messages
    if (messagesProcessed > 1)
    {
        LogStream::cout << "Processed " << messagesProcessed
                        << " concatenated messages from "
                        << incompleteMsg->getClient().getIP().getIpString()
                        << LogStream::endl;
    }

    // If we processed all data, remove the incomplete message
    if (processedBytes >= totalSize)
    {
        m_incompletMsgs.erase(m_incompletMsgs.begin() + incompletMsgIndex);
    }
    else if (processedBytes > 0)
    {
        // If we processed some data but not all, we need to keep the remaining data
        // This handles the case where the last message in a concatenated buffer is incomplete
        uint32_t remainingSize = totalSize - processedBytes;

        // Update the incomplete message with the remaining data
        incompleteMsg->replaceData(buffer + processedBytes, remainingSize);

        LogStream::cout << "Kept " << remainingSize
                        << " bytes of incomplete data from "
                        << incompleteMsg->getClient().getIP().getIpString()
                        << LogStream::endl;
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
