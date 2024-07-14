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
    // Get the udp packet and parse it
    bool l_receivedSomething = false;
    udpPacket *l_udpPacket = g_udp.getPacket(&l_receivedSomething);
    if ( (l_udpPacket != NULL) && (l_receivedSomething) )
    {
        char *l_packet = l_udpPacket->getPacket();
        char *l_ip = l_udpPacket->getIp();
        if (l_packet != NULL && l_ip != NULL)
        {
            parseUdp(l_packet, l_ip);
        }
    }
}

void udpParser::parseUdp(char* packet, char* ip)
{
    // Parse the UDP packet
    uint32_t packetSize = strlen(packet);
    switch (packet[0])
    {
        case PACKET_TYPE_INIT:
        {
            // A new module is found, add it to the list of modules
            if (m_currentSession->getModuleManager()->addModule(ip))
            {
                module *newModule = m_currentSession->getModuleManager()->getModule(ip);
                if (newModule == NULL)
                {
                    printf("Error while adding new module !\n");
                    return;
                }
                switch(packet[1]) // type of module
                {
                    case TYPE_DRUM_MODULE:
                        m_currentSession->getMicroManager()->addMicro(newModule);
                        m_currentSession->getLedManager()->addLed(newModule);
                        printf("New drum module ! ip : %s\n", ip);
                        break;

                    default:
                        // Unknowed module, ignore
                        printf("Unknown module detected ! ip : %s\n", ip);
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
            module *l_module = m_currentSession->getModuleManager()->getModule(ip);
            if (l_module != NULL)
            {
                // Set the micro value of this module
                uint8_t l_microValue = packet[1];
                m_currentSession->getMicroManager()->setMicro(l_module, l_microValue);
            }
            break;
        }

        default:
            // Unknown packet type, ignore
            break;
    }
    // A packet has been received for a module, sync it
    module *l_module = m_currentSession->getModuleManager()->getModule(ip);
    if (l_module != NULL)
    {
        l_module->sync();
    }
}

udpParser g_udpParser;
