/**
 * @file interMsgExample.hpp
 * @brief Example inter-message structure for communication
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __INTERMSGEXAMPLE_HPP__
#define __INTERMSGEXAMPLE_HPP__

#include "network/interCom/interMsg/interMsg.hpp"
#include <cstdint>

/**
 * @brief Example inter-message structure for communication
 */
class InterMsgExample : public InterMsg
{
    private:
    static constexpr const SendType m_sendType = SendType::UnicastWithControl;

    /**
     * @brief Example data for the message.
     */
    uint32_t m_exampleData;

    /**
     * @brief Example string for the message.
     */
    char m_exampleString[256];

    public:
    /**
     * @brief Create an InterMsgExample with a specific example data.
     * @param exampleData The example data to be included in the message.
     */
    InterMsgExample(Client client, int exampleData);

    /**
     * @brief Constructor for InterMsg with a callback.
     * @note The others attributes are initialized to default values.
     * @note This constructor is protected to ensure that only derived classes can instantiate it.
     * @note This constructor is used in registered message handlers.
     */
    InterMsgExample(MessageReceivedCallback callback);

    /**
     * @see InterMsg::serializePriv
     */
    uint32_t serializePriv(char *data) const override;

    /**
     * @see InterMsg::deserializePriv
     */
    uint32_t deserializePriv(const char *data) override;

    /**
     * @brief Get the example data from the message.
     * @return The example data.
     */
    int getExampleData() const;

    /**
     * @brief Get a description of the message.
     */
    std::string toString() const override;
};

#endif // __INTERMSGEXAMPLE_HPP__
