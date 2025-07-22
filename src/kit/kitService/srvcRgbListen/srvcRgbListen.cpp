#include "srvcRgbListen.hpp"
#include "network/interCom/interMsgList/interMsgRgb/interMsgRgb.hpp"
#include "network/interCom/interComParser/interComParser.hpp"
#include "api/circleLeds/circleLeds.hpp"

KitSrvcRgbListen::KitSrvcRgbListen()
    : KitService(KitServiceType::RgbListen)
{
    // Register the callback for RGB messages
    InterComParser::registerCallback(InterMsgId::Rgb,
        [this](const Client &client, InterMsg &msg, void *object)
        {
            this->onRgbMsg(client, msg);
        }
    );
}

void KitSrvcRgbListen::onRgbMsg(const Client &client, InterMsg &msg)
{
    // Cast the message to RGB message type
    InterMsgRgb &rgbMsg = static_cast<InterMsgRgb&>(msg);

    // Update the circle LEDs with the RGB values
    circleLedsFill(rgbMsg.getRed(), rgbMsg.getGreen(), rgbMsg.getBlue());
}
