#ifndef C_CLIB_H
#define C_CLIB_H

#include "common.h"
#include "m/m_vec.h"

namespace cLib {

s32 targetAngleY(const mVec3_c &target, const mVec3_c &source);
f32 addCalcPosXZ(mVec3_c *src, const mVec3_c &target, f32 scale, f32 maxStep, f32 minStep);

} // namespace cLib

#endif
