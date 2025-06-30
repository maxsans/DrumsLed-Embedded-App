#include "interMsgRgb.hpp"

#include "tools/logStream/logStream.h"

InterMsgRgb::InterMsgRgb(Client client, uint8_t red, uint8_t green, uint8_t blue)
    : InterMsg(client, InterMsgData(InterMsgHeader(InterMsgId::Rgb, sizeof(m_data)), (char *)&m_data), m_sendType)
{
    m_data.red = red;
    m_data.green = green;
    m_data.blue = blue;
}

InterMsgRgb::InterMsgRgb(Client client, char *rawData, uint32_t size)
    : InterMsg(client, InterMsgData(rawData, size, (char *)&m_data), m_sendType)
{
}

const char* InterMsgRgb::getPrivData() const
{
    return (const char *)&m_data;
}

uint8_t InterMsgRgb::getRed() const
{
    return m_data.red;
}

uint8_t InterMsgRgb::getGreen() const
{
    return m_data.green;
}

uint8_t InterMsgRgb::getBlue() const
{
    return m_data.blue;
}

std::string InterMsgRgb::toString() const
{
    return "InterMsgRgb: RGB(" + std::to_string(m_data.red) +
           ", " + std::to_string(m_data.green) +
           ", " + std::to_string(m_data.blue) +
           ", client = " + getClient().getIP().getIpString();
}
