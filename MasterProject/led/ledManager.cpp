#include "ledManager.h"

#include "udp.h"
#include "udpParser.h"

ledManager::ledManager()
{
    // Constructor
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
    return NULL;
}

uint32_t ledManager::getLedCount()
{
    // Get the number of leds
    return m_leds.size();
}

void ledManager::update()
{
    // Update all leds
    for (uint8_t i = 0; i < m_leds.size(); i++)
    {
        // Send the color to the module over UDP
        uint8_t redValue, greenValue, blueValue;
        m_leds[i]->getColor(&redValue, &greenValue, &blueValue);
        char msg[] =
        {
            PACKET_TYPE_RGB,
            (char)redValue,
            (char)greenValue,
            (char)blueValue
        };
        g_udp.send(m_leds[i]->getModule()->getIp(), msg, 4);
    }
}

ledManager g_ledManager;
