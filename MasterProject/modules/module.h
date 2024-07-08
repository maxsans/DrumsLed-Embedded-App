#ifndef __MODULE_H__
#define __MODULE_H__

class module
{
    private:

    protected:
        char* m_ip;

    public:
        module();
        module(char* ip);
        void setIp(char* ip);
        char* getIp();
};


#endif
