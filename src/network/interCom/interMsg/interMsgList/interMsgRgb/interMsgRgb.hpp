#ifndef __INTER_MSG_RGB_HPP__
#define __INTER_MSG_RGB_HPP__

#include "network/interCom/interMsg/interMsg.hpp"

class InterMsgRgb : public InterMsg
{
    private:
    static constexpr const SendType m_sendType
        = SendType::UnicastWithoutControl;

    /**
     * @brief RGB color data.
     */
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;

    public:
    /**
     * @brief Create an InterMsgRgb with specific RGB values.
     * @param red Red component (0-255).
     * @param green Green component (0-255).
     * @param blue Blue component (0-255).
     */
    InterMsgRgb(Client client, uint8_t red, uint8_t green, uint8_t blue);

    /**
     * @brief Constructor for InterMsg with a callback.
     * @note The others attributes are initialized to default values.
     * @note This constructor is protected to ensure that only derived classes can instantiate it.
     * @note This constructor is used in registered message handlers.
     */
    InterMsgRgb(MessageReceivedCallback callback);

    /**
     * @see InterMsg::serializePriv
     */
    uint32_t serializePriv(char *data) const override;

    /**
     * @see InterMsg::deserializePriv
     */
    uint32_t deserializePriv(const char *data) override;

    /**
     * @brief Get the red component.
     * @return The red value.
     */
    uint8_t getRed() const;

    /**
     * @brief Get the green component.
     * @return The green value.
     */
    uint8_t getGreen() const;

    /**
     * @brief Get the blue component.
     * @return The blue value.
     */
    uint8_t getBlue() const;

    /**
     * @brief Get a description of the message.
     */
    std::string toString() const override;
};

#endif // __INTER_MSG_RGB_HPP__
