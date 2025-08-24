#ifndef __KIT_CONFIG_GENERATOR_HPP__
#define __KIT_CONFIG_GENERATOR_HPP__

#include "kit/kitConfig/kitConfig.hpp"
#include "kit/kitService/kitService.hpp"
#include <vector>

/**
 * @brief KitConfigGenerator static class to generate kit configurations of the current slave target.
 */
class KitConfigGenerator
{
    private:
    /**
     * @brief The kit configuration of the current slave target.
     */
    static KitConfig m_kitConfig;

    /**
     * @brief Specific initialization for the kit configuration generator for the current slave target.
     */
    static void specificInit();

    public:
    /**
     * @brief Constructor is private to prevent instantiation.
     */
    KitConfigGenerator() = delete;

    /**
     * @brief Initialize the kit configuration generator.
     */
    static void init();

    /**
     * @brief Get the kit configuration of the current slave target.
     * @return The kit configuration of the current slave target.
     */
    static KitConfig getKitConfig();

    /**
     * @brief Generate a list of kit services for the current slave target.
     * @return A vector of pointers to KitService objects.
     * @note The caller is responsible for managing the lifetime of the KitService objects.
     */
    static std::vector<KitService *> generateKitServices();
};

#endif // __KIT_CONFIG_GENERATOR_HPP__
