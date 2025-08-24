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
     * @brief Create an InterMsgPingSlaves.
     * @note Any client is allowed because the message is broadcasted.
     */
    InterMsgPingSlaves();

    /**
     * @brief Constructor for InterMsg with a callback.
     * @note The others attributes are initialized to default values.
     * @note This constructor is protected to ensure that only derived classes can instantiate it.
     * @note This constructor is used in registered message handlers.
     */
    InterMsgPingSlaves(MessageReceivedCallback callback);

    /**
     * @see InterMsg::serializePriv
     */
    uint32_t serializePriv(char *data) const override;

    /**
     * @see InterMsg::deserializePriv
     */
    uint32_t deserializePriv(const char *data) override;

    /**
     * @brief Get a description of the message.
     */
    std::string toString() const override;
};

#endif // __INTER_MSG_PING_SLAVES_HPP__
