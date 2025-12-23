/**
 * @file interMsgExample.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "interMsgExample.hpp"
#include "api/network/networkConversion/networkConversion.hpp"
#include "tools/logStream/logStream.hpp"

InterMsgExample::InterMsgExample(Client client, int exampleData)
    : InterMsg(client, InterMsgId::Example, m_sendType)
{
    m_exampleData = exampleData;
}

InterMsgExample::InterMsgExample(MessageReceivedCallback callback)
    : InterMsg(callback, InterMsgId::Example)
{
}

uint32_t InterMsgExample::serializePriv(char *data) const
{
    // Serialize the example data using network byte order
    uint32_t exampleDataNetwork = network_htonl(m_exampleData);
    std::memcpy(data, &exampleDataNetwork, sizeof(exampleDataNetwork));
    std::memcpy(data + sizeof(exampleDataNetwork),
                m_exampleString,
                sizeof(m_exampleString));
    return sizeof(exampleDataNetwork) + sizeof(m_exampleString);
}

uint32_t InterMsgExample::deserializePriv(const char *data)
{
    // Deserialize the example data using network byte order
    uint32_t exampleDataNetwork;
    std::memcpy(&exampleDataNetwork, data, sizeof(exampleDataNetwork));
    m_exampleData = network_ntohl(exampleDataNetwork);
    std::memcpy(m_exampleString,
                data + sizeof(exampleDataNetwork),
                sizeof(m_exampleString));
    return sizeof(exampleDataNetwork) + sizeof(m_exampleString);
}

int InterMsgExample::getExampleData() const
{
    return m_exampleData;
}

std::string InterMsgExample::toString() const
{
    return "InterMsgExample: exampleData = " + std::to_string(m_exampleData)
           + ", client = " + getClient().getIP().getIpString();
}
