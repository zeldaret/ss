#ifndef D_AREA_MATH_H
#define D_AREA_MATH_H

#include <m/m_angle.h>
#include <m/m_mtx.h>
#include <m/m_vec.h>

void matrixCreateFromPosRotYScale(mMtx_c &, const mVec3_c &, const mAng, const mVec3_c &, mMtx_c *, f32);
bool checkIfVec3fInMatrix(const mMtx_c &, const mVec3_c &);

#endif
