#ifndef __KIT_SERVICE_TYPE_HPP__
#define __KIT_SERVICE_TYPE_HPP__

#include <string>

class KitServiceType
{
    public:
        /**
         * @brief Enumeration of the different service types.
         */
        enum Type
        {
            // Here are input services :
            PeriodicLevelAdcSend,

            // Here are output services :
            PeriodicRgbListen,

            // Count of service types
            Count
        };

        private:
            /**
             * @brief The type of the service.
             */
            Type m_type;

            constexpr static const char* m_typeStrings[Type::Count] =
            {
                [Type::PeriodicLevelAdcSend] = "PeriodicLevelAdcSend",
                [Type::PeriodicRgbListen] = "PeriodicRgbListen"
            };

        public:
            /**
             * @brief Constructor for KitServiceType.
             * @param type The type of the service.
             */
            KitServiceType(Type type);

            /**
            * @brief Get the string representation of the service type.
            * @return The string representation.
            */
            std::string toString() const;
};

#endif // __KIT_SERVICE_TYPE_HPP__
