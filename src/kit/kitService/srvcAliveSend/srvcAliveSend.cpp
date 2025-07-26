#include "srvcAliveSend.hpp"
#include "network/interCom/interMsgList/interMsgAlive/interMsgAlive.hpp"

const timeMs KitSrvcAliveSend::m_sendInterval = timeMs(500);

KitSrvcAliveSend::KitSrvcAliveSend()
    : KitService(KitServiceType::AliveSend),
      m_sendPeriodicCall(m_sendInterval, &KitSrvcAliveSend::periodicSendCallback, this)
{
    // For now the communication isn't established with the master client
    m_sendPeriodicCall.enable(false);
}

void KitSrvcAliveSend::periodicSendCallback(void *object)
{
    KitSrvcAliveSend *service = static_cast<KitSrvcAliveSend *>(object);
    service->periodicSendCallback();
}

void KitSrvcAliveSend::periodicSendCallback()
{
    // Send the alive message to the master client
    Client l_masterClient = getMasterClient();
    InterMsgAlive l_msgAlive(l_masterClient);
    // Send the alive message to the master client
    l_msgAlive.send();
}

void KitSrvcAliveSend::onStart()
{
    // Start the periodic calls for sending alive messages
    // since the communication with the master client is established
    m_sendPeriodicCall.enable(true);
}

void KitSrvcAliveSend::onStop()
{
    // Stop the periodic calls for sending alive messages
    m_sendPeriodicCall.enable(false);
}
