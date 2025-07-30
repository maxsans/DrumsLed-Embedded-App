#ifndef __INTER_MSG_HEADER_HPP__
#define __INTER_MSG_HEADER_HPP__

#include "network/interCom/interMsgId/interMsgId.hpp"

class InterMsgHeader
{
    public:
    /**
     * @brief The maximum size of the private data that can be handled by the InterMsgData class.
     */
    static constexpr const uint32_t MAX_PRIV_DATA_SIZE = 1024;

    typedef union {
        struct
        {
            /**
             * @brief The message ID.
             */
            InterMsgId::RawType m_id;

            /**
             * @brief The size of the private message data.
             */
            uint32_t m_privSize;
        } m_header;

        char m_rawData[sizeof(m_header)];
    } headerType;

    headerType m_headerUnion;

    public:
    /**
     * @brief Construct a new Inter Msg Header from fields.
     * @param id The message ID.
     * @param privSize The size of the private message data.
     */
    InterMsgHeader(InterMsgId id, uint32_t privSize);

    /**
     * @brief Construct a new Inter Msg Header from raw data.
     * @param rawData Pointer to the raw data buffer.
     */
    InterMsgHeader(const char *rawData);

    /**
     * @brief Get the size of the header.
     * @return The size of the header in bytes.
     * @note The header consists of the message ID and the size of the private message data.
     */
    static constexpr uint32_t getHeaderSize()
    {
        return sizeof(headerType);
    }

    /**
     * @brief Get the message ID.
     * @return The message ID.
     */
    InterMsgId getId() const;

    /**
     * @brief Get the size of the private message data.
     * @return The size of the private message data.
     */
    uint32_t getPrivSize() const;

    /**
     * @brief Compare two InterMsgHeader objects for equality.
     * @param other The other InterMsgHeader object to compare with.
     * @return True if the two headers are equal, false otherwise.
     */
    bool operator==(const InterMsgHeader &other) const;

    /**
     * @brief Compare two InterMsgHeader objects for inequality.
     * @param other The other InterMsgHeader object to compare with.
     * @return True if the two headers are not equal, false otherwise.
     */
    bool operator!=(const InterMsgHeader &other) const;
};

#endif // __INTER_MSG_HEADER_HPP__
