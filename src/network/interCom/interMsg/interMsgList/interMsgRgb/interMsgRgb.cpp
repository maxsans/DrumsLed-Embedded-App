#include "interMsgRgb.hpp"
#include "api/network/networkConversion/networkConversion.hpp"
#include "tools/logStream/logStream.hpp"

InterMsgRgb::InterMsgRgb(Client client,
                         uint8_t red,
                         uint8_t green,
                         uint8_t blue)
    : InterMsg(client, InterMsgId::Rgb, m_sendType)
{
    m_red = red;
    m_green = green;
    m_blue = blue;
}

InterMsgRgb::InterMsgRgb(MessageReceivedCallback callback)
    : InterMsg(callback, InterMsgId::Rgb)
{
}

uint32_t InterMsgRgb::serializePriv(char *data) const
{
    // Serialize the RGB data using network byte order
    uint32_t redNetwork = network_htonl(m_red);
    uint32_t greenNetwork = network_htonl(m_green);
    uint32_t blueNetwork = network_htonl(m_blue);
    std::memcpy(data, &redNetwork, sizeof(redNetwork));
    std::memcpy(data + sizeof(redNetwork), &greenNetwork, sizeof(greenNetwork));
    std::memcpy(data + sizeof(redNetwork) + sizeof(greenNetwork),
                &blueNetwork,
                sizeof(blueNetwork));
    return sizeof(redNetwork) + sizeof(greenNetwork) + sizeof(blueNetwork);
}

uint32_t InterMsgRgb::deserializePriv(const char *data)
{
    // Deserialize the RGB data using network byte order
    uint32_t redNetwork;
    uint32_t greenNetwork;
    uint32_t blueNetwork;
    std::memcpy(&redNetwork, data, sizeof(redNetwork));
    std::memcpy(&greenNetwork, data + sizeof(redNetwork), sizeof(greenNetwork));
    std::memcpy(&blueNetwork,
                data + sizeof(redNetwork) + sizeof(greenNetwork),
                sizeof(blueNetwork));
    m_red = network_ntohl(redNetwork);
    m_green = network_ntohl(greenNetwork);
    m_blue = network_ntohl(blueNetwork);
    return sizeof(redNetwork) + sizeof(greenNetwork) + sizeof(blueNetwork);
}

uint8_t InterMsgRgb::getRed() const
{
    return m_red;
}

uint8_t InterMsgRgb::getGreen() const
{
    return m_green;
}

uint8_t InterMsgRgb::getBlue() const
{
    return m_blue;
}

std::string InterMsgRgb::toString() const
{
    return "InterMsgRgb: RGB(" + std::to_string(m_red) + ", "
           + std::to_string(m_green) + ", " + std::to_string(m_blue)
           + ", client = " + getClient().getIP().getIpString();
}
