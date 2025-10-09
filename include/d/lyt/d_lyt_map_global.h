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
        // aka area
        MAPMODE_PROVINCE = 1,
        // aka area
        MAPMODE_WORLD_SKY = 2,
        // aka course
        MAPMODE_STAGE = 3,
        // aka detail
        MAPMODE_ZOOM = 4,
    };

    static dLytMapGlobal_c *GetInstance() {
        return sInstance;
    }

    const mVec3_c &getMapScroll() const {
        return mMapScroll;
    }

    void setMapScroll(const mVec3_c &v) {
        mMapScroll = v;
    }

    void setField_0x0C(const mVec2_c &v) {
        field_0x0C = v;
    }

    const mVec2_c &getField_0x20() const {
        return field_0x20;
    }

    void setField_0x20(const mVec2_c &v) {
        field_0x20 = v;
    }

    const mVec2_c &getField_0x28() const {
        return field_0x28;
    }

    void setField_0x28(const mVec2_c &v) {
        field_0x28 = v;
    }

    void setMapRotationCenter(const mVec3_c &v) {
        mMapRotationCenter = v;
    }

    const mVec3_c &getMapRotationCenter() const {
        return mMapRotationCenter;
    }

    const mAng &getMapRotation() const {
        return mMapRotation;
    }

    void setMapRotation(const mAng &rot) {
        mMapRotation = rot;
    }

    const mVec3_c &getPlayerPos() const {
        return mPlayerPosition;
    }

    u8 getField_0x55() const {
        return field_0x55;
    }

    const mAng &getField_0x56() const {
        return field_0x56;
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

    void setZoomFrame(f32 v) {
        mZoomFrame = v;
    }

    void setField_0x58(f32 v) {
        field_0x58 = v;
    }

    f32 getField_0x44() const {
        return field_0x44;
    }

    void setField_0x44(f32 v) {
        field_0x44 = v;
    }

    s32 getFloor() const {
        return mFloor;
    }

    void setFloor(s32 floor) {
        mFloor = floor;
    }

    u8 getField_0x4C() const {
        return field_0x4C;
    }

    void setField_0x4C(u8 v) {
        field_0x4C = v;
    }

    u8 getField_0x4D() const {
        return field_0x4D;
    }

    u8 getField_0x4F() const {
        return field_0x4F;
    }

    void setField_0x4F(u8 v) {
        field_0x4F = v;
    }

    UNKWORD getField_0x50() const {
        return field_0x50;
    }

    u8 getField_0x54() const {
        return field_0x54;
    }

    void projectOntoMap(mVec2_c &result, const mVec3_c &position) const;
    void unprojectFromMap(mVec3_c &result, const mVec2_c &position) const;
    void unprojectFromMap(mVec3_c &result, const mVec2_c &position, const mAng &rot) const;
    void unprojectFromMap(mVec3_c &result, const mVec2_c &position, const mVec3_c &v2, const mAng &rot) const;

private:
    static dLytMapGlobal_c *sInstance;

    void projectOntoMap(
        mVec2_c &result, const mVec3_c &position, const mVec3_c &mapScroll, const mVec3_c &mapRotationCenter,
        const mAng &rot, f32 f1, f32 f2
    ) const;

    void unprojectFromMap(
        mVec3_c &result, const mVec2_c &position, const mVec2_c &v1, const mVec3_c &mapScroll,
        const mVec3_c &mapRotationCenter, const mAng &rot, f32 f1, f32 f2
    ) const;

    /* 0x00 */ mVec3_c mMapScroll;
    /* 0x0C */ mVec2_c field_0x0C;
    /* 0x14 */ mVec3_c mMapRotationCenter;
    /* 0x20 */ mVec2_c field_0x20;
    /* 0x28 */ mVec2_c field_0x28;
    /* 0x30 */ mVec3_c mPlayerPosition;
    /* 0x3C */ mAng mMapRotation;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ s32 mFloor;
    /* 0x4C */ u8 field_0x4C;
    /* 0x4D */ u8 field_0x4D;
    /* 0x4E */ u8 mAlpha;
    /* 0x4F */ u8 field_0x4F;
    /* 0x50 */ UNKWORD field_0x50;
    /* 0x54 */ u8 field_0x54;
    /* 0x55 */ u8 field_0x55;
    /* 0x56 */ mAng field_0x56;
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ s32 mCurrentMapMode;
    /* 0x60 */ s32 mNextMapMode;
    /* 0x64 */ f32 mZoomFrame; ///< 0.0f when zoomed out, 1.0f when zoomed in
};

#endif
