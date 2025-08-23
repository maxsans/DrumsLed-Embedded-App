#ifndef __INTER_MSG_HPP__
#define __INTER_MSG_HPP__

#include "interMsgId/interMsgId.hpp"
#include "network/client/client.hpp"
#include <cstdint>
#include <functional>
#include <string>

class InterMsg
{
    public:
    /**
     * @brief Maximum size of private data in the message.
     */
    static constexpr uint32_t m_maxPrivDataSize = 1024;

    /**
     * @brief Emplacement of message IDs from start of message
     */
    static constexpr uint32_t m_idOffset = 0;

    /**
     * @brief Size of message IDs in the message.
     */
    static constexpr uint32_t m_idSize = sizeof(uint32_t);

    /**
     * @brief Emplacement of message lengths from start of message
     */
    static constexpr uint32_t m_lenOffset = m_idOffset + m_idSize;

    /**
     * @brief Size of message lengths in the message.
     */
    static constexpr uint32_t m_lenSize = sizeof(uint32_t);

    /**
     * @brief Emplacement of private data from start of message
     */
    static constexpr uint32_t m_privDataOffset = m_lenOffset + m_lenSize;

    /**
     * @brief Type of callback to call if a message is received.
     * @note This callback is used to process the incoming messages.
     */
    using MessageReceivedCallback = std::function<void(Client, InterMsg &)>;

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
     * @brief The ID of the message.
     */
    InterMsgId m_id;

    /**
     * @brief The type of send for the message.
     */
    SendType m_sendType;

    /**
     * @brief Callback to call if a message is received.
     * @note This callback is used to process the incoming messages.
     */
    MessageReceivedCallback m_callback;

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
             InterMsgId id,
             SendType sendType = SendType::UnicastWithControl);

    /**
     * @brief Constructor for InterMsg without a client.
     * @note This constructor is used for messages that do not require a specific client association (broadcast messages).
     * @note Due to the nature of this constructor, SendType is implicitly set to Broadcast.
     * @note This constructor is protected to ensure that only derived classes can instantiate it.
     */
    InterMsg(InterMsgId id);

    /**
     * @brief Constructor for InterMsg with a callback.
     * @note The others attributes are initialized to default values.
     * @note This constructor is protected to ensure that only derived classes can instantiate it.
     * @note This constructor is used in registered message handlers.
     */
    InterMsg(MessageReceivedCallback callback, InterMsgId id);

    /**
     * @brief Virtual method to serialize the private data of the message.
     * @param data Pointer to a buffer where the serialized data will be stored.
     * @note The buffer should be large enough to hold the serialized data.
     * @return The size of the serialized data.
     */
    virtual uint32_t serializePriv(char *data) const = 0;

    /**
     * @brief Virtual method to deserialize the private data of the message.
     * @param data Pointer to a buffer containing the serialized private data.
     * @note The buffer should be large enough to hold the serialized data.
     * @return The size of the deserialized data.
     */
    virtual uint32_t deserializePriv(const char *data) = 0;

    public:
    /**
     * @brief Send the message.
     */
    void send();

    /**
     * @brief Handle the reception of a message.
     * @param client The client that sent the message.
     * @param msg The received message including all headers and private data.
     * @return The size used by the message in the buffer.
     * @return 0 if the message is incomplete.
     */
    uint32_t onReception(const Client &client, const char *msg);

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
     * @brief Get a description of the message.
     */
    virtual std::string toString() const;
};

#endif // __INTER_MSG_HPP__
