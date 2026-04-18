#ifndef D_MAIN_H
#define D_MAIN_H

#include "rvl/OS.h"

namespace dMain {

bool isStopFlagSet(u32 val);
void setStopFlag(u32 val);
void clearStopFlag(u32 val);

void Create();
void Execute();
void *main01(void *arg);

}; // namespace dMain

#endif // D_MAIN_H
