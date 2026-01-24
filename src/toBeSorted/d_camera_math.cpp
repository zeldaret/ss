#include "toBeSorted/d_camera_math.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m_vec.h"
#include "toBeSorted/deg_angle_util.h"

#include <math.h>

void d_camera_math_float_order() {
    // Maybe more functions here
    0.5;
    1.0;
    0.0f;
    -1.0;
    2.0;
    4.0;
    -1e-7;
}

f32 camEaseIn(f32 arg, f32 param) {
    // TODO - regswaps, maybe equivalent
    f64 factor;
    if (arg >= 0.0f) {
        factor = 1.0;
    } else {
        factor = -1.0;
        arg = -arg;
    }

    f64 f1 = 2.0 * arg * param - 2.0 * arg - 2.0 * param;
    f64 tmp = (-f1 - 1.0);
    f64 f6 = f1 * f1 - tmp * 4.0 * arg;
    if (f6 > -1e-7) {
        f6 = sqrt(f6);
    } else {
        f6 = 0.0;
    }

    f64 f2 = 2.0 * tmp;
    f64 f2_ = (-f1 - f6);
    if (f2 > 1e-7 || f2 < -1e-7) {
        f2 = f2_ / f2;
        f6 = 1.0 - f2;
        f6 = f2 * f2 + f6 * f6 + param * 2.0 * f6 * f2;
        // What
        if (f6 > 1.0000000116860974e-7) {
            f6 = (f2 * f2) / f6;
        } else {
            f6 = 0.0;
        }
    } else {
        f32 a = camEaseIn(arg - 1e-6f, param);
        f32 b = camEaseIn(arg + 1e-6f, param);
        f6 = (a + b) * 0.5;
    }
    return f6 * factor;
}

f32 camEaseInOut(f32 arg, f32 param) {
    if (arg <= 0.0f) {
        return 0.0f;
    } else if (arg >= 1.0f) {
        return 1.0f;
    }

    f32 f1;
    f32 result;
    if (arg <= 0.5f) {
        f1 = arg * 2.0f;
        result = camEaseIn(f1, param) * 0.5f;
    } else {
        f1 = (1.0f - arg) * 2.0f;
        result = (1.0f - camEaseIn(f1, param)) * 0.5f + 0.5f;
    }
    return result * (1.0f - f1) + arg * f1;
}

mVec3_c camGetPointOnLine(const mVec3_c &target, const mVec3_c &origin, const mVec3_c &point) {
    // NONMATCHING - stack swap between the two temporary vectors
    dPolar p(target - origin);
    p.R = (point - origin).mag();
    return origin + p.toCartesian();
}

mVec3_c camOffsetPoint(dAcObjBase_c *ac, const mVec3_c &localOffset) {
    mVec3_c result = localOffset;
    if (ac != nullptr) {
        result.rotX(ac->mRotation.x);
        result.rotY(ac->mRotation.y);
        result += ac->mPosition;
    }
    return result;
}


mVec3_c camOffsetPoint(dAcObjBase_c *ac, const mVec3_c &localOffset, const mAng3_c &rot) {
    mVec3_c result = localOffset;
    if (ac != nullptr) {
        result.rotX(rot.x);
        result.rotY(rot.y);
        result += ac->mPosition;
    }
    return result;
}

mVec3_c camPointToLocal(dAcObjBase_c *ac, const mVec3_c &pos) {
    mVec3_c result = pos;
    if (ac != nullptr) {
        result -= ac->mPosition;
        // TODO: Shouldn't these be in the opposite order for a true inverse?
        result.rotX(-ac->mRotation.x);
        result.rotY(-ac->mRotation.y);
    }
    return result;
}

mVec3_c camPointToLocal(dAcObjBase_c *ac, const mVec3_c &pos, const mAng3_c &rot) {
    mVec3_c result = pos;
    if (ac != nullptr) {
        result -= ac->mPosition;
        // TODO: Shouldn't these be in the opposite order for a true inverse?
        result.rotX(-rot.x);
        result.rotY(-rot.y);
    }
    return result;
}
