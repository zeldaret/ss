#ifndef D_MAIN_H
#define D_MAIN_H

#include "rvl/OS.h"

namespace dMain {

OSTime g_InitialTime;

void Create();
void Execute();
void *main01(void *arg);

}; // namespace dMain

#endif // D_MAIN_H
