#ifndef __INTER_MSG_ALIVE_HPP__
#define __INTER_MSG_ALIVE_HPP__

#include "network/interCom/interMsg/interMsg.hpp"

class InterMsgAlive : public InterMsg
{
    private:
        static constexpr const SendType m_sendType = SendType::UnicastWithControl;

        /**
         * @brief This msg has no data
         */
        static constexpr const char *m_data = nullptr;

    public:
        /**
         * @brief Create an InterMsgPingSlaves with timestamp and sequence number.
         * @param client The client sending the message.
         */
        InterMsgAlive(Client client);

        /**
         * @brief Default constructor for InterMsgAlive.
         * This constructor is used when no specific client is associated with the message.
         */
        InterMsgAlive();

        /**
         * @brief Create an InterMsgAlive from raw data.
         * @param rawData The raw data to be used for the message.
         * @param size The size of the raw data.
         */
        InterMsgAlive(Client client, char *rawData, uint32_t size);

        /**
         * @brief Used just to respect the InterMsg interface.
         * @return nullptr since this message has no private data.
         */
        const char* getPrivData() const override;

        /**
         * @brief Get a description of the message.
         */
        std::string toString() const override;
};

#endif // __INTER_MSG_ALIVE_HPP__
