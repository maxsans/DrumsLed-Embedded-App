#include "interMsgId.hpp"

InterMsgId::InterMsgId(MsgId id)
    : m_id(id)
{
}

InterMsgId::InterMsgId(RawType rawId)
    : m_id(static_cast<MsgId>(rawId))
{
}

InterMsgId::RawType InterMsgId::rawValue() const
{
    return static_cast<RawType>(m_id);
}

InterMsgId::MsgId InterMsgId::value() const
{
    return m_id;
}

bool InterMsgId::operator==(const InterMsgId& other) const
{
    return m_id == other.m_id;
}

bool InterMsgId::operator!=(const InterMsgId& other) const
{
    return m_id != other.m_id;
}

bool InterMsgId::operator<(const InterMsgId& other) const
{
    return m_id < other.m_id;
}
