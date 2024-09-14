#ifndef __NETWORK_CLIENT_MACADDR_H__
#define __NETWORK_CLIENT_MACADDR_H__

#include <stdint.h>
#include <stdbool.h>
#include <string>

class MacAddr
{
    private:
        uint8_t m_mac[6];

    public:
        MacAddr();
        MacAddr(uint8_t mac1, uint8_t mac2, uint8_t mac3, uint8_t mac4, uint8_t mac5, uint8_t mac6);
        MacAddr(const MacAddr &mac);
        MacAddr(const char *mac);
        ~MacAddr();

        uint8_t getMac1() const;
        uint8_t getMac2() const;
        uint8_t getMac3() const;
        uint8_t getMac4() const;
        uint8_t getMac5() const;
        uint8_t getMac6() const;
        uint64_t getMac() const;
        std::string getMacString() const;

        void setMac1(uint8_t mac1);
        void setMac2(uint8_t mac2);
        void setMac3(uint8_t mac3);
        void setMac4(uint8_t mac4);
        void setMac5(uint8_t mac5);
        void setMac6(uint8_t mac6);
        void setMac(uint64_t mac);
        void setMac(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint8_t ip5, uint8_t ip6);
        void setMac(const char *mac);
        void setMac(const MacAddr &mac);
        void setMac(const std::string &mac);

        bool operator==(const MacAddr &mac) const;
        bool operator!=(const MacAddr &mac) const;
};

#endif
