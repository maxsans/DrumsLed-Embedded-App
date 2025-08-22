/**
 * @file incompletMsg.hpp
 * @brief Handles incomplete messages in the InterComParser.
 * @author Cyprien Ménard
 * @date 2025-08-18
 */

#ifndef __INCOMPLETMSG_HPP__
#define __INCOMPLETMSG_HPP__

#include "network/client/client.hpp"
#include <vector>

/**
 * @brief Class to handle incomplete messages in the InterComParser.
 */
class IncompletMsg
{
    private:
    /**
     * @brief Buffer to store incomplete messages.
     */
    std::vector<char> m_buffer;

    /**
     * @brief Source Client from which the message originated.
     */
    Client m_client;

    public:
    /**
     * @brief Constructor to initialize IncompletMsg with a client.
     * @param client The client from which the message originated.
     * @param data Pointer to the data buffer containing the incomplete message.
     * @param len Length of the data buffer.
     */
    IncompletMsg(const Client &client, const char *data, int16_t len);

    /**
     * @brief Get the size of the incomplete message.
     * @return Size of the incomplete message.
     */
    size_t getSize() const;

    /**
     * @brief Get the data of the incomplete message.
     * @return Pointer to the data buffer of the incomplete message.
     */
    const char *getData() const;

    /**
     * @brief Get the client associated with this incomplete message.
     * @return The Client object associated with this incomplete message.
     */
    const Client &getClient() const;

    /**
     * @brief Concatenate new data to the existing incomplete message.
     * @param data Pointer to the new data buffer.
     * @param len Length of the new data buffer.
     */
    void appendData(const char *data, size_t len);

    /**
     * @brief Check if the message is complete.
     * @return True if the message is complete, false otherwise.
     */
    bool isComplete() const;

    /**
     * @brief Replace the buffer data with new data.
     * @param data Pointer to the new data buffer.
     * @param len Length of the new data buffer.
     */
    void replaceData(const char *data, size_t len);
};

#endif // __INCOMPLETMSG_HPP__
