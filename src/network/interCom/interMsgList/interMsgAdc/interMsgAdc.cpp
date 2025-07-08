#include "interMsgAdc.hpp"

#include "tools/logStream/logStream.h"
#include <cstring>

InterMsgAdc::InterMsgAdc(Client client, adc_measure_t adcValue)
    : InterMsg(client, InterMsgData(InterMsgHeader(InterMsgId::Adc, sizeof(m_data)), (char *)&m_data), m_sendType)
{
    m_data.m_adcValue = adcValue;
}

InterMsgAdc::InterMsgAdc(Client client, char *rawData, uint32_t size)
    : InterMsg(client, InterMsgData(rawData, size, (char *)&m_data), m_sendType)
{
}

const char* InterMsgAdc::getPrivData() const
{
    return reinterpret_cast<const char*>(&m_data);
}

adc_measure_t InterMsgAdc::getAdcValue() const
{
    return m_data.m_adcValue;
}

std::string InterMsgAdc::toString() const
{
    return "InterMsgAdc: adcValue = " + std::to_string(m_data.m_adcValue) +
           ", client = " + getClient().getIP().getIpString();
}
