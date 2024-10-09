#ifndef __NETWORK_CLIENT_IPV4_H__
#define __NETWORK_CLIENT_IPV4_H__

#include <stdint.h>
#include <stdbool.h>
#include <string>


class IPv4
{
    private:
        uint8_t m_ip[4];

    public:
        IPv4();
        IPv4(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4);
        IPv4(uint32_t ip);
        IPv4(IPv4 &ip);
        IPv4(char *ip);
        ~IPv4();

        uint8_t getIp1();
        uint8_t getIp2();
        uint8_t getIp3();
        uint8_t getIp4();
        uint32_t getIp();
        std::string getIpString();

        void setIp1(uint8_t ip1);
        void setIp2(uint8_t ip2);
        void setIp3(uint8_t ip3);
        void setIp4(uint8_t ip4);
        void setIp(uint32_t ip);
        void setIp(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4);
        void setIp(char *ip);
        void setIp(IPv4 &ip);
        void setIp(std::string &ip);

        bool operator==(const IPv4 &ip);
        bool operator!=(const IPv4 &ip);
};

#endif
