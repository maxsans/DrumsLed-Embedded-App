#include "kit/kitService/kitService.hpp"

KitService::KitService(KitServiceType serviceType) : m_serviceType(serviceType)
{
}

Client KitService::getMasterClient() const
{
    return m_masterClient;
}

void KitService::start(Client masterClient)
{
    m_masterClient = masterClient;
    onStart();
}
