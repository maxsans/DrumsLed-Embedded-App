#include "srvcRgbListen.hpp"
#include "network/interCom/interMsgList/interMsgRgb/interMsgRgb.hpp"
#include "network/interCom/interComParser/interComParser.hpp"
#include "api/circleLeds/circleLeds.hpp"
#include "tools/logStream/logStream.hpp"

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

    // By default, light up the led while the master isn't registered
    LogStream::cout << "KitSrvcRgbListen initialized. Waiting for RGB messages..." << LogStream::endl;
    circleLedsFill(255, 255, 255);
}

void KitSrvcRgbListen::onStart()
{
    // Turn off the LEDs when the service starts
    circleLedsFill(0, 0, 0);
}

void KitSrvcRgbListen::onStop()
{
    // By default, light up the led while the master isn't registered
    circleLedsFill(255, 255, 255);
}

void KitSrvcRgbListen::onRgbMsg(const Client &client, InterMsg &msg)
{
    // Cast the message to RGB message type
    InterMsgRgb &rgbMsg = static_cast<InterMsgRgb&>(msg);

    // Update the circle LEDs with the RGB values
    circleLedsFill(rgbMsg.getRed(), rgbMsg.getGreen(), rgbMsg.getBlue());
}
