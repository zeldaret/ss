#ifndef RVL_SDK_MTX_MTXVEC_H
#define RVL_SDK_MTX_MTXVEC_H
#include "common.h"
#include "rvl/MTX/mtx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MTXMultVec        PSMTXMultVec
#define MTXMultVecArray   PSMTXMultVecArray
#define MTXMultVecSR      PSMTXMultVecSR
#define MTXMultVecArraySR PSMTXMultVecArraySR

void PSMTXMultVec(const Mtx m, const Vec* src, Vec* dst);
void PSMTXMultVecArray(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count);
void PSMTXMultVecSR(const Mtx m, const Vec* src, Vec* dst);
void PSMTXMultVecArraySR(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count);

#ifdef __cplusplus
}
#endif
#endif
