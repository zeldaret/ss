#ifndef D_LYT_MAP_GLOBAL_H
#define D_LYT_MAP_GLOBAL_H

#include "common.h"
#include "m/m_angle.h"
#include "m/m_vec.h"

class dLytMapGlobal_c {
public:
    dLytMapGlobal_c();
    ~dLytMapGlobal_c() {
        sInstance = nullptr;
    }

    enum MapMode_e {
        MAPMODE_WORLD = 0,
        MAPMODE_PROVINCE = 1,
        MAPMODE_STAGE = 3,
        MAPMODE_ZOOM = 4,

        // There probably are more special-purpose modes...
    };

    static dLytMapGlobal_c *GetInstance() {
        return sInstance;
    }

    const mVec2_c &getField_0x20() const {
        return field_0x20;
    }

    const mVec3_c &getPlayerPos() const {
        return mPlayerPosition;
    }

    u8 getAlpha() const {
        return mAlpha;
    }

    s32 getCurrentMapMode() const {
        return mCurrentMapMode;
    }

    s32 getNextMapMode() const {
        return mNextMapMode;
    }

    f32 getZoomFrame() const {
        return mZoomFrame;
    }

    f32 getField_0x44() const {
        return field_0x44;
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
    /* 0x48 */ UNKWORD mLevel;
    /* 0x4C */ u8 field_0x4C;
    /* 0x4D */ u8 field_0x4D;
    /* 0x4E */ u8 mAlpha;
    /* 0x4F */ u8 field_0x4F;
    /* 0x50 */ UNKWORD field_0x50;
    /* 0x54 */ u8 _0x54[0x5C - 0x54];
    /* 0x5C */ s32 mCurrentMapMode;
    /* 0x60 */ s32 mNextMapMode;
    /* 0x64 */ f32 mZoomFrame; ///< 0.0f when zoomed out, 1.0f when zoomed in
};

#endif
