/**
 * @file interMsgOtaStart.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "interMsgOtaStart.hpp"
#include <cstring>

InterMsgOtaStart::InterMsgOtaStart(Client client, int64_t length)
    : InterMsg(
          client,
          InterMsgData(InterMsgHeader(InterMsgId::OtaStart, sizeof(m_data)),
                       (char *)&m_data),
          m_sendType),
      m_data{length}
{
}

InterMsgOtaStart::InterMsgOtaStart(Client client, char *rawData, int64_t size)
    : InterMsg(client, InterMsgData(rawData, size, (char *)&m_data), m_sendType)
{
    if (size == sizeof(m_data))
    {
        std::memcpy(&m_data, rawData, sizeof(m_data));
    }
}

int64_t InterMsgOtaStart::getLength() const
{
    return m_data.m_length;
}

bool InterMsgOtaStart::isOtaNeeded() const
{
    return m_data.m_length > 0;
}

std::string InterMsgOtaStart::toString() const
{
    std::string str;
    if (m_data.m_length > 0)
    {
        str = "InterMsgOtaStart: length = " + std::to_string(m_data.m_length)
              + ", client = " + getClient().getIP().getIpString();
    }
    else
    {
        str = "InterMsgOtaStart: OTA not needed, client = "
              + getClient().getIP().getIpString();
    }
    return str;
}
