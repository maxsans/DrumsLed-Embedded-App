#include "ledManager.h"

#include "api/udp/udp.h"
#include "network/udpParser/udpPackets/udpPacketRgb.h"
#include "assert.h"

#include <sys/time.h>

#define UPDATE_INTERVAL 20 // ms

LedManager::LedManager()
{
    // Constructor
    m_updatePeriodicCalls.setCallback(updateCallBack, this);
    m_updatePeriodicCalls.setPeriod(UPDATE_INTERVAL);
}

LedManager::~LedManager()
{
    // Destructor
    for (uint8_t i = 0; i < m_leds.size(); i++)
    {
        delete m_leds[i];
    }
}

void LedManager::addLed(Module *m)
{
    // Check if the led already exists
    for (uint8_t i = 0; i < m_leds.size(); i++)
    {
        if (m_leds[i]->getModule() == m)
        {
            return; // Already exists, dont add again
        }
    }
    // Add a new led to the manager
    m_leds.push_back(new RgbLed(m));
}

RgbLed *LedManager::getLed(uint8_t index)
{
    // Get a led by index
    assert(index < m_leds.size());
    return m_leds[index];
}

RgbLed *LedManager::getLed(Module *m)
{
    // Get a led by module
    for (uint8_t i = 0; i < m_leds.size(); i++)
    {
        if (m_leds[i]->getModule() == m)
        {
            return m_leds[i];
        }
    }
    assert(false); // Led not found
    return NULL;
}

uint32_t LedManager::getLedCount()
{
    // Get the number of leds
    return m_leds.size();
}

void LedManager::updateCallBack(void *object)
{
    // Callback to update the leds
    ((LedManager*)object)->update();
}

void LedManager::update()
{
    // Update all leds
    for (uint8_t i = 0; i < m_leds.size(); i++)
    {
        // Send the color to the module over UDP
        RgbColor l_color = m_leds[i]->getColor();
        Client l_client = m_leds[i]->getModule()->getClient();
        UdpPacketRgb(l_client, l_color).send();
    }
}

Module *LedManager::getModule(uint8_t index)
{
    // Get a module by index
    assert(index < m_leds.size());
    return m_leds[index]->getModule();
}

Module *LedManager::getModule(Module *m)
{
    // Get a module by led
    for (uint8_t i = 0; i < m_leds.size(); i++)
    {
        if (m_leds[i]->getModule() == m)
        {
            return m_leds[i]->getModule();
        }
    }
    return NULL;
}
