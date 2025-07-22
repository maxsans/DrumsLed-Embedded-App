#include "kitConfigGenerator.hpp"

void KitConfigGenerator::init()
{
    m_kitConfig.setType(KitType::Type::Drum);
    m_kitConfig.setAttribute(KitAttributeType::Type::RoundRgb, 200);
    m_kitConfig.setAttribute(KitAttributeType::Type::LevelAdc, 1);
}
