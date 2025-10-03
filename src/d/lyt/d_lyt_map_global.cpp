#include "d/lyt/d_lyt_map_global.h"

#include "m/m_vec.h"

dLytMapGlobal_c *dLytMapGlobal_c::sInstance;

dLytMapGlobal_c::dLytMapGlobal_c()
    : mMapScroll(0.0f, 0.0f, 0.0f),
      field_0x0C(0.0f, 0.0f),
      mMapRotationCenter(0.0f, 0.0f, 0.0f),
      field_0x20(0.0f, 0.0f),
      field_0x28(0.0f, 0.0f),
      mMapRotation(0),
      field_0x40(1.0f),
      field_0x44(1.0f),
      mFloor(0),
      field_0x4C(0),
      field_0x4D(0),
      mAlpha(0),
      field_0x4F(0),
      field_0x50(0) {
    sInstance = this;
}

void dLytMapGlobal_c::projectOntoMap(
    mVec2_c &result, const mVec3_c &position, const mVec3_c &mapScroll, const mVec3_c &mapRotationCenter, const mAng &rot,
    f32 f1, f32 f2
) const {
    // Initializing like this fixes regswaps, even if it's not needed...
    mVec3_c work(0.0f, 0.0f, 0.0f);

    work.x = position.x - mapRotationCenter.x;
    work.z = position.z - mapRotationCenter.z;
    work.y = 0.0f;
    work.rotY(-rot);

    work.x += mapRotationCenter.x - mapScroll.x;
    work.z += mapRotationCenter.z - mapScroll.z;

    work.x *= f2 * f1;
    work.z *= f2 * f1;
    work.x *= field_0x28.x;

    result.x = work.x;
    result.y = -work.z;
}

void dLytMapGlobal_c::projectOntoMap(mVec2_c &result, const mVec3_c &position) const {
    projectOntoMap(result, position, mMapScroll, mMapRotationCenter, mMapRotation, field_0x44, field_0x40);
}

void dLytMapGlobal_c::unprojectFromMap(mVec3_c &result, const mVec2_c &position) const {
    unprojectFromMap(
        result, position, field_0x20, mMapScroll, mMapRotationCenter, mMapRotation, field_0x44, field_0x40
    );
}

void dLytMapGlobal_c::unprojectFromMap(mVec3_c &result, const mVec2_c &position, const mAng &rot) const {
    unprojectFromMap(result, position, field_0x20, mMapScroll, mMapRotationCenter, rot, field_0x44, field_0x40);
}

void dLytMapGlobal_c::unprojectFromMap(mVec3_c &result, const mVec2_c &position, const mVec3_c &v2, const mAng &rot)
    const {
    unprojectFromMap(result, position, field_0x20, v2, mMapRotationCenter, rot, field_0x44, field_0x40);
}

void dLytMapGlobal_c::unprojectFromMap(
    mVec3_c &result, const mVec2_c &position, const mVec2_c &v1, const mVec3_c &mapScroll, const mVec3_c &mapRotationCenter,
    const mAng &rot, f32 f1, f32 f2
) const {
    mVec3_c work(0.0f, 0.0f, 0.0f);

    f1 *= f2;
    f32 tmp = 1.0f / f1;
    mVec2_c diff = position - v1;

    work.x = diff.x * tmp * (1.0f / field_0x28.x);
    work.z = -diff.y * tmp;

    work = work + mapScroll;

    work = work - mapRotationCenter;
    work.rotY(rot);
    work += mapRotationCenter;

    result = work;
}
