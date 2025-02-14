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
    if (g_session.getModuleManager()->getModule(m_client) != NULL)
    {
        // Module already in the list, ignore
        return;
    }
    // Add the module to the list
    if (g_session.getModuleManager()->addModule(new Module(m_client)))
    {
        Module *newModule = g_session.getModuleManager()->getModule(m_client);
        if (newModule == NULL)
        {
            printf("Error while adding new module !\n");
            return;
        }
        switch(m_moduleType) // type of module
        {
            case TYPE_DRUM_MODULE:
                g_session.getMicroManager()->addMicro(newModule);
                g_session.getLedManager()->addLed(newModule);
                LogStream() << "New drum module ! ip : "
                    << m_client.getIP().getIpString()
                    << ", mac : " << m_client.getMAC().getMacString()
                    << LogStream::endl;
                break;

            default:
                // Unknowed module, ignore
                LogStream() << "Unknown module detected ! ip : "
                    << m_client.getIP().getIpString()
                    << ", mac : " << m_client.getMAC().getMacString()
                    << LogStream::endl;
                break;
        }
    }
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
