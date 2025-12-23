/**
 * @file interMsgOtaStart.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "interMsgOtaStart.hpp"
#include "api/network/networkConversion/networkConversion.hpp"
#include <cstring>

InterMsgOtaStart::InterMsgOtaStart(Client client, int64_t length)
    : InterMsg(client, InterMsgId::OtaStart, m_sendType), m_length(length)
{
}

InterMsgOtaStart::InterMsgOtaStart(MessageReceivedCallback callback)
    : InterMsg(callback, InterMsgId::OtaStart)
{
}

uint32_t InterMsgOtaStart::serializePriv(char *data) const
{
    // Serialize the OTA start message using network byte order
    uint64_t lengthNetwork = network_htonll(m_length);
    std::memcpy(data, &lengthNetwork, sizeof(lengthNetwork));
    return sizeof(lengthNetwork);
}

uint32_t InterMsgOtaStart::deserializePriv(const char *data)
{
    // Deserialize the data using network byte order
    uint64_t lengthNetwork;
    std::memcpy(&lengthNetwork, data, sizeof(lengthNetwork));
    m_length = network_ntohll(lengthNetwork);
    return sizeof(lengthNetwork);
}

int64_t InterMsgOtaStart::getLength() const
{
    return m_length;
}

bool InterMsgOtaStart::isOtaNeeded() const
{
    return m_length > 0;
}

std::string InterMsgOtaStart::toString() const
{
    std::string str;
    if (m_length > 0)
    {
        str = "InterMsgOtaStart: length = " + std::to_string(m_length)
              + ", client = " + getClient().getIP().getIpString();
    }
    else
    {
        str = "InterMsgOtaStart: OTA not needed, client = "
              + getClient().getIP().getIpString();
    }
    return str;
}
