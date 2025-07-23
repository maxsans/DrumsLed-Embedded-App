#ifndef __KIT_SERCICE_HPP__
#define __KIT_SERCICE_HPP__

#include "kit/kitConfig/kitServiceType/kitServiceType.hpp"
#include "network/client/client.hpp"

/**
 * @brief Base class for all kit services.
 * @warning This class is abstract and should not be instantiated directly.
 */
class KitService
{
    private:
        /**
         * @brief The type of the kit service.
         */
        KitServiceType m_serviceType;

        /**
         * @brief The master client for communication.
         */
        Client m_masterClient;

    protected:
        /**
         * @brief Constructor for KitService.
         * @param serviceType The type of the kit service.
         * @note This constructor is protected to prevent direct instantiation of KitService.
         */
        KitService(KitServiceType serviceType);

        /**
         * @brief Get the master client for communication.
         */
        Client getMasterClient() const;

        /**
         * @brief Called when the kit service is started.
         * @note This method can be overridden by derived classes to perform additional initialization.
         * @note By default, it does nothing.
         */
        virtual void onStart() {};

    public:
        KitService() = delete; // Prevent default constructor

        /**
         * @brief Start the kit service.
         * @param masterClient The master client for communication.
         * @note This method should be called when the communication with the master client is established.
         */
        void start(Client masterClient);
};

#endif // __KIT_SERVICE_HPP__
