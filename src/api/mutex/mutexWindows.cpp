#include "mutex.hpp"
#include <windows.h>

Mutex::Mutex()
{
    CRITICAL_SECTION *cs = new CRITICAL_SECTION;
    InitializeCriticalSection(cs);
    m_privateData = cs;
}

Mutex::~Mutex()
{
    if (m_privateData)
    {
        CRITICAL_SECTION *cs = static_cast<CRITICAL_SECTION *>(m_privateData);
        DeleteCriticalSection(cs);
        delete cs;
    }
}

void Mutex::lock()
{
    if (m_privateData)
    {
        EnterCriticalSection(static_cast<CRITICAL_SECTION *>(m_privateData));
    }
}

void Mutex::unlock()
{
    if (m_privateData)
    {
        LeaveCriticalSection(static_cast<CRITICAL_SECTION *>(m_privateData));
    }
}
