#ifndef __INTER_COM_PARSER_HPP__
#define __INTER_COM_PARSER_HPP__

#include "network/interCom/interMsg/interMsg.hpp"
#include "tools/timeTools/periodicCallsMs

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
        using MessageReceivedCallback = std::function<void(const Client &, InterMsg &)>;

        /**
         * @brief Map of callbacks to call if a message is received.
         */
        static std::map<InterMsgId, MessageReceivedCallback> m_callbacks;

        /**
         * @brief Periodic calls handler to process incoming messages.
         * @note This is used to call the periodicCall() method periodically to process incoming messages.
         */
        periodicCallsMs m_periodicCall;

        /**
         * @brief The interval in milliseconds for periodic calls to process incoming messages.
         * @note This value is set to 0 to be called as fast as possible
         */
        static const constexpr uint32_t PERIODIC_CALL_INTERVAL_MS = 0;

        /**
         * @brief Process a single incoming message.
         */
        void processMessage(char msg[MAX_MESSAGE_SIZE], uint32_t msgSize, const char *ipAddress, uint16_t port, const char *macAddress);

        /**
         * @brief Check for incoming messages and process them.
         * @note This function is called periodically to check for incoming messages.
         *       It should be registered with a periodicCallsMs instance.
         * @see InterMsg::getIncomingMessages()
         * @see InterMsg::processCallbacks()
         */
        void checkIncomingMessages();

        /**
         * @brief Process the callbacks for incoming messages.
         * @see InterMsg::processCallbacks()
         */
        void periodicCall();

    public:
        InterComParser();

        /**
         * @brief Internal callback to process the incoming messages.
         * @param object Pointer to the InterComParser instance.
         * @note This function is called periodically to process incoming messages.
         *       It should be registered with a periodicCallsMs instance.
         * @note The object pointer can be casted to InterComParser* in the callback
         *       function to access the instance methods and members.
         */
        static void periodicCallBack(void *object);

        /**
         * @brief Register a callback for a specific message ID.
         * @param msgId The message ID for which the callback should be registered.
         * @param callback The callback function to be called when a message with the specified ID is received.
         * @note This function allows you to register a custom callback for processing messages of a specific type.
         */
        static void registerCallback(InterMsgId msgId, MessageReceivedCallback callback);
};

#endif // __INTER_COM_PARSER_HPP__
