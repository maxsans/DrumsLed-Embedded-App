#ifndef __SRVC_RGB_LISTEN_HPP__
#define __SRVC_RGB_LISTEN_HPP__

#include "kit/kitService/kitService.hpp"
#include "network/interCom/interMsg/interMsg.hpp"

class KitSrvcRgbListen : public KitService
{
    private:
    /**
     * @brief Callback to call when a new RGB message is received.
     * @param msg The RGB message.
     */
    void onRgbMsg(const Client &client, InterMsg &msg);

    /**
     * @see KitService::onStart()
     */
    void onStart() override;

    /**
     * @see KitService::onStop()
     */
    void onStop() override;

    public:
    KitSrvcRgbListen();
};

#endif // __SRVC_RGB_LISTEN_HPP__
