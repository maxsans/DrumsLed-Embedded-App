#include "drumModule.h"

#include "network/udpParser/udpPackets/udpPacketRgb.h"

/**
 * @brief The update interval of the LEDs.
 * @note Unit: ms
 */
#define UPDATE_INTERVAL 20

DrumModule::DrumModule(Client client) :
    Module(TYPE_DRUM_MODULE, client),
    m_updatePeriodicCalls(UPDATE_INTERVAL, updateCallBack, this)
{
}

void DrumModule::process()
{
    // Process the micro
    m_micro.process();
}

Micro *DrumModule::getMicro()
{
    return &m_micro;
}

RgbLed *DrumModule::getRgbLed()
{
    return &m_led;
}

void DrumModule::updateCallBack(void *object)
{
    // Callback to update the leds
    ((DrumModule*)object)->update();
}

void DrumModule::update()
{
    // Send the color to the module over UDP
    RgbColor l_color = m_led.getColor();
    UdpPacketRgb(m_client, l_color).send();
}

void DrumModule::setMicroValue(uint8_t microValue)
{
    // Set the micro value
    m_micro.setMicroValue(microValue);
}
