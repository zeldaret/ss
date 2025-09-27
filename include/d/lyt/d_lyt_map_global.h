#ifndef D_LYT_MAP_GLOBAL_H
#define D_LYT_MAP_GLOBAL_H

#include "m/m_angle.h"
#include "m/m_vec.h"

class dLytMapGlobal_c {
public:
    dLytMapGlobal_c();

    static dLytMapGlobal_c *GetInstance() {
        return sInstance;
    }

    void projectOntoMap(mVec2_c &result, const mVec3_c &position) const;
    void unprojectFromMap(mVec3_c &result, const mVec2_c &position) const;
    void unprojectFromMap(mVec3_c &result, const mVec2_c &position, const mAng &rot) const;
    void unprojectFromMap(mVec3_c &result, const mVec2_c &position, const mVec3_c &v2, const mAng &rot) const;

private:
    static dLytMapGlobal_c *sInstance;

    void projectOntoMap(
        mVec2_c &result, const mVec3_c &position, const mVec3_c &v1, const mVec3_c &mapRotationCenter, const mAng &rot,
        f32 f1, f32 f2
    ) const;

    void unprojectFromMap(
        mVec3_c &result, const mVec2_c &position, const mVec2_c &v1, const mVec3_c &v2,
        const mVec3_c &mapRotationCenter, const mAng &rot, f32 f1, f32 f2
    ) const;

    /* 0x00 */ mVec3_c field_0x00;
    /* 0x0C */ mVec2_c field_0x0C;
    /* 0x14 */ mVec3_c mMapRotationCenter;
    /* 0x20 */ mVec2_c field_0x20;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ mVec3_c mPlayerPosition;
    /* 0x3C */ mAng mMapRotation;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ UNKWORD field_0x48;
    /* 0x4C */ u8 field_0x4C;
    /* 0x4D */ u8 field_0x4D;
    /* 0x4E */ u8 field_0x4E;
    /* 0x4F */ u8 field_0x4F;
    /* 0x50 */ UNKWORD field_0x50;
    /* 0x54 */ u8 _0x54[0x64 - 0x54];
    /* 0x60 */ f32 field_0x64;
};

#endif
