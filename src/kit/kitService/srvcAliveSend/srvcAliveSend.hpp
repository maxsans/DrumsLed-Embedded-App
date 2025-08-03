#ifndef __SRVC_ALIVE_SEND_HPP__
#define __SRVC_ALIVE_SEND_HPP__

#include "kit/kitService/kitService.hpp"
#include "tools/timeTools/periodicCallsMs.hpp"

class KitSrvcAliveSend : public KitService
{
    private:
    /**
     * @brief The interval in milliseconds for sending alive messages.
     */
    static const timeMs m_sendInterval;

    /**
     * @brief The periodic call for sending alive messages.
     */
    periodicCallsMs m_sendPeriodicCall;

    /**
     * @brief Callback function for periodic sending of ADC level data.
     */
    static void periodicSendCallback(void *object);
    void periodicSendCallback();

    /**
     * @see KitService::onStart
     */
    void onStart() override;

    /**
     * @see KitService::onStop
     */
    void onStop() override;

    public:
    /**
     * @brief Constructor for KitSrvcAliveSend.
     */
    KitSrvcAliveSend();
};

#endif // __SRVC_ALIVE_SEND_HPP__
