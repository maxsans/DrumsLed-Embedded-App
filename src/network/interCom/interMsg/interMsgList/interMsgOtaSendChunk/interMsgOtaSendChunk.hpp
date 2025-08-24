/**
 * @file interMsgOtaSendChunk.hpp
 * @brief Message to send a chunk of OTA data
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __INTERMSGOTASENDCHUNK_HPP__
#define __INTERMSGOTASENDCHUNK_HPP__

#include "network/interCom/interMsg/interMsg.hpp"
#include "tools/containers/binary/binary.hpp"

/**
 * @brief Message to send a chunk of OTA data
 */
class InterMsgOtaSendChunk : public InterMsg
{
    public:
    /**
     * @brief Size of the chunk
     */
    static constexpr const uint32_t m_chunkMaxSize = 256;

    private:
    static constexpr const SendType m_sendType = SendType::UnicastWithControl;

    /**
     * @brief Chunk data
     */
    char m_chunkData[m_chunkMaxSize];

    /**
     * @brief Size of the chunk
     * @note Must be less than or equal to m_chunkMaxSize
     */
    uint32_t m_chunkSize;

    public:
    /**
     * @brief Create an InterMsgOtaSendChunk msg
     * @param client The client that sent the message or the client to which the message is directed.
     * @param chunkData The data of the chunk to send.
     * @param chunkSize The size of the chunk to send.
     */
    InterMsgOtaSendChunk(Client client,
                         const char *chunkData,
                         uint32_t chunkSize);

    /**
     * @brief Constructor for InterMsg with a callback.
     * @note The others attributes are initialized to default values.
     * @note This constructor is protected to ensure that only derived classes can instantiate it.
     * @note This constructor is used in registered message handlers.
     */
    InterMsgOtaSendChunk(MessageReceivedCallback callback);

    /**
     * @see InterMsg::serializePriv
     */
    uint32_t serializePriv(char *data) const override;

    /**
     * @see InterMsg::deserializePriv
     */
    uint32_t deserializePriv(const char *data) override;

    /**
     * @brief Get the chunk data
     * @return The chunk data
     */
    const char *getChunkData() const;

    /**
     * @brief Get the chunk size
     * @return The chunk size
     */
    uint32_t getChunkSize() const;

    /**
     * @brief Get a description of the message.
     */
    std::string toString() const override;
};

#endif // __INTERMSGOTASENDCHUNK_HPP__
