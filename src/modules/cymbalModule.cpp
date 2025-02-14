#include "cymbalModule.h"

#include "network/udpParser/udpPackets/udpPacketRgb.h"

/**
 * @brief The update interval of the LEDs.
 * @note Unit: ms
 */
#define UPDATE_INTERVAL 20

CymbalModule::CymbalModule(Client client) :
    Module(TYPE_CYMBAL_MODULE, client),
    m_updatePeriodicCalls(UPDATE_INTERVAL, updateCallBack, this)
{
}

void CymbalModule::process()
{
    // Process the micro
    m_micro.process();
}

Micro *CymbalModule::getMicro()
{
    return &m_micro;
}

RgbLed *CymbalModule::getRgbLed()
{
    return &m_led;
}

void CymbalModule::updateCallBack(void *object)
{
    // Callback to update the leds
    ((CymbalModule*)object)->update();
}

void CymbalModule::update()
{
    // Send the color to the module over UDP
    RgbColor l_color = m_led.getColor();
    UdpPacketRgb(m_client, l_color).send();
}

