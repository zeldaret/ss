#ifndef D_MAIN_H
#define D_MAIN_H

#include "rvl/OS.h"

namespace dMain {
    void Create();
    void Execute();
    void *main01(void *arg);
    OSTime g_InitialTime;
};

#endif // D_MAIN_H
