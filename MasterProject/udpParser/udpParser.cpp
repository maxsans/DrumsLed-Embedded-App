#include "udpParser.h"

#include "moduleManager.h"
#include "ledManager.h"
#include "microManager.h"

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
}

udpParser::~udpParser()
{
    // Destructor
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
            if (g_moduleManager.addModule(ip))
            {
                module *newModule = g_moduleManager.getModule(ip);
                if (newModule == NULL)
                {
                    printf("Error while adding new module !\n");
                    return;
                }
                switch(packet[1]) // type of module
                {
                    case TYPE_DRUM_MODULE:
                        g_microManager.addMicro(newModule);
                        g_ledManager.addLed(newModule);
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
            module *l_module = g_moduleManager.getModule(ip);
            if (l_module != NULL)
            {
                micro *l_micro = g_microManager.getMicro(l_module);
                if (l_micro != NULL)
                {
                    l_micro->setMicroValue(packet[1]);
                }
            }
            break;
        }

        default:
            // Unknown packet type, ignore
            break;
    }
}

udpParser g_udpParser;
