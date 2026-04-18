#ifndef M_THREAD_H
#define M_THREAD_H

// This file is a complete guess

struct OSThread;

namespace mThread {

void registerThread(OSThread *thread, void *threadArg);

} // namespace mThread

#endif
