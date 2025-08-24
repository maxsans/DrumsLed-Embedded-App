#ifndef __INTER_MSG_HPP__
#define __INTER_MSG_HPP__

#include <cstdint>
#include <string>

#include "network/client/client.hpp"
#include "network/interCom/interMsgData/interMsgData.hpp"

class InterMsg
{
    protected:
    /**
     * @brief Message send types.
     */
    enum class SendType
    {
        None, /** < No send type, used for messages that do not require sending. */
        UnicastWithControl, /** < Unicast with control, used for messages that require acknowledgment or specific handling. */
        UnicastWithoutControl, /** < Unicast without control, used for messages that do not require acknowledgment or specific handling. */
        Broadcast, /** < Broadcast, used for messages that should be sent to all clients without specific handling. */
    };

    private:
    /**
     * @brief The client that sent the message or the client to which the message is directed.
     */
    Client m_client;

    /**
     * @brief The data of the message, including the message header and private data.
     */
    InterMsgData m_data;

    /**
     * @brief The type of send for the message.
     */
    SendType m_sendType;

    /**
     * @brief Get the raw data of the message.
     * @param data Pointer to a buffer where the message data will be stored.
     * @note The buffer should be large enough to hold the message data.
     * @return The size of the data written to the buffer.
     */
    uint32_t getData(char *data) const;

    protected:
    /**
     * @brief Constructor for InterMsg.
     * @note This constructor is protected to ensure that only derived classes can instantiate it.
     */
    InterMsg(Client client,
             InterMsgData data,
             SendType sendType = SendType::UnicastWithControl);

    /**
     * @brief Constructor for InterMsg without a client.
     * @note This constructor is used for messages that do not require a specific client association (broadcast messages).
     * @note Due to the nature of this constructor, SendType is implicitly set to Broadcast.
     * @note This constructor is protected to ensure that only derived classes can instantiate it.
     */
    InterMsg(InterMsgData data);

    public:
    /**
     * @brief Send the message.
     */
    void send();

    /**
     * @brief Get the client associated with the message.
     * @return The client that sent the message or the client to which the message is directed.
     */
    const Client &getClient() const;

    /**
     * @brief Get Id of the message.
     * @return The message ID.
     * @note The message ID is used to identify the type of message being sent.
     * @see InterMsgId
     */
    InterMsgId getId() const;

    /**
     * @brief Equal operator for InterMsg.
     * @param other The other InterMsg to compare with.
     * @return true if the two messages are equal, false otherwise.
     */
    virtual bool operator==(const InterMsg *other) const;

    /**
     * @brief Get a description of the message.
     */
    virtual std::string toString() const;
};

#endif // __INTER_MSG_HPP__
