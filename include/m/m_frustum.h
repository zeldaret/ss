#ifndef M_FRUSTUM_H
#define M_FRUSTUM_H

#include "common.h"
#include "m/m_mtx.h"
#include "nw4r/math/math_geometry.h"

// Bunch of untested assumptions in here
class mFrustum_c {
public:
    void set(f32, f32, f32 near, f32 far, const mMtx_c &mtx);
    void set(f32, f32, f32, f32, f32 near, f32 far, const mMtx_c &mtx, bool);


    /* 0x00 */ mMtx_c mView;
    /* 0x30 */ nw4r::math::PLANE mPlane_0x30;
    /* 0x40 */ nw4r::math::PLANE mPlane_0x40;
    /* 0x50 */ nw4r::math::PLANE mPlane_0x50;
    /* 0x60 */ nw4r::math::PLANE mPlane_0x60;
    /* 0x70 */ f32 mNear;
    /* 0x74 */ f32 mFar;
    /* 0x78 */ nw4r::math::AABB mAabb;
    /* 0x90 */ nw4r::math::PLANE mPlane_0x90;
    /* 0xA0 */ nw4r::math::PLANE mPlane_0xA0;
    /* 0xB0 */ nw4r::math::PLANE mPlane_0xB0;
    /* 0xC0 */ nw4r::math::PLANE mPlane_0xC0;
    /* 0xD0 */ nw4r::math::PLANE mPlane_0xD0;
    /* 0xE0 */ nw4r::math::PLANE mPlane_0xE0;
};

#endif
