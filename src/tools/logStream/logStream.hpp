#ifndef __LOG_STREAM_HPP__
#define __LOG_STREAM_HPP__

#include "api/logs/logs.hpp"
#include <cstdio>
#include <cstring>
#include <string>

class LogStream
{
    public:
    LogStream() = default;

    static LogStream cout;

    LogStream &operator<<(const char *value);
    LogStream &operator<<(int value);
    LogStream &operator<<(unsigned int value);
    LogStream &operator<<(float value);
    LogStream &operator<<(double value);
    LogStream &operator<<(char value);
    LogStream &operator<<(std::string value);
    LogStream &operator<<(LogStream &(*func)(LogStream &));

    static LogStream &endl(LogStream &stream);
};

#endif
