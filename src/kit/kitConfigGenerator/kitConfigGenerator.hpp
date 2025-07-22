#ifndef __KIT_CONFIG_GENERATOR_HPP__
#define __KIT_CONFIG_GENERATOR_HPP__

#include "kit/kitConfig/kitConfig.hpp"

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
};

#endif // __KIT_CONFIG_GENERATOR_HPP__
