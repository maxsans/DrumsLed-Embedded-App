#ifndef __INTER_MSG_RGB_HPP__
#define __INTER_MSG_RGB_HPP__

#include "network/interCom/interMsg/interMsg.hpp"
#include <functional>

class InterMsgRgb : public InterMsg
{
    private:
        static constexpr const SendType m_sendType = SendType::UnicastWithoutControl;

        struct Data
        {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
        } m_data;

    public:
        /**
         * @brief Create an InterMsgRgb with specific RGB values.
         * @param red Red component (0-255).
         * @param green Green component (0-255).
         * @param blue Blue component (0-255).
         */
        InterMsgRgb(Client client, uint8_t red, uint8_t green, uint8_t blue);

        /**
         * @brief Create an InterMsgRgb from raw data.
         * @param rawData The raw data to be used for the message.
         * @param size The size of the raw data.
         */
        InterMsgRgb(Client client, char *rawData, uint32_t size);

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
