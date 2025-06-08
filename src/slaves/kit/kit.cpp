#include "kit.h"

#include "tools/logStream/logStream.h"

Client Kit::m_masterClient;

Kit::Kit()
{
}

Kit::~Kit()
{
}

Client &Kit::getMasterClient()
{
    return m_masterClient;
}

void Kit::setMasterClient(Client &client)
{
    if (m_masterClient != client)
    {
        LogStream::cout << "New master client: " << client.getIP().getIpString() << LogStream::endl;
    }
    m_masterClient = client;
}
