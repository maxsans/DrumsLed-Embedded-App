#include "ledManager.h"

#include "udp.h"
#include "udpParser.h"
#include "assert.h"

#include <sys/time.h>

#define UPDATE_INTERVAL 20 // ms

ledManager::ledManager()
{
    // Constructor
    m_updatePeriodicCalls.setCallback(updateCallBack, this);
    m_updatePeriodicCalls.setPeriod(UPDATE_INTERVAL);
}

ledManager::~ledManager()
{
    // Destructor
    for (uint8_t i = 0; i < m_leds.size(); i++)
    {
        delete m_leds[i];
    }
}

void ledManager::addLed(module *m)
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
    m_leds.push_back(new rgbLed(m));
}

rgbLed *ledManager::getLed(uint8_t index)
{
    // Get a led by index
    assert(index < m_leds.size());
    return m_leds[index];
}

rgbLed *ledManager::getLed(module *m)
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

uint32_t ledManager::getLedCount()
{
    // Get the number of leds
    return m_leds.size();
}

void ledManager::updateCallBack(void *object)
{
    // Callback to update the leds
    ((ledManager*)object)->update();
}

void ledManager::update()
{
    // Update all leds
    for (uint8_t i = 0; i < m_leds.size(); i++)
    {
        // Send the color to the module over UDP
        uint8_t redValue, greenValue, blueValue;
        rgbColor color = m_leds[i]->getColor();
        color.getColor(&redValue, &greenValue, &blueValue);
        char msg[] =
        {
            PACKET_TYPE_RGB,
            (char)redValue,
            (char)greenValue,
            (char)blueValue
        };
        m_leds[i]->getModule()->getClient().send(msg, sizeof(msg));
    }
}

module *ledManager::getModule(uint8_t index)
{
    // Get a module by index
    assert(index < m_leds.size());
    return m_leds[index]->getModule();
}

module *ledManager::getModule(module *m)
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
