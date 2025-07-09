#ifndef __MODULE_HPP__
#define __MODULE_HPP__

#include "network/client/client
#include "tools/timeTools/periodicCallsMs
#include "micro/micro
#include "led/rgbLed

typedef enum
{
    TYPE_DRUM_MODULE,
    TYPE_CYMBAL_MODULE
} moduleType_t;


/**
 * @brief Class to represent a module.
 */
class Module
{
    private:
        moduleType_t m_moduleType;

    protected:
        periodicCallsMs m_checkTimePeriodicCalls;
        Client m_client;
        bool m_connected;
        uint64_t m_lastSyncTime;
        void checkTime();

    public:
        Module(moduleType_t moduleType, Client client);

        /**
         * @brief Process the module.
         * @note This function must be called in the main loop.
         * @note By default, this function will not do anything.
         * Please override it in the child class if needed.
         */
        virtual void process();

        /**
         * @brief Get the micro of the module.
         * @note By default, this function will return nullptr.
         * Please override it in the child class if needed.
         *
         * @return Micro* The micro of the module.
         */
        virtual Micro *getMicro();

        /**
         * @brief Get the rgbLed of the module.
         * @note By default, this function will return nullptr.
         * Please override it in the child class if needed.
         *
         * @return RgbLed* The rgbLed of the module.
         */
        virtual RgbLed *getRgbLed();

        /**
         * @brief Check if the module is connected.
         *
         * @return true if connected
         * @return false if not connected
         */
        bool isConnected();

        /**
         * @brief Get the type of the module.
         *
         * @return moduleType_t The type of the module.
         */
        moduleType_t getType();

        /**
         * @brief Internal callback.
         */
        static void checkTimeCallBack(void *object);

        /**
         * @brief After news over udp, sync the module to push back the timeout.
         */
        void sync();

        /**
         * @brief Set the IP of the module.
         *
         * @param ip The new sIP of the module
         */
        void setIp(Ipv4 ip);

        /**
         * @brief Get the client of the module.
         *
         * @return Client The client of the module.
         */
        Client getClient();
};


#endif
