#ifndef __INTER_MSG_PING_SLAVES_HPP__
#define __INTER_MSG_PING_SLAVES_HPP__

#include "network/interCom/interMsg/interMsg.hpp"

class InterMsgPingSlaves : public InterMsg
{
    private:
        static constexpr const SendType m_sendType = SendType::Broadcast;

        /**
         * @brief This msg has no data
         */
        static constexpr const char *m_data = nullptr;

    public:
        /**
         * @brief Create an InterMsgPingSlaves with timestamp and sequence number.
         * @param client The client sending the message.
         */
        InterMsgPingSlaves(Client client);

        /**
         * @brief Default constructor for InterMsgPingSlaves.
         * This constructor is used when no specific client is associated with the message.
         */
        InterMsgPingSlaves();

        /**
         * @brief Create an InterMsgPingSlaves from raw data.
         * @param rawData The raw data to be used for the message.
         * @param size The size of the raw data.
         */
        InterMsgPingSlaves(Client client, char *rawData, uint32_t size);

        const char* getPrivData() const override;

        /**
         * @brief Get a description of the message.
         */
        std::string toString() const override;
};

#endif // __INTER_MSG_PING_SLAVES_HPP__
