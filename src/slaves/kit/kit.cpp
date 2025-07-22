#include "kit.hpp"

#include "tools/logStream/logStream.hpp"
#include "network/client/client.hpp"
#include "api/adc/adc.hpp"
#include "api/circleLeds/circleLeds.hpp"
#include "network/interCom/interMsgList/interMsgAdc/interMsgAdc.hpp"

Client Kit::m_masterClient;
periodicCallsMs Kit::m_adcPeriodicCall(1000, adc_send, nullptr);

Kit::Kit()
{
}

Kit::~Kit()
{
}

void Kit::adc_send(void *object)
{
    adc_measure_t l_adcValue = adc_read();
    LogStream::cout << "ADC value: " << (int)l_adcValue << LogStream::endl;
    InterMsgAdc(m_masterClient, l_adcValue).send();
}

void Kit::init()
{
    LogStream::cout << "Kit init" << LogStream::endl;

    adc_init();
    circleLedsInit();
}

void Kit::process()
{
}

Client &Kit::getMasterClient()
{
    return m_masterClient;
}

void Kit::setMasterClient(Client &client)
{
    if (m_masterClient.getIP() != client.getIP())
    {
        init();
        m_masterClient = client;
        LogStream::cout << "New master client: " << client.getIP().getIpString() << LogStream::endl;
    }
}
