#ifndef __NETWORK_CLIENT_CLIENT_H__
#define __NETWORK_CLIENT_CLIENT_H__

#include "ipv4.h"
#include "macAddr.h"

#include <stdint.h>

class Client
{
    private:
        Ipv4 m_ip;
        MacAddr m_mac;

    public:
        Client();
        Client(Ipv4 ip, MacAddr mac);
        Client(Client &client);
        ~Client();

        Ipv4 getIP();
        MacAddr getMAC();

        void setIP(Ipv4 ip);
        void setMAC(MacAddr mac);

        void send (char *data, uint32_t size);

        bool operator==(Client &client);
        bool operator!=(Client &client);
};


#endif
