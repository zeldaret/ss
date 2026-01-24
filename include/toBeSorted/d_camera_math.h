#ifndef TOBESORTED_D_CAMERA_MATH_H
#define TOBESORTED_D_CAMERA_MATH_H

#include "common.h"
#include "m/m_angle.h"
#include "m/m_vec.h"

class dAcObjBase_c;

/**
 * @brief A function that looks like an ease-in tween.
 *
 * @param arg    The time argument in [0, 1]
 * @param param  The power of the easing function. 0 = linear, 1 = quite powerful
 * @return f32   The result in [0, 1]
 */
f32 camEaseIn(f32 arg, f32 param);

/**
 * @brief A function that looks like an ease-in-out tween.
 *
 * @param arg    The time argument in [0, 1]
 * @param param  The power of the easing function. 0 = linear, 1 = a little bit
 * @return f32   The result in [0, 1]
 */
f32 camEaseInOut(f32 arg, f32 param);

/**
 * @brief Get a point on the line from origin to target that is as
 *        far away from origin as point is away from origin.
 */
mVec3_c camGetPointOnLine(const mVec3_c &target, const mVec3_c &origin, const mVec3_c &point);

/**
 * @brief Get a point relative to ac's position. localOffset is rotated by the actor's XY rotation first.
 */
mVec3_c camOffsetPoint(dAcObjBase_c *ac, const mVec3_c &localOffset);

/**
 * @brief Like the other camOffsetPoint, but with an explicitly passed rotation.
 */
mVec3_c camOffsetPoint(dAcObjBase_c *ac, const mVec3_c &localOffset, const mAng3_c &rot);

/**
 * @brief The inverse of camOffsetPoint
 */
mVec3_c camPointToLocal(dAcObjBase_c *ac, const mVec3_c &pos);

/**
 * @brief The inverse of camOffsetPoint
 */
mVec3_c camPointToLocal(dAcObjBase_c *ac, const mVec3_c &pos, const mAng3_c &rot);

#endif
