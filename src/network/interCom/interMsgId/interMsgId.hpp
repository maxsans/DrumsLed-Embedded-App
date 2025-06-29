#ifndef __INTER_MSG_ID_HPP__
#define __INTER_MSG_ID_HPP__

#include <cstdint>

/**
 * @brief Inter-message ID class.
 */
class InterMsgId
{
    public:
    /**
     * @brief Raw type for the message ID.
     * This is used for serialization and deserialization of the message ID.
     */
    typedef uint32_t RawType;

    /**
     * @brief The message ID enumeration.
     * You can add more message IDs as needed.
     * @note Preferably, add IDs at the end to avoid updating all targets.
     */
    enum MsgId
    {
        None = 0,
        Exemple,
    };

    private:
    MsgId m_id;

    public:
    InterMsgId(MsgId id);
    InterMsgId(RawType rawId);

    /**
     * @return MsgId The message ID.
     */
    MsgId value() const;

    /**
     * @brief Convert the message ID to a raw value.
     * @return RawType The raw value of the message ID.
     */
    RawType rawValue() const;

    bool operator==(const InterMsgId& other) const;
    bool operator!=(const InterMsgId& other) const;
};

#endif // __INTER_MSG_ID_HPP__
