#ifndef __INTER_MSG_DATA_HPP__
#define __INTER_MSG_DATA_HPP__

#include <cstdint>

#include "network/interCom/interMsgId/interMsgId.hpp"
#include "interMsgHeader/interMsgHeader.hpp"

class InterMsgData
{
    public:
        /**
        * @brief The maximum size of the private data that can be handled by the InterMsgData class.
        */
        static constexpr const uint32_t MAX_PRIV_DATA_SIZE = 1024;

    private:
        InterMsgHeader m_header;

        /**
         * @brief Pointer to the private message data.
         */
        char *m_privData;

    public:
        /**
         * @brief Construct a new Inter Msg Data from fields.
         * @param header The message header containing the message ID and private data size.
         * @param privData Pointer to the private message data.
         * @note The private data size should not exceed MAX_PRIV_DATA_SIZE.
         */
        InterMsgData(InterMsgHeader header, char *privData);

        /**
         * @brief Construct a new Inter Msg Data from raw data.
         * @param rawData Pointer to the raw data buffer including the header and private message data.
         * @param rawSize Size of the raw data buffer.
         * @param privData Pointer to the allocated private message data buffer.
         */
        InterMsgData(char *rawData, uint32_t rawSize, char *privData);

        /**
         * @brief Get the private data of the message.
         * @param data Pointer to a buffer where the message data will be stored.
         * @note The buffer should be large enough to hold the message data.
         * @see MAX_PRIV_DATA_SIZE for the maximum size of the private data.
         * @return The size of the data written to the buffer.
         */
        uint32_t getPrivData(char data[MAX_PRIV_DATA_SIZE]) const;

        /**
         * @brief Get the raw data of the message.
         * @param data Pointer to a buffer where the message data will be stored.
         * @note The buffer should be large enough to hold the message data.
         * @see MAX_PRIV_DATA_SIZE for the maximum size of the data.
         * @return The size of the data written to the buffer.
         */
        uint32_t getData(char data[MAX_PRIV_DATA_SIZE]) const;

        /**
         * @brief Get the header of the message.
         * @return A reference to the InterMsgHeader object containing the message ID and private data size.
         */
        const InterMsgHeader& getHeader() const;

        /**
         * @brief Compare this InterMsgData with another InterMsgData for equality.
         * @param other The other InterMsgData to compare with.
         * @return true if both InterMsgData objects are equal, false otherwise.
         */
        bool operator==(const InterMsgData &other) const;

        /**
         * @brief Compare this InterMsgData with another InterMsgData for inequality.
         * @param other The other InterMsgData to compare with.
         * @return true if both InterMsgData objects are not equal, false otherwise.
         */
        bool operator!=(const InterMsgData &other) const;
};

#endif // __INTER_MSG_DATA_HPP__
