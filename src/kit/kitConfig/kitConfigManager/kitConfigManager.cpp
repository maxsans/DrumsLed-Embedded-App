#include "kitConfigManager.hpp"

KitConfig KitConfigManager::m_kitConfig();

void KitConfigManager::init()
{
#ifdef __TARGET_DRUMKIT
    m_kitConfig.setType(KitType::Type::DrumsLed);
    m_kitConfig.setAttribute(KitAttributeType::Type::RoundRgb, 200);
    m_kitConfig.setAttribute(KitAttributeType::Type::LevelAdc, 1);
#elif defined(__TARGET_CYMBALKIT)
    m_kitConfig.setType(KitType::Type::Cymbal);
    m_kitConfig.setAttribute(KitAttributeType::Type::RoundRgb, 16);
    m_kitConfig.setAttribute(KitAttributeType::Type::LevelAdc, 1);
#endif
}

KitConfig KitConfigManager::getKitConfig()
{
    return m_kitConfig;
}
