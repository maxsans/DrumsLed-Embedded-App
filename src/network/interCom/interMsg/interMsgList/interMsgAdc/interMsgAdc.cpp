#include "interMsgAdc.hpp"

#include "api/network/networkConversion/networkConversion.hpp"
#include "tools/logStream/logStream.hpp"
#include <cstring>

InterMsgAdc::InterMsgAdc(Client client, adc_measure_t adcValue)
    : InterMsg(client, InterMsgId::Adc, m_sendType)
{
    m_adcValue = adcValue;
}

InterMsgAdc::InterMsgAdc(MessageReceivedCallback callback)
    : InterMsg(callback, InterMsgId::Adc)
{
}

uint32_t InterMsgAdc::serializePriv(char *data) const
{
    // Serialize the ADC data using network byte order
    uint32_t adcValueNetwork = network_htonl(m_adcValue);
    std::memcpy(data, &adcValueNetwork, sizeof(adcValueNetwork));
    return sizeof(adcValueNetwork);
}

uint32_t InterMsgAdc::deserializePriv(const char *data)
{
    // Deserialize the ADC data using network byte order
    uint32_t adcValueNetwork;
    std::memcpy(&adcValueNetwork, data, sizeof(adcValueNetwork));
    m_adcValue = network_ntohl(adcValueNetwork);
    return sizeof(adcValueNetwork);
}

adc_measure_t InterMsgAdc::getAdcValue() const
{
    return m_adcValue;
}

std::string InterMsgAdc::toString() const
{
    return "InterMsgAdc: adcValue = " + std::to_string(m_adcValue)
           + ", client = " + getClient().getIP().getIpString();
}
