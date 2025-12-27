#ifndef D_VEC_H
#define D_VEC_H

#include "m/m_angle.h"
#include "m/m_vec.h"

/**
 * @brief Gets a point on a XZ circle.
 * 
 * @param center   the circle center. Will also hold the result.
 * @param angle    the Y angle.
 * @param radius   the circle radius.
 */
inline static void getXZCirclePoint(mVec3_c &center, const mAng &angle, f32 radius) {
    center.x += radius * angle.sin();
    center.z += radius * angle.cos();
}

/**
 * @brief Converts a 2D vector to a 3D vector holding the original's vector coordinates in its XY components.
 * 
 * @param v         The 2D vector
 * @return mVec3_c  The 3D vector
 */
inline mVec3_c vec2ToVec3XY(const mVec2_c &v) {
    return mVec3_c(v.x, v.y, 0.0f);
}

#endif
