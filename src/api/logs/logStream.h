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

        static LogStream& endl(LogStream& stream);
};

#endif
