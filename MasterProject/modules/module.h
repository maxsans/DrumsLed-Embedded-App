#ifndef __MODULE_H__
#define __MODULE_H__

class module
{
    private:

    protected:
        char* m_ip;

    public:
        module();
        void setip(char* ip);
        char* getip();
};


#endif
