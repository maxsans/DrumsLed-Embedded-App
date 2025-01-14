#include "logs.h"
#include "logStream.h"

LogStream::~LogStream()
{
    log("%s", buffer.str().c_str());
}

std::ostringstream& LogStream::getBuffer()
{
    return buffer;
}

LogStream& LogStream::endl(LogStream& stream)
{
    stream.getBuffer() << std::endl;
    return stream;
}
