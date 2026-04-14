#include "m/m_thread.h"

#include "egg/core/eggUnk.h"
#include "rvl/OS/OSThread.h"

void mThread::registerThread(OSThread *thread, void *threadArg) {
    if (threadArg && EGG::ThreadMonitor::getInstance()) {
        EGG::ThreadMonitor::getInstance()->registerThread(thread, *(EGG::UnknownStruct *)threadArg);
    }
}
