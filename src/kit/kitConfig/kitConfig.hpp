#ifndef __KIT_CONFIG_HPP__
#define __KIT_CONFIG_HPP__

#include "kitAttributeType/kitAttributeType.hpp"
#include "kitServiceType/kitServiceType.hpp"
#include "kitType/kitType.hpp"

#include <cstring>
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
     * @brief Maximum length for the program path string.
     */
    static constexpr size_t MAX_PROGRAM_PATH_LENGTH = 256;

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

    /**
     * @brief CRC32 checksum of the program.
     * @note This is used to check if an update is needed.
     */
    uint32_t m_programCrc32;

    /**
     * @brief The path to the program file.
     * @note This is used to locate the program binary on the filesystem.
     *       Using fixed-size array to avoid dynamic allocation.
     */
    char m_programPath[MAX_PROGRAM_PATH_LENGTH];

    public:
    /**
     * @brief Constructor to create a KitConfig with a specific type.
     * @param type The type of the kit.
     */
    KitConfig(KitType::Type type = KitType::Type::None);

    /**
     * @brief Constructor to create a KitConfig from serialized data.
     * @param rawData The serialized data to create the KitConfig from.
     * @note The rawData must be at least getSerializedSize() bytes long.
     */
    KitConfig(const char *rawData);

    /**
     * @brief Get the serialized size of the kit configuration.
     * @return The serialized size in bytes.
     */
    uint32_t getSerializedSize() const;

    /**
     * @brief Serialize the kit configuration to a byte array.
     * @param buffer The buffer to serialize to.
     * @note The buffer must be at least getSerializedSize() bytes long.
     */
    void serialize(char *buffer) const;

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
     * @brief Set the program CRC32 checksum.
     * @param crc32 The CRC32 checksum of the program.
     */
    void setProgramCrc32(uint32_t crc32);

    /**
     * @brief Set the path to the program file.
     * @param path The path to the program file.
     */
    void setProgramPath(const char *path);

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
     * @brief Get the program CRC32 checksum.
     * @return The CRC32 checksum of the program.
     */
    uint32_t getProgramCrc32() const;

    /**
     * @brief Get the path to the program file.
     * @return The path to the program file.
     */
    const char *getProgramPath() const;

    /**
     * @brief Get a string representation of the kit configuration.
     * @return A string describing the kit configuration.
     */
    std::string toString() const;
};

#endif // __KIT_CONFIG_HPP__
