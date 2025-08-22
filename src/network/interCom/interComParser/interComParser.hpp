#ifndef __INTER_COM_PARSER_HPP__
#define __INTER_COM_PARSER_HPP__

#include "incompletMsg/incompletMsg.hpp"
#include "network/interCom/interMsg/interMsg.hpp"
#include <functional>
#include <map>
#include <vector>

class InterComParser
{
    private:
    /**
     * @brief Maximum size of a message that can be received.
     * @note This is the maximum size of the message buffer used to receive messages.
     */
    static const constexpr uint32_t MAX_MESSAGE_SIZE = 2048;

    /**
     * @brief Type of callback to call if a message is received.
     * @note This callback is used to process the incoming messages.
     */
    using MessageReceivedCallback
        = std::function<void(const Client &, InterMsg &, void *)>;

    /**
     * @brief Structure to hold callback information including object pointer
     */
    struct CallbackInfo
    {
        MessageReceivedCallback callback;
        void *object;

        CallbackInfo() : object(nullptr)
        {
        }
        CallbackInfo(MessageReceivedCallback cb, void *obj = nullptr)
            : callback(cb), object(obj)
        {
        }
    };

    /**
     * @brief Map of callbacks to call if a message is received.
     */
    static std::map<InterMsgId, CallbackInfo> m_callbacks;

    /**
     * @brief Map of client-specific callbacks using string keys (clientKey + msgId)
     */
    static std::map<std::pair<Client, InterMsgId>, CallbackInfo>
        m_clientCallbacks;

    /**
     * @brief Vector of incomplete messages.
     */
    static std::vector<IncompletMsg> m_incompletMsgs;

    /**
     * @brief Process incoming data from UDP or TCP.
     * @param data Pointer to the received data.
     * @param len Length of the received data.
     * @param ip IP address of the sender.
     * @param port Port number of the sender.
     * @param mac MAC address of the sender.
     * @note This method is called when new data is received from UDP or TCP.
     */
    void processIncomingData(const char *data,
                             int16_t len,
                             const char *ip,
                             int16_t port,
                             const char *mac);

    /**
     * @brief Process a single incoming message.
     */
    void processMessage(const char *data, uint32_t len, const Client &client);

    /**
     * @brief Process all complete messages from a buffer, handling concatenated messages.
     * @param incompleteMsg Pointer to the incomplete message buffer.
     * @param incompletMsgIndex Index of the incomplete message in the vector.
     */
    void processCompleteMessages(IncompletMsg *incompleteMsg,
                                 uint32_t incompletMsgIndex);

    public:
    InterComParser();

    /**
     * @brief Register a callback for a specific message ID.
     * @param msgId The message ID for which the callback should be registered.
     * @param callback The callback function to be called when a message with the specified ID is received.
     * @note This function allows you to register a custom callback for processing messages of a specific type.
     */
    static void registerCallback(InterMsgId msgId,
                                 MessageReceivedCallback callback);

    /**
     * @brief Register a callback for a specific message ID with an object pointer.
     * @param msgId The message ID for which the callback should be registered.
     * @param callback The callback function to be called when a message with the specified ID is received.
     * @param object Pointer to the object to be passed to the callback function.
     * @note This function allows you to register a custom callback for processing messages of a specific type.
     */
    static void registerCallback(InterMsgId msgId,
                                 MessageReceivedCallback callback,
                                 void *object);

    /**
     * @brief Register a callback for a specific message ID and a specific client.
     * @param client The client for which the callback should be registered.
     * @param msgId The message ID for which the callback should be registered.
     * @param callback The callback function to be called when a message with the specified ID is received.
     * @note This function allows you to register a custom callback for processing messages of a specific type.
     * @note The callback will be called uniquely if the message is received from the specified client.
     */
    static void registerCallback(const Client &client,
                                 InterMsgId msgId,
                                 MessageReceivedCallback callback);

    /**
     * @brief Register a callback for a specific message ID and a specific client with an object pointer.
     * @param client The client for which the callback should be registered.
     * @param msgId The message ID for which the callback should be registered.
     * @param callback The callback function to be called when a message with the specified ID is received.
     * @param object Pointer to the object to be passed to the callback function.
     * @note This function allows you to register a custom callback for processing messages of a specific type.
     * @note The callback will be called uniquely if the message is received from the specified client.
     */
    static void registerCallback(const Client &client,
                                 InterMsgId msgId,
                                 MessageReceivedCallback callback,
                                 void *object);

    /**
     * @brief Register a static callback for a specific message ID.
     * @param msgId The message ID for which the callback should be registered.
     * @param callback The static callback function to be called when a message with the specified ID is received.
     * @note This overload allows registering a static function pointer as callback.
     */
    static void registerCallback(InterMsgId msgId,
                                 void (*callback)(const Client &, InterMsg &));
};

#endif // __INTER_COM_PARSER_HPP__
