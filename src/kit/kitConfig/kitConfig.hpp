#ifndef __KIT_CONFIG_HPP__
#define __KIT_CONFIG_HPP__

#include "kitType/kitType.hpp"

/**
 * @brief KitConfig class to manage the configuration of a kit.
 */
class KitConfig
{
    private:
        /**
         * @brief The type of the kit, represented by KitType.
         */
        KitType m_type;

    public:
        /**
         * @brief Constructor to create a KitConfig with a specific type.
         * @param type The type of the kit.
         */
        KitConfig(KitType::Type type);
};

#endif // __KIT_CONFIG_HPP__
