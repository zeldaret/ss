#ifndef RVL_SDK_MTX_MTXVEC_H
#define RVL_SDK_MTX_MTXVEC_H
#include "rvl/MTX/mtx.h"
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

void PSMTXMultVec(const Mtx, const Vec *, Vec *);
void PSMTXMultVecSR(const Mtx, const Vec *, Vec *);

#ifdef __cplusplus
}
#endif
#endif
