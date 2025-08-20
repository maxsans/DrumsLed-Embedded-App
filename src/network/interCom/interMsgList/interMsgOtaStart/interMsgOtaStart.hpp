/**
 * @file interMsgOtaStart.hpp
 * @brief Msg to start an OTA update
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __INTERMSGOTASTART_HPP__
#define __INTERMSGOTASTART_HPP__

#include "network/interCom/interMsg/interMsg.hpp"

/**
 * @brief Msg to start an OTA update
 */
class InterMsgOtaStart : public InterMsg
{
    private:
    static constexpr const SendType m_sendType = SendType::UnicastWithControl;

    /**
     * @brief Private Data of the msg
     */
    struct Data
    {
        /**
         * @brief Length (in bytes) of the program binary
         * @note 0 if the OTA must be skipped
         * (The module is already up to date)
         */
        int64_t m_length;
    } m_data;

    public:
    /**
     * @brief Create an InterMsgOtaStart msg
     * @param client The client that sent the message or the client to which the message is directed.
     * @param length Length (in bytes) of the program binary
     * @note 0 if the OTA must be skipped (the module is already up to date)
     */
    InterMsgOtaStart(Client client, int64_t length);

    /**
     * @brief Create an InterMsgOtaStart msg from raw data
     * @param client The client that sent the message or the client to which the message is directed.
     * @param rawData The raw data to be used for the message.
     * @param size The size of the raw data.
     */
    InterMsgOtaStart(Client client, char *rawData, int64_t size);

    /**
     * @brief Get the length (in bytes) of the program binary
     * @return Length (in bytes) of the program binary
     */
    int64_t getLength() const;

    /**
     * @brief Check if the OTA update is needed.
     * @return true if the OTA update is needed, false otherwise.
     */
    bool isOtaNeeded() const;

    /**
     * @brief Get a description of the message.
     */
    std::string toString() const override;
};

#endif // __INTERMSGOTASTART_HPP__
