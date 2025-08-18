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
        None = 0,

        // Here are default services :

        // Here are input services :
        LevelAdcSend, /** < Service to periodically send ADC level data. */

        // Here are output services :
        RgbListen, /** < Service to periodically listen for RGB data. */

        // Add more service types here as needed

        // Count of service types
        Count
    };

    private:
    /**
     * @brief The type of the service.
     */
    Type m_type;

    public:
    /**
     * @brief Constructor for KitServiceType.
     * @param type The type of the service.
     */
    KitServiceType(Type type = Type::None);

    /**
     * @brief Get the type of the service.
     * @return The type of the service.
     */
    Type getType() const;

    /**
     * @brief Get the string representation of the service type.
     * @return The string representation.
     */
    std::string toString() const;
};

#endif // __KIT_SERVICE_TYPE_HPP__
