#ifndef __INTER_MSG_INIT_MODULE_HPP__
#define __INTER_MSG_INIT_MODULE_HPP__

#include "network/interCom/interMsg/interMsg.hpp"

class InterMsgInitModule : public InterMsg
{
    private:
        static constexpr const SendType m_sendType = SendType::UnicastWithControl;

        struct Data
        {
            uint8_t moduleType; // TODO : use real module type enum
        } m_data;

    public:
        /**
         * @brief Create an InterMsgInitModule with initialization parameters.
         * @param moduleType The type of module being initialized.
         */
        InterMsgInitModule(Client client, uint8_t moduleType);

        /**
         * @brief Create an InterMsgInitModule from raw data.
         * @param rawData The raw data to be used for the message.
         * @param size The size of the raw data.
         */
        InterMsgInitModule(Client client, char *rawData, uint32_t size);

        /**
         * @brief Get the module type.
         * @return The module type.
         */
        uint8_t getModuleType() const;

        /**
         * @brief Get a description of the message.
         */
        std::string toString() const override;
};

#endif // __INTER_MSG_INIT_MODULE_HPP__
