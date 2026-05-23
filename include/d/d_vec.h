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

/**
 * @brief Computes left x right
 * 
 * @param result the result
 * @param left left operant
 * @param right right operand
 */
inline void vecCross(mVec3_c &result, const mVec3_c &left, const mVec3_c &right) {
    result.set(
        (left.y * right.z) - (left.z * right.y), (left.z * right.x) - (left.x * right.z),
        (left.x * right.y) - (left.y * right.x)
    );
}

#endif
