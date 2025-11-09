#ifndef C_CLIB_H
#define C_CLIB_H

#include "common.h"
#include "m/m_vec.h"

namespace cLib {

s16 targetAngleY(const mVec3_c &target, const mVec3_c &source);
s16 targetAngleX(const mVec3_c &target, const mVec3_c &source);
void addCalcPos2(mVec3_c *src, const mVec3_c &target, f32 scale, f32 maxStep);
f32 addCalcPosXZ(mVec3_c *src, const mVec3_c &target, f32 scale, f32 maxStep, f32 minStep);
f32 addCalcPos(mVec3_c *src, const mVec3_c &target, f32 scale, f32 maxStep, f32 minStep);
bool chasePosXZ(mVec3_c &, mVec3_c &, f32);

// made up name
f32 easeIn(f32 value, f32 exponent);
f32 easeOut(f32 value, f32 exponent);
f32 easeInOut(f32 value, f32 exponent);

} // namespace cLib

#endif
