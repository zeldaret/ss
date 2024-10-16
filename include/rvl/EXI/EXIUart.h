#ifndef RVL_SDK_EXI_UART_H
#define RVL_SDK_EXI_UART_H
#include "common.h"
#include "rvl/EXI/EXIBios.h" // IWYU pragma: export


#ifdef __cplusplus
extern "C" {
#endif

void __OSEnableBarnacle(EXIChannel, u32);

#ifdef __cplusplus
}
#endif
#endif
