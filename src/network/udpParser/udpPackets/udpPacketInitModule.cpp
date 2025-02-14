#include "udpPacketInitModule.h"

#include "api/logs/logStream.h"
#include "api/udp/udp.h"
#ifdef __TARGET_MASTER
    #include "session/session.h"
#endif

UdpPacketInitModule::UdpPacketInitModule(Client client, char *data) : UdpPacket(client, PACKET_TYPE_INIT_MODULE)
{
    m_moduleType = (moduleType_t)data[0];
}

UdpPacketInitModule::UdpPacketInitModule(Client client, moduleType_t moduleType) : UdpPacket(client, PACKET_TYPE_INIT_MODULE)
{
    m_moduleType = moduleType;
}

void UdpPacketInitModule::parse()
{
#ifdef __TARGET_MASTER
    // A new module is found, add it to the list of modules
    // Check if the module is already in the list
    if (ModuleManager::getModule(m_client) != NULL)
    {
        // Module already in the list, ignore
        return;
    }
    // Add the module to the list
    Module *l_module = ModuleManager::addModule(m_moduleType, m_client);
    moduleType_t l_moduleType = l_module->getType();
    LogStream::cout << "New module added (type " << l_moduleType
        << " ip " << m_client.getIP().getIpString()
        << " mac " << m_client.getMAC().getMacString() << ")" << LogStream::endl;
#endif
}

void UdpPacketInitModule::send()
{
    char l_packet[] =
    {
        PACKET_TYPE_INIT_MODULE,
        (char)m_moduleType
    };
    udp_send(l_packet, sizeof(l_packet), m_client.getIP().getIpString().c_str(), UDP_DEFAULT_PORT);
}
