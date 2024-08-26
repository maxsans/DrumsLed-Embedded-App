#include "fade.h"

#include <windows.h>

fade::fade(micro *m, rgbLed *rgbLed, rgbColor color, uint32_t duration) : animation(ANIMATION_TYPE_FADE, m, rgbLed)
{
    m_color = color;
    m_duration = duration;
    m_hitTime = 0;
}

fade::~fade()
{
}

void fade::start()
{
    m_rgbLed->releaseColor(COLOR_PRIORITY_FADE);
}

void fade::process()
{
    if (m_micro->isHit())
    {
        m_hitTime = GetTickCount64();
    }

    if (m_hitTime != 0)
    {
        uint64_t currentTime = GetTickCount64();
        uint64_t elapsedTime = currentTime - m_hitTime;

        if (elapsedTime < m_duration)
        {
            float ratio = (float)elapsedTime / m_duration;
            rgbColor color = m_color * ratio;
            m_rgbLed->setColor(COLOR_PRIORITY_FADE, color);
        }
        else
        {
            m_rgbLed->releaseColor(COLOR_PRIORITY_FADE);
            m_hitTime = 0;
        }
    }
}

void fade::stop()
{
    m_rgbLed->releaseColor(COLOR_PRIORITY_FADE);
}
