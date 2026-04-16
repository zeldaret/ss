#include "m/m_thread.h"

#include "egg/core/eggThreadMgr.h"
#include "rvl/OS/OSThread.h"

void mThread::registerThread(OSThread *thread, void *threadArg) {
    if (threadArg && EGG::ThreadMgr::getInstance()) {
        EGG::ThreadMgr::getInstance()->registerThread(thread, *(EGG::UnknownStruct *)threadArg);
    }
}
