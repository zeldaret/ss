#ifndef D_MAIN_H
#define D_MAIN_H

#include "rvl/OS.h"

namespace dMain {

bool fn_80054F90(u32 val);
void fn_80054FB0(u32 val);
void fn_80054FE0(u32 val);

void Create();
void Execute();
void *main01(void *arg);

}; // namespace dMain

#endif // D_MAIN_H
