#include "interMsg.hpp"

#include "api/network/networkConversion/networkConversion.hpp"
#include "api/network/tcp/tcp.hpp"
#include "api/network/udp/udp.hpp"
#include <cassert>
#include <cstring>

InterMsg::InterMsg(Client client, InterMsgId id, SendType sendType)
    : m_client(client), m_id(id), m_sendType(sendType)
{
}

InterMsg::InterMsg(InterMsgId id) : m_id(id), m_sendType(SendType::Broadcast)
{
}

InterMsg::InterMsg(MessageReceivedCallback callback, InterMsgId id)
    : m_id(id), m_sendType(SendType::None), m_callback(callback)
{
}

void InterMsg::send()
{
    const size_t l_maxSize = 8 + m_maxPrivDataSize;
    char l_data[l_maxSize];
    // Add the msg ID
    char *l_idData = l_data + m_idOffset;
    *reinterpret_cast<uint32_t *>(l_idData) = network_htonl(m_id.rawValue());
    // Add the private data of the msg by serializing it
    // Note : serializePriv() put the data in network byte order
    char *l_privData = l_data + m_privDataOffset;
    uint32_t l_privSize = serializePriv(l_privData);

    // Prevent buffer overflow
    if (l_privSize > m_maxPrivDataSize)
    {
        // Optionally log or handle the error
        assert(false
               && "serializePriv returned size larger than m_maxPrivDataSize");
        return;
    }

    // Add the msg len
    char *l_lenData = l_data + m_lenOffset;
    *reinterpret_cast<uint32_t *>(l_lenData) = network_htonl(l_privSize);

    // Get the total message size
    uint32_t l_size = m_privDataOffset + l_privSize;

    // Get the IP address
    const char *l_ip = m_client.getIP().getIpString().c_str();

    // Send the message
    switch (m_sendType)
    {
        case SendType::UnicastWithControl:
            tcp_send(l_data, l_size, l_ip, TCP_DEFAULT_PORT);
            break;

        case SendType::UnicastWithoutControl:
            udp_send(l_data, l_size, l_ip, UDP_DEFAULT_PORT);
            break;

        case SendType::Broadcast:
            udp_send_broadcast(l_data, l_size, UDP_DEFAULT_PORT);
            break;

        default:
            break;
    }
}

uint32_t InterMsg::onReception(const Client &client, const char *msg)
{
    // Deserialize the message data
    const char *l_privData = msg + m_privDataOffset;
    int32_t l_deserializeResult = deserializePriv(l_privData);

    if (l_deserializeResult < 0)
    {
        // The message is incomplete
        return 0;
    }

    // Call the message received callback
    if (m_callback)
    {
        m_callback(client, *this);
    }

    // Return the total length of the message
    return m_privDataOffset + l_deserializeResult;
}

const Client &InterMsg::getClient() const
{
    return m_client;
}

InterMsgId InterMsg::getId() const
{
    return m_id;
}

std::string InterMsg::toString() const
{
    return "InterMsg: " + m_client.getIP().getIpString()
           + ", ID: " + std::to_string(m_id.rawValue())
           + ", SendType: " + std::to_string(static_cast<int>(m_sendType));
}
