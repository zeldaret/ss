#ifndef D_ANGLE_H
#define D_ANGLE_H

#include "m/m_angle.h"
#include "m/m_vec.h"

inline void setRotXY(mAng3_c &rot, s32 x, s32 y) {
    rot.set(x, y, mAng(0));
}

inline void setRotXYVec(mAng3_c &rot, const mVec3_c &direction) {
    setRotXY(rot, -direction.atan2sY_XZ(), direction.atan2sX_Z());
}

#endif
