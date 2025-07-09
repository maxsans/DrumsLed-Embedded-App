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

        const Ipv4& getIP() const;
        const MacAddr& getMAC() const;

        void setIP(Ipv4 ip);
        void setMAC(MacAddr mac);

        bool operator==(const Client &client) const;
        bool operator!=(const Client &client) const;
};


#endif
