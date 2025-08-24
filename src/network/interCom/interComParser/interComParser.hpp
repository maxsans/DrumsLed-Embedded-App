#ifndef __INTER_COM_PARSER_HPP__
#define __INTER_COM_PARSER_HPP__

#include "network/interCom/interMsg/interMsg.hpp"

#include <functional>
#include <map>

class InterComParser
{
    private:
    /**
     * @brief Maximum size of a message that can be received.
     * @note This is the maximum size of the message buffer used to receive messages.
     */
    static const constexpr uint32_t MAX_MESSAGE_SIZE = 1024;

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
     * @brief Process a single incoming message.
     */
    void processMessage(const char *data,
                        int16_t len,
                        const char *ip,
                        int16_t port,
                        const char *mac);

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
