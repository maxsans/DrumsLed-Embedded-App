/**
 * @file fade.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "fade.hpp"
#include "tools/timeTools/timeMs.hpp"

Fade::Fade(Micro *m, RgbLed *rgbLed, RgbColor color, TimeMs duration)
    : Animation(ANIMATION_TYPE_FADE, m, rgbLed),
      m_periodicCall(0, Fade::process, this)
{
    m_color = color;
    m_duration = duration;
    m_hitTime = 0;
}

Fade::~Fade()
{
}

void Fade::start()
{
    m_rgbLed->releaseColor(COLOR_PRIORITY_FADE);
}

void Fade::process(void *object)
{
    Fade *fade = static_cast<Fade *>(object);
    fade->process();
}

void Fade::process()
{
    if (m_micro->isHit())
    {
        m_hitTime.setNow();
    }

    if (m_hitTime != 0)
    {
        TimeMs currentTime = TimeMs::nowMs();
        TimeMs elapsedTime = currentTime - m_hitTime;

        if (elapsedTime <= m_duration)
        {
            float ratio;
            if (elapsedTime == 0)
            {
                ratio = 1;
            }
            else
            {
                ratio = 1 - (float)elapsedTime.get() / (float)m_duration.get();
            }
            RgbColor color = m_color * ratio;
            m_rgbLed->setColor(COLOR_PRIORITY_FADE, color);
        }
        else
        {
            m_rgbLed->releaseColor(COLOR_PRIORITY_FADE);
            m_hitTime = 0;
        }
    }
}

void Fade::stop()
{
    m_rgbLed->releaseColor(COLOR_PRIORITY_FADE);
}
