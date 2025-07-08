#ifndef __KIT_CONFIG_MANAGER_HPP__
#define __KIT_CONFIG_MANAGER_HPP__

#include "kit/kitConfig/kitConfig.hpp"

/**
 * @brief KitConfigManager static class to manage kit configurations of the current slave target.
 */
class KitConfigManager
{
    private:
        /**
         * @brief The kit configuration of the current slave target.
         */
        static KitConfig m_kitConfig;

    public:
        /**
         * @brief Initialize the kit configuration manager.
         */
        static void init();

        /**
         * @brief Get the kit configuration of the current slave target.
         * @return The kit configuration of the current slave target.
         */
        static KitConfig getKitConfig();
};

#endif // __KIT_CONFIG_MANAGER_HPP__
