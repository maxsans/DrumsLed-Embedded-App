#ifndef __NETWORK_CLIENT_CLIENT_HPP__
#define __NETWORK_CLIENT_CLIENT_HPP__

#include "ipv4.hpp"
#include "macAddr.hpp"

#include <stdint.h>
#include <string>

class Client
{
    private:
        Ipv4 m_ip;
        MacAddr m_mac;

    public:
        Client();
        Client(Ipv4 ip, MacAddr mac);
        Client(const Ipv4& ip, const char* macAddress);
        Client(const Client& other);
        Client& operator=(const Client& other);

        const Ipv4& getIP() const;
        const Ipv4& getIp() const;  // Alternative method name for compatibility
        const MacAddr& getMAC() const;
        std::string getMacAddress() const;  // Alternative method for string MAC

        void setIP(Ipv4 ip);
        void setMAC(MacAddr mac);

        bool operator==(const Client &client) const;
        bool operator!=(const Client &client) const;

        /**
         * @brief Required by std::map
         * @warning Dummy operator, do nothing
         * @note always returns false
         */
        bool operator<(const Client &client) const;
};


#endif
