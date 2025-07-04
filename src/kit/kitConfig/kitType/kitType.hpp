#ifndef __KIT_TYPE_HPP__
#define __KIT_TYPE_HPP__

#include <string>

class KitType
{
    public:
        enum Type
        {
            None = 0,

            Drum,
            Cymbal,
            // Add new kit types here
            // Don't forget to update KitType::typeStrings

            /**
             * @brief Total number of kit types.
             * This should always be the last entry in the enum.
             */
            Count
        };

    private:
        /**
         * @brief The type of the kit, represented by KitType::Type.
         */
        Type m_type;

        /**
         * @brief Array of string representations for each KitType::Type.
         * The strings are indexed by the KitType::Type enum values.
         */
        constexpr static const char* typeStrings[Type::Count] =
        {
            "Drum",
            "Cymbal",
        };

    public:
        KitType(Type type = Type::None);

        /**
         * @brief Convert KitType::Type to string representation.
         * @return The string representation of the type.
         */
        std::string toString() const;
};

#endif // __KIT_TYPE_HPP__
