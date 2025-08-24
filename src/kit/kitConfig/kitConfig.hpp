#ifndef __KIT_CONFIG_HPP__
#define __KIT_CONFIG_HPP__

#include "kitAttributeType/kitAttributeType.hpp"
#include "kitServiceType/kitServiceType.hpp"
#include "kitType/kitType.hpp"

#include <string>

/**
 * @brief KitConfig class to manage the configuration of a kit.
 * @note This class should be sended with raw data in InterMsgInitModule.
 *       So it should not contain any virtual methods, dynamic memory allocation, pointers, or references.
 */
class KitConfig
{
    private:
    /**
     * @brief The type of the kit, represented by KitType.
     */
    KitType m_type;

    /**
     * @brief The array of attribute types for the kit.
     */
    KitAttributeType m_attributeTypes[KitAttributeType::Count];

    /**
     * @brief The array of service types for the kit.
     */
    KitServiceType m_serviceTypes[KitServiceType::Count];

    public:
    /**
     * @brief Constructor to create a KitConfig with a specific type.
     * @param type The type of the kit.
     */
    KitConfig(KitType::Type type = KitType::Type::None);

    /**
     * @brief Set the kit type.
     * @param type The type of the kit to set.
     */
    void setType(KitType::Type type);

    /**
     * @brief Set an attribute type to the kit configuration.
     * @param attributeType The type of the attribute to add.
     * @param nbAttributes The number of attributes of this type the kit has.
     */
    void setAttribute(KitAttributeType attributeType, uint32_t nbAttributes);

    /**
     * @brief Set a service type to the kit configuration.
     * @param serviceType The type of the service to add.
     */
    void setService(KitServiceType serviceType);

    /**
     * @brief Get the type of the kit.
     * @return The type of the kit.
     */
    KitType getType() const;

    /**
     * @brief Check if the kit has a specific attribute type.
     * @param attributeType The type of the attribute to check.
     * @return true if the kit has the attribute type, false otherwise.
     */
    bool hasAttributeType(KitAttributeType::Type attributeType) const;

    /**
     * @brief Check if the kit has a specific service type.
     * @param serviceType The type of the service to check.
     * @return true if the kit has the service type, false otherwise.
     */
    bool hasServiceType(KitServiceType::Type serviceType) const;

    /**
     * @brief Get a string representation of the kit configuration.
     * @return A string describing the kit configuration.
     */
    std::string toString() const;
};

#endif // __KIT_CONFIG_HPP__
