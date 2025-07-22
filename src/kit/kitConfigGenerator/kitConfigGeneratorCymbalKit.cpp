#include "kitConfigGenerator.hpp"

void KitConfigGenerator::init()
{
    m_kitConfig.setType(KitType::Type::Cymbal);
    m_kitConfig.setAttribute(KitAttributeType::Type::RoundRgb, 16);
    m_kitConfig.setAttribute(KitAttributeType::Type::LevelAdc, 1);
}
