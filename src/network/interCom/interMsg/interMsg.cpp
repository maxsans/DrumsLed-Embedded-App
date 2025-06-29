#include "interMsg.hpp"

#include "api/udp/udp.h"
#include "api/tcp/tcp.h"

#include <cassert>
#include <cstring>

InterMsg::InterMsg(Client client,
                   InterMsgData data,
                   SendType sendType)
    : m_client(client), m_data(data), m_sendType(sendType)
{
}

void InterMsg::send()
{
    char l_data[InterMsgData::MAX_PRIV_DATA_SIZE];
    size_t l_size = m_data.getPrivData(l_data);
    const char *l_ip = m_client.getIP().getIpString().c_str();
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

const Client& InterMsg::getClient() const
{
    return m_client;
}

bool InterMsg::operator==(const InterMsg *other) const
{
    return (m_client == other->m_client &&
            m_data == other->m_data);
}

std::string InterMsg::toString() const
{
    return "InterMsg: " + m_client.getIP().getIpString() +
           ", ID: " + std::to_string(m_data.getHeader().getId().rawValue()) +
           ", PrivSize: " + std::to_string(m_data.getHeader().getPrivSize()) +
           ", SendType: " + std::to_string(static_cast<int>(m_sendType));
}
