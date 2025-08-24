#include "interComParser.hpp"
#include "api/network/networkConversion/networkConversion.hpp"
#include "api/network/tcp/tcp.hpp"
#include "api/network/udp/udp.hpp"
#include "network/interCom/interMsg/interMsg.hpp"
#include "tools/logStream/logStream.hpp"
#include "tools/os/async/async.hpp"

std::map<InterMsgId, InterMsg *> InterComParser::m_callbacks;
std::vector<IncompletMsg> InterComParser::m_incompletMsgs;

InterComParser::InterComParser()
    : m_periodicTask(10, std::bind(&InterComParser::periodicTask, this))
{
    // Initialize the TCP and UDP connections
    //udp_init();
    //tcp_init();
}

void InterComParser::periodicTask()
{
    // Pull incomplete messages from udp and tcp api
    //std::vector<IncompletMsg> udpMsgs = udp_recv();
    //std::vector<IncompletMsg> tcpMsgs = tcp_recv();
    //// And process them
    //processIncomingData(&udpMsgs);
    //processIncomingData(&tcpMsgs);
}

void InterComParser::processIncomingData(
    std::vector<IncompletMsg> *incompletMsgs)
{
    if (incompletMsgs == nullptr)
    {
        return;
    }

    for (auto &msg : *incompletMsgs)
    {
        processIncompleteMessage(&msg);
    }
}

void InterComParser::processIncompleteMessage(IncompletMsg *incompleteMsg)
{
    // First, check if an incomplete msg is already buffered from this client
    IncompletMsg *l_incompleteMsg = nullptr;
    uint32_t incompletMsgIndex = 0;
    for (auto &it : m_incompletMsgs)
    {
        if (it.getClient() == incompleteMsg->getClient())
        {
            // If an incomplete message is found, append the new data to it
            it.appendData(incompleteMsg->getData(), incompleteMsg->getSize());
            l_incompleteMsg = &it;
            incompletMsgIndex = &it - &m_incompletMsgs[0];
            break;
        }
    }

    // Then, create a new IncompletMsg if none was found
    if (l_incompleteMsg == nullptr)
    {
        m_incompletMsgs.push_back(*incompleteMsg);
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
    {
        return;
    }

    const char *buffer = incompleteMsg->getData();
    uint32_t totalSize = incompleteMsg->getSize();
    uint32_t processedBytes = 0;
    uint32_t messagesProcessed = 0;

    // Process all concatenated messages in the buffer
    while (processedBytes < totalSize)
    {
        // Check if we have enough data for at least a header
        if (totalSize - processedBytes < InterMsg::m_privDataOffset)
        {
            // Not enough data for a complete header, break and wait for more data
            break;
        }

        // Get the msg ID from the current position in the buffer
        InterMsgId l_msgId(network_ntohl(*reinterpret_cast<const uint32_t *>(
            buffer + processedBytes + InterMsg::m_idOffset)));

        // Validate the msg ID
        if (!l_msgId.isValid())
        {
            LogStream::cout << "Invalid message ID from "
                            << incompleteMsg->getClient().getIP().getIpString()
                            << " (" << "message ID: " << l_msgId.rawValue()
                            << ")" << LogStream::endl;
            // Remove the corrupted message buffer
            m_incompletMsgs.erase(m_incompletMsgs.begin() + incompletMsgIndex);
            return;
        }

        // Get the size of private data
        uint32_t l_privDataSize
            = network_ntohl(*reinterpret_cast<const uint32_t *>(
                buffer + processedBytes + InterMsg::m_lenOffset));

        // Validate message size to prevent buffer overflows
        if (l_privDataSize > InterMsg::m_maxPrivDataSize)
        {
            LogStream::cout
                << "Invalid message size from "
                << incompleteMsg->getClient().getIP().getIpString() << " ("
                << "message ID: " << l_msgId.rawValue()
                << ", size: " << l_privDataSize
                << " bytes, max allowed: " << InterMsg::m_maxPrivDataSize << ")"
                << LogStream::endl;
            // Remove the corrupted message buffer
            m_incompletMsgs.erase(m_incompletMsgs.begin() + incompletMsgIndex);
            return;
        }

        // Calculate the expected message size
        uint32_t expectedMsgSize = InterMsg::m_privDataOffset + l_privDataSize;

        // Check if we have enough data for the complete message
        if (totalSize - processedBytes < expectedMsgSize)
        {
            // Not enough data for the complete message, break and wait for more data
            break;
        }

        // We have a complete message, process it
        this->processMessage(
            incompleteMsg->getClient(), l_msgId, buffer + processedBytes);
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
    }
    else
    {
        LogStream::cout << "mmh" << LogStream::endl;
    }
}

void InterComParser::processMessage(const Client &client,
                                    InterMsgId msgId,
                                    const char *data)
{
    // Find the registered msg instances with the incoming ID
    InterMsg *registeredMsg = m_callbacks[msgId];
    if (registeredMsg == nullptr)
    {
        LogStream::cout << "Warning: No registered message found for ID "
                        << msgId.rawValue() << LogStream::endl;
        return;
    }

    // Call the method to call on msg reception
    registeredMsg->onReception(client, data);
}

void InterComParser::registerDeserializer(InterMsg *msg)
{
    if (msg == nullptr)
    {
        LogStream::cout << "Warning: Null message cannot be registered"
                        << LogStream::endl;
        return;
    }
    // Get the Id of the message
    InterMsgId msgId = msg->getId();
    m_callbacks[msgId] = msg;
}
