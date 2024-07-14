#ifndef __MODULE_H__
#define __MODULE_H__

#include <stdint.h>

class module
{
    private:

    protected:
        char* m_ip;
        bool m_connected;
        uint64_t m_lastSyncTime;
        void checkTime();

    public:
        module(char* ip);
        void process();
        bool isConnected();
        void sync();
        void setIp(char* ip);
        char* getIp();
};


#endif
