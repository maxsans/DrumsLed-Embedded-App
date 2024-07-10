#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#define TERMINAL_BUFFER_SIZE 256

#include "session.h"

#include <stdint.h>

class terminal
{
    private:
        static session *m_currentSession;
        static char m_buffer[TERMINAL_BUFFER_SIZE];
        static int32_t m_bufferIndex;
        static void parseBuffer();

    public:
        terminal();
        ~terminal();
        static void setCurrentSession(session *currentSession);
        static void process();
};


#endif
