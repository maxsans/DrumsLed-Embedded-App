#include "logStream.hpp"
#include "api/logs/logs.hpp"
#include <cstdarg>
#include <cstdio>

LogStream LogStream::cout;

LogStream &LogStream::operator<<(const char *value)
{
    log("%s", value);
    return *this;
}

LogStream &LogStream::operator<<(int value)
{
    log("%d", value);
    return *this;
}

LogStream &LogStream::operator<<(unsigned int value)
{
    log("%u", value);
    return *this;
}

LogStream &LogStream::operator<<(long value)
{
    log("%ld", value);
    return *this;
}

LogStream &LogStream::operator<<(unsigned long value)
{
    log("%lu", value);
    return *this;
}

LogStream &LogStream::operator<<(long long value)
{
    log("%lld", value);
    return *this;
}

LogStream &LogStream::operator<<(unsigned long long value)
{
    log("%llu", value);
    return *this;
}

LogStream &LogStream::operator<<(float value)
{
    log("%f", value);
    return *this;
}

LogStream &LogStream::operator<<(double value)
{
    log("%f", value);
    return *this;
}

LogStream &LogStream::operator<<(char value)
{
    log("%c", value);
    return *this;
}

LogStream &LogStream::operator<<(std::string value)
{
    log("%s", value.c_str());
    return *this;
}

LogStream &LogStream::operator<<(LogStream &(*func)(LogStream &))
{
    return func(*this);
}

LogStream &LogStream::endl(LogStream &stream)
{
    log("\n");
    return stream;
}
