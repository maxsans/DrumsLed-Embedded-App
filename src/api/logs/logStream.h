#ifndef __LOG_STREAM_H__
#define __LOG_STREAM_H__

#include "logs.h"
#include <sstream>

class LogStream
{
    private:
        std::ostringstream buffer;

    public:
        LogStream() = default;
        ~LogStream();

        std::ostringstream& getBuffer();

        template <typename T>
        LogStream& operator<<(const T& value)
        {
            buffer << value;
            return *this;
        }
        LogStream& operator<<(std::ostream& (*func)(std::ostream&))
        {
            buffer << func;
            return *this;
        }
        LogStream& operator<<(std::ios& (*func)(std::ios&))
        {
            buffer << func;
            return *this;
        }
        LogStream& operator<<(std::ios_base& (*func)(std::ios_base&))
        {
            buffer << func;
            return *this;
        }
        LogStream& operator<<(LogStream& (*func)(LogStream&))
        {
            return func(*this);
        }

        static LogStream& endl(LogStream& stream);
};

#endif
