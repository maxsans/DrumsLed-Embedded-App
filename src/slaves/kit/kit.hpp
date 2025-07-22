#ifndef __KIT_HPP__
#define __KIT_HPP__

#include "network/client/client.hpp"
#include "api/circleLeds/circleLeds.hpp"
#include "tools/timeTools/periodicCallsMs.hpp"

class Kit
{
private:
    /**
     * @brief Master client.
     *
     */
    static Client m_masterClient;

    static const uint32_t m_adcPeriodMs = 20; // Send ADC data every 20ms

    static periodicCallsMs m_adcPeriodicCall;

    /**
     * @brief Process ADC data.
     *
     * This function is called periodically to process ADC data.
     */
    static void adc_send(void *object);

public:
    Kit();
    ~Kit();

    static void init();

    static void process();

    /**
     * @brief Get the master client.
     *
     * @return Client& Reference to the master client.
     */
    static Client &getMasterClient();

    /**
     * @brief Set the master client.
     *
     * @param client The new master client.
     */
    static void setMasterClient(Client &client);
};

#endif
