#ifndef __KIT_H__
#define __KIT_H__

#include "network/client/client.h"

class Kit
{
private:
    /**
     * @brief Master client.
     *
     */
    static Client m_masterClient;

public:
    Kit();
    ~Kit();

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
