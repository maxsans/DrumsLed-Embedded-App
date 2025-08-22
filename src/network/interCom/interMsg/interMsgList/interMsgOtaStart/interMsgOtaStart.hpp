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
     * @brief Length (in bytes) of the program binary
     * @note 0 if the OTA must be skipped
     * (The module is already up to date)
     */
    int64_t m_length;

    public:
    /**
     * @brief Create an InterMsgOtaStart msg
     * @param client The client that sent the message or the client to which the message is directed.
     * @param length Length (in bytes) of the program binary
     * @note 0 if the OTA must be skipped (the module is already up to date)
     */
    InterMsgOtaStart(Client client, int64_t length);

    /**
     * @brief Constructor for InterMsg with a callback.
     * @note The others attributes are initialized to default values.
     * @note This constructor is protected to ensure that only derived classes can instantiate it.
     * @note This constructor is used in registered message handlers.
     */
    InterMsgOtaStart(MessageReceivedCallback callback);

    /**
     * @see InterMsg::serializePriv
     */
    uint32_t serializePriv(char *data) const override;

    /**
     * @see InterMsg::deserializePriv
     */
    uint32_t deserializePriv(const char *data) override;

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
