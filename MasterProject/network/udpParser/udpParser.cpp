#include "udpParser.h"

#include "udp.h"
#include "session.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef enum
{
    TYPE_DRUM_MODULE
} moduleType;

udpParser::udpParser()
{
    // Constructor
    m_currentSession = NULL;
}

udpParser::udpParser(session *currentSession)
{
    // Constructor
    m_currentSession = currentSession;
}

udpParser::~udpParser()
{
    // Destructor
}

void udpParser::setCurrentSession(session *currentSession)
{
    // Set the current session
    m_currentSession = currentSession;
}

void udpParser::parseUdp()
{
    // Parse the UDP packets
    udpPacket *l_packet =  g_udp.getPacket();
    if(l_packet != NULL && l_packet->getLen() > 0)
    {
        parseUdp(l_packet);
    }
}

void udpParser::parseUdp(udpPacket *packet)
{
    char *data = packet->getData();
    Client client = packet->getClient();
    // Parse the UDP packet
    switch (data[0])
    {
        case PACKET_TYPE_INIT:
        {
            // A new module is found, add it to the list of modules
            // Check if the module is already in the list
            if (m_currentSession->getModuleManager()->getModule(client) != NULL)
            {
                // Module already in the list, ignore
                break;
            }
            // Add the module to the list
            if (m_currentSession->getModuleManager()->addModule(new module(client)))
            {
                module *newModule = m_currentSession->getModuleManager()->getModule(client);
                if (newModule == NULL)
                {
                    printf("Error while adding new module !\n");
                    return;
                }
                switch(data[1]) // type of module
                {
                    case TYPE_DRUM_MODULE:
                        m_currentSession->getMicroManager()->addMicro(newModule);
                        m_currentSession->getLedManager()->addLed(newModule);
                        printf("New drum module ! ip : %s, mac : %s\n",
                                    client.getIP().getIpString().c_str(),
                                    client.getMAC().getMacString().c_str());
                        break;

                    default:
                        // Unknowed module, ignore
                        printf("Unknown module detected ! ip : %s, mac : %s\n",
                                    client.getIP().getIpString().c_str(),
                                    client.getMAC().getMacString().c_str());
                        break;
                }
            }
            break;
        }

        case PACKET_TYPE_RGB:
            // Normally impossible on master, ignore
            break;

        case PACKET_TYPE_ADC:
        {
            // New value on the adc of this module, uptate it micro value
            module *l_module = m_currentSession->getModuleManager()->getModule(client);
            if (l_module != NULL)
            {
                // Set the micro value of this module
                uint8_t l_microValue = data[1];
                m_currentSession->getMicroManager()->setMicro(l_module, l_microValue);
            }
            break;
        }

        default:
            // Unknown packet type, ignore
            break;
    }
    // A packet has been received for a module, sync it
    module *l_module = m_currentSession->getModuleManager()->getModule(client);
    if (l_module != NULL)
    {
        l_module->sync();
    }
}

udpParser g_udpParser;
