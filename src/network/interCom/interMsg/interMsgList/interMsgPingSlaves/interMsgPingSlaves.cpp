#include "interMsgPingSlaves.hpp"
#include "api/network/networkConversion/networkConversion.hpp"
#include "tools/logStream/logStream.hpp"

InterMsgPingSlaves::InterMsgPingSlaves() : InterMsg(InterMsgId::PingSlaves)
{
}

InterMsgPingSlaves::InterMsgPingSlaves(MessageReceivedCallback callback)
    : InterMsg(callback, InterMsgId::PingSlaves)
{
}

uint32_t InterMsgPingSlaves::serializePriv(char *data) const
{
    // Serialize the ping slaves message (no data to serialize)
    return 0;
}

uint32_t InterMsgPingSlaves::deserializePriv(const char *data)
{
    // Deserialize the ping slaves message (no data to deserialize)
    return 0;
}

std::string InterMsgPingSlaves::toString() const
{
    return "InterMsgPingSlaves: client = " + getClient().getIP().getIpString();
}
