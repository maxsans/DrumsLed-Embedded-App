#include "kit.h"

#include "tools/logStream/logStream.h"
#include "network/client/client.h"
#include "api/addrLed/addrLed.h"
#include "api/adc/adc.h"
#include "network/interCom/interMsgList/interMsgAdc/interMsgAdc.hpp"

Client Kit::m_masterClient;
AddrLed Kit::m_leds(Kit::m_numLeds, Kit::m_ledPin);
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

    m_leds.init();
    adc_init();
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
