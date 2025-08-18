#ifndef __INTER_MSG_GENERIC_HPP__
#define __INTER_MSG_GENERIC_HPP__

#include "network/interCom/interMsg/interMsg.hpp"

/**
 * @brief Generic intercommunication message class.
 * @note This class is used when the specific message class is not currently known.
 * @note A pointer to this class can be casted to a specific message class if needed.
 */
class InterMsgGeneric : public InterMsg
{
    private:
    struct Data
    {
        /**
         * @brief Private data of the message.
         */
        char privData[InterMsgHeader::MAX_PRIV_DATA_SIZE];
    } m_data;

    public:
    InterMsgGeneric(Client client, InterMsgData data, SendType sendType);
    InterMsgGeneric(Client client, char *rawData, uint32_t size);

    /**
     * @brief Get the raw specific data of the message.
     * @return Pointer to the private data.
     */
    const char *getPrivData() const override;
};

#endif // __INTER_MSG_GENERIC_HPP__
