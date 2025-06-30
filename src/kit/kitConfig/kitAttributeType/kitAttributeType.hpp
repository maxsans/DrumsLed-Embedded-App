#ifndef __KIT_ATTRIBUTE_TYPE_HPP__
#define __KIT_ATTRIBUTE_TYPE_HPP__

#include <string>
#include <cstdint>

/**
 * @brief KitAttributeType class to represent different attribute types for a kit.
 */
class KitAttributeType
{
    public:
        /**
         * @brief Enumeration of the different attribute types.
         */
        enum Type
        {
            Rgb,
            LevelAdc,

            Count
        };

        private:
            /**
             * @brief The type of the attribute.
             */
            Type m_type;

            /**
            * @brief The number of attributes of this type the kit has.
            */
            uint32_t m_nbAttributes;

            constexpr static const char* m_typeStrings[Type::Count] =
            {
                [Rgb] = "Rgb",
                [LevelAdc] = "LevelAdc"
            };

        public:
            KitAttributeType(Type type, uint32_t nbAttributes);

            /**
             * @brief Get the type of the attribute.
             * @return The type of the attribute.
             */
            Type getType() const;

            /**
             * @brief Get the number of attributes of this type the kit has.
             * @return The number of attributes.
             */
            uint32_t getNbAttributes() const;

            /**
            * @brief Convert the attribute type to a string representation.
            * @return The string representation of the attribute type.
            */
            std::string toString() const;
};

#endif // __KIT_ATTRIBUTE_TYPE_HPP__
