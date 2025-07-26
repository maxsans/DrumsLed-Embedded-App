#include "kit/kitService/kitService.hpp"
#include "tools/logStream/logStream.hpp"

KitService::KitService(KitServiceType serviceType) : m_serviceType(serviceType)
{
}

Client KitService::getMasterClient() const
{
    return m_masterClient;
}

void KitService::start(Client masterClient)
{
    LogStream::cout << "Starting KitService of type: " << m_serviceType.toString() << LogStream::endl;
    m_masterClient = masterClient;
    onStart();
}

void KitService::stop()
{
    LogStream::cout << "Stopping KitService of type: " << m_serviceType.toString() << LogStream::endl;
    onStop();
    m_masterClient = Client(); // Reset master client
}
