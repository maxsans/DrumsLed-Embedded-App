#ifndef __LOG_STREAM_H__
#define __LOG_STREAM_H__

#include "logs.h"
#include <cstring>
#include <cstdio>
#include <string>

class LogStream
{
    public:
        LogStream() = default;

        static LogStream cout;

        LogStream& operator<<(const char* value);
        LogStream& operator<<(int value);
        LogStream& operator<<(unsigned int value);
        LogStream& operator<<(float value);
        LogStream& operator<<(double value);
        LogStream& operator<<(char value);
        LogStream& operator<<(std::string value);
        LogStream& operator<<(LogStream& (*func)(LogStream&));

        static LogStream& endl(LogStream& stream);
};

#endif
