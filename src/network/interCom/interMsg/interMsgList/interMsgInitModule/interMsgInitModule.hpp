#ifndef __INTER_MSG_INIT_MODULE_HPP__
#define __INTER_MSG_INIT_MODULE_HPP__

#include "kit/kitConfig/kitConfig.hpp"
#include "network/interCom/interMsg/interMsg.hpp"

class InterMsgInitModule : public InterMsg
{
    private:
    static constexpr const SendType m_sendType
        = SendType::UnicastWithoutControl;

    struct Data
    {
        /**
         * @brief Kit configuration for the module.
         */
        KitConfig m_kitConfig;
    } m_data;

    public:
    /**
     * @brief Create an InterMsgInitModule with initialization parameters.
     * @param kitConfig The configuration of the kit to be initialized.
     */
    InterMsgInitModule(Client client, KitConfig kitConfig);

    /**
     * @brief Constructor for InterMsg with a callback.
     * @note The others attributes are initialized to default values.
     * @note This constructor is protected to ensure that only derived classes can instantiate it.
     * @note This constructor is used in registered message handlers.
     */
    InterMsgInitModule(MessageReceivedCallback callback);

    /**
     * @see InterMsg::serializePriv
     */
    uint32_t serializePriv(char *data) const override;

    /**
     * @see InterMsg::deserializePriv
     */
    uint32_t deserializePriv(const char *data) override;

    /**
     * @brief Get the module configuration.
     * @return The kit configuration.
     */
    KitConfig getKitConfig() const;

    /**
     * @brief Get a description of the message.
     */
    std::string toString() const override;
};

#endif // __INTER_MSG_INIT_MODULE_HPP__
