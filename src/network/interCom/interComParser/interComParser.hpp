#ifndef __INTER_COM_PARSER_HPP__
#define __INTER_COM_PARSER_HPP__

#include "incompletMsg/incompletMsg.hpp"
#include "network/interCom/interMsg/interMsg.hpp"
#include "tools/os/periodicCallsMs/periodicCallsMs.hpp"
#include <functional>
#include <map>
#include <vector>

class InterComParser
{
    private:
    /**
     * @brief Map of InterMsgId to InterMsg instances to deserialize on message reception.
     */
    static std::map<InterMsgId, InterMsg *> m_callbacks;

    /**
     * @brief Vector of incomplete messages.
     */
    static std::vector<IncompletMsg> m_incompletMsgs;

    /**
     * @brief Periodic task to check incomplete messages.
     */
    PeriodicCallsMs m_periodicTask;
    void periodicTask();

    /**
     * @brief Process incoming data from UDP or TCP.
     */
    void processIncomingData(
        char *data, int16_t len, char *ip, int16_t *port, char *mac);

    /**
     * @brief Process a single incoming message.
     * @param client The client that sent the message.
     * @param msgId The ID of the message.
     * @param data The received message including all headers and private data.
     */
    void processMessage(const Client &client,
                        InterMsgId msgId,
                        const char *data);

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
     * @brief Register an interMsg to deserialize on message reception.
     * @note the message must be allocated on the heap and will be owned by the parser.
     * @note the message must contain a valid callback to call
     */
    static void registerDeserializer(InterMsg *msg);
};

#endif // __INTER_COM_PARSER_HPP__
