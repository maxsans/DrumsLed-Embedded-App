#ifndef __INTER_MSG_INIT_MODULE_HPP__
#define __INTER_MSG_INIT_MODULE_HPP__

#include "kit/kitConfig/kitConfig.hpp"
#include "network/interCom/interMsg/interMsg.hpp"

class InterMsgInitModule : public InterMsg
{
    private:
    static constexpr const SendType m_sendType = SendType::UnicastWithControl;

    struct Data
    {
        /**
         * @brief Kit configuration for the module.
         */
        KitConfig m_kitConfig;
    } m_data;

    /**
     * @see InterMsg::getPrivData
     */
    const char *getPrivData() const override;

    public:
    /**
     * @brief Create an InterMsgInitModule with initialization parameters.
     * @param kitConfig The configuration of the kit to be initialized.
     */
    InterMsgInitModule(Client client, KitConfig kitConfig);

    /**
     * @brief Create an InterMsgInitModule from raw data.
     * @param rawData The raw data to be used for the message.
     * @param size The size of the raw data.
     */
    InterMsgInitModule(Client client, char *rawData, uint32_t size);

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
