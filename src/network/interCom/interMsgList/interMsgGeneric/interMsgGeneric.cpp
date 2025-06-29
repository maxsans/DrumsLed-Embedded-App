#include "interMsgGeneric.hpp"

#include "tools/logStream/logStream.h"

#include <cassert>

InterMsgGeneric::InterMsgGeneric(Client client, InterMsgData data, SendType sendType)
    : InterMsg(client, data, sendType)
{
    assert(data.getHeader().getPrivSize() <= MAX_PRIV_DATA_SIZE);
    char l_privData[MAX_PRIV_DATA_SIZE];
    size_t l_size = data.getPrivData(l_privData);
    assert(l_size <= MAX_PRIV_DATA_SIZE);
    std::memcpy(m_data.privData, l_privData, l_size);
}

InterMsgGeneric::InterMsgGeneric(Client client, char *rawData, size_t size)
    : InterMsg(client, InterMsgData(rawData, size), InterMsg::SendType::None)
{
    assert(size <= MAX_PRIV_DATA_SIZE);
    std::memcpy(m_data.privData, rawData, size);
}

const char* InterMsgGeneric::getPrivData() const
{
    return m_data.privData;
}
