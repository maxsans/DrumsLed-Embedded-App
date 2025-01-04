#include "macAddr.h"

MacAddr::MacAddr()
{
    m_mac[0] = 0;
    m_mac[1] = 0;
    m_mac[2] = 0;
    m_mac[3] = 0;
    m_mac[4] = 0;
    m_mac[5] = 0;
}

MacAddr::MacAddr(uint8_t mac1, uint8_t mac2, uint8_t mac3, uint8_t mac4, uint8_t mac5, uint8_t mac6)
{
    m_mac[0] = mac1;
    m_mac[1] = mac2;
    m_mac[2] = mac3;
    m_mac[3] = mac4;
    m_mac[4] = mac5;
    m_mac[5] = mac6;
}

MacAddr::MacAddr(const MacAddr &mac)
{
    m_mac[0] = mac.m_mac[0];
    m_mac[1] = mac.m_mac[1];
    m_mac[2] = mac.m_mac[2];
    m_mac[3] = mac.m_mac[3];
    m_mac[4] = mac.m_mac[4];
    m_mac[5] = mac.m_mac[5];
}

MacAddr::MacAddr(const char *mac)
{
    setMac(mac);
}

MacAddr::~MacAddr()
{
}

uint8_t MacAddr::getMac1() const
{
    return m_mac[0];
}

uint8_t MacAddr::getMac2() const
{
    return m_mac[1];
}

uint8_t MacAddr::getMac3() const
{
    return m_mac[2];
}

uint8_t MacAddr::getMac4() const
{
    return m_mac[3];
}

uint8_t MacAddr::getMac5() const
{
    return m_mac[4];
}

uint8_t MacAddr::getMac6() const
{
    return m_mac[5];
}

uint64_t MacAddr::getMac() const
{
    return (uint64_t)m_mac[0] << 40 | (uint64_t)m_mac[1] << 32 | (uint64_t)m_mac[2] << 24 | (uint64_t)m_mac[3] << 16 | (uint64_t)m_mac[4] << 8 | (uint64_t)m_mac[5];
}

std::string MacAddr::getMacString() const
{
    return      std::to_string(m_mac[0])
        + ":" + std::to_string(m_mac[1])
        + ":" + std::to_string(m_mac[2])
        + ":" + std::to_string(m_mac[3])
        + ":" + std::to_string(m_mac[4])
        + ":" + std::to_string(m_mac[5]);
}

void MacAddr::setMac1(uint8_t mac1)
{
    m_mac[0] = mac1;
}

void MacAddr::setMac2(uint8_t mac2)
{
    m_mac[1] = mac2;
}

void MacAddr::setMac3(uint8_t mac3)
{
    m_mac[2] = mac3;
}

void MacAddr::setMac4(uint8_t mac4)
{
    m_mac[3] = mac4;
}

void MacAddr::setMac5(uint8_t mac5)
{
    m_mac[4] = mac5;
}

void MacAddr::setMac6(uint8_t mac6)
{
    m_mac[5] = mac6;
}

void MacAddr::setMac(uint64_t mac)
{
    m_mac[0] = (mac >> 40) & 0xFF;
    m_mac[1] = (mac >> 32) & 0xFF;
    m_mac[2] = (mac >> 24) & 0xFF;
    m_mac[3] = (mac >> 16) & 0xFF;
    m_mac[4] = (mac >> 8) & 0xFF;
    m_mac[5] = mac & 0xFF;
}

void MacAddr::setMac(uint8_t mac1, uint8_t mac2, uint8_t mac3, uint8_t mac4, uint8_t mac5, uint8_t mac6)
{
    m_mac[0] = mac1;
    m_mac[1] = mac2;
    m_mac[2] = mac3;
    m_mac[3] = mac4;
    m_mac[4] = mac5;
    m_mac[5] = mac6;
}

void MacAddr::setMac(const char *mac)
{
    sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &m_mac[0], &m_mac[1], &m_mac[2], &m_mac[3], &m_mac[4], &m_mac[5]);
}

void MacAddr::setMac(const MacAddr &mac)
{
    m_mac[0] = mac.m_mac[0];
    m_mac[1] = mac.m_mac[1];
    m_mac[2] = mac.m_mac[2];
    m_mac[3] = mac.m_mac[3];
    m_mac[4] = mac.m_mac[4];
    m_mac[5] = mac.m_mac[5];
}

void MacAddr::setMac(const std::string &mac)
{
    setMac(mac.c_str());
}

bool MacAddr::operator==(const MacAddr &mac) const
{
    return m_mac[0] == mac.m_mac[0] && m_mac[1] == mac.m_mac[1] && m_mac[2] == mac.m_mac[2] && m_mac[3] == mac.m_mac[3] && m_mac[4] == mac.m_mac[4] && m_mac[5] == mac.m_mac[5];
}

bool MacAddr::operator!=(const MacAddr &mac) const
{
    return m_mac[0] != mac.m_mac[0] || m_mac[1] != mac.m_mac[1] || m_mac[2] != mac.m_mac[2] || m_mac[3] != mac.m_mac[3] || m_mac[4] != mac.m_mac[4] || m_mac[5] != mac.m_mac[5];
}
