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
     * @brief Map of InterMsgId to InterMsg instances to deserialize on message reception.
     */
    static std::map<InterMsgId, InterMsg *> m_callbacks;

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
    void processMessage(const Client &client,
                        InterMsgId msgId,
                        const char *privData,
                        uint32_t privDataSize);

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
