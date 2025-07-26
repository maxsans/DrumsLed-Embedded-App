#include "kitConfigGenerator.hpp"
#include "kit/kitService/srvcAliveSend/srvcAliveSend.hpp"
#include "kit/kitService/srvcLevelAdcSend/srvcLevelAdcSend.hpp"
#include "kit/kitService/srvcRgbListen/srvcRgbListen.hpp"

void KitConfigGenerator::init()
{
    m_kitConfig.setType(KitType::Type::Drum);
    m_kitConfig.setAttribute(KitAttributeType::Type::RoundRgb, 200);
    m_kitConfig.setAttribute(KitAttributeType::Type::Rgb, 1);
    m_kitConfig.setAttribute(KitAttributeType::Type::LevelAdc, 1);
}

std::vector<KitService *> KitConfigGenerator::generateKitServices()
{
    std::vector<KitService *> services;
    // Generate the kit services based on the kit configuration
    services.push_back(new KitSrvcAliveSend());
    services.push_back(new KitSrvcLevelAdcSend());
    services.push_back(new KitSrvcRgbListen());
    return services;
}
