/**
 * @file os.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "os.hpp"
#include "tools/os/async/async.hpp"
#include "tools/os/periodicCallsMs/periodicCallsMs.hpp"

void OS::process()
{
    periodicCallsMs::processAll();
    Async::process();
}
