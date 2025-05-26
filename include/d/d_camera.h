#ifndef D_CAMERA_H
#define D_CAMERA_H

#include "d/d_base.h"
#include "m/m_vec.h"

extern "C" bool fn_80081FE0(void *, const char *);
extern "C" bool fn_800918E0(void *, s32, s16);
extern "C" void fn_80080960(void *, s32, s32, s8, s32);

class dCamera_c : public dBase_c {
public:
    bool isCurrentTrend(const char *name) const {
        return fn_80081FE0(field_0xD98, name);
    }

    void *getField_0xD98() const {
        return field_0xD98;
    }

    const mVec3_c &getPositionMaybe() const {
        return mPositionMaybe;
    }
    const mVec3_c &getField_0x78() const {
        return field_0x78;
    }

    bool doFn_800918E0(s32 a1, s16 a2) {
        return fn_800918E0(field_0xDA0, a1, a2);
    }

    void setScreenShakeIntensity(f32 val) {
        mScreenShakeIntensity = val;
    }

private:
    /* 0x068 */ u8 _0x068[0x6C - 0x068];
    /* 0x06C */ mVec3_c mPositionMaybe;
    /* 0x078 */ mVec3_c field_0x78;
    /* 0x084 */ u8 _0x084[0xD98 - 0x084];
    /* 0xD98 */ void *field_0xD98;
    /* 0xD9C */ u8 _0xD9C[0xDA0 - 0xD9C];
    /* 0xDA0 */ void *field_0xDA0;
    /* 0xDA4 */ u8 _0xDA4[0xDB0 - 0xDA4];
    /* 0xDB0 */ f32 mScreenShakeIntensity;
};

#endif
