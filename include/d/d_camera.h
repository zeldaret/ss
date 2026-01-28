#ifndef D_CAMERA_H
#define D_CAMERA_H

#include "common.h"
#include "d/d_base.h"
#include "m/m_angle.h"
#include "m/m_vec.h"

extern "C" bool fn_80081FE0(void *, const char *);
extern "C" bool fn_800918E0(void *, s32, s16);
extern "C" void fn_80080960(void *, s32, s32, s8, s32);
extern "C" void fn_80093340(void *);
extern "C" void fn_80093360(void *);
extern "C" void fn_80093380(void *);

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
    f32 getField_0x2A8() const {
        return field_0x2A8;
    }
    mAng getYRot() const;

    bool doFn_800918E0(s32 a1, s16 a2) {
        return fn_800918E0(field_0xDA0, a1, a2);
    }

    void doFn_80093340() {
        fn_80093340(field_0xDA4);
    }

    void doFn_80093360() {
        fn_80093360(field_0xDA4);
    }

    void doFn_80093380() {
        fn_80093380(field_0xDA4);
    }

    void setScreenShakeIntensity(f32 val) {
        mScreenShakeIntensity = val;
    }

    UNKWORD getField_0xDA8() const {
        return field_0xDA8;
    }

    bool isUnderwater() const;
    f32 getUnderwaterDepth() const;

    void fn_8019EA00(const mVec3_c&, const mVec3_c&, f32, f32);
    void fn_8019E430();
    void fn_8019E410();
    mAng fn_8019E3B0() const;
    void fn_8019EA70(bool);

private:
    /* 0x068 */ u8 _0x068[0x6C - 0x068];
    /* 0x06C */ mVec3_c mPositionMaybe;
    /* 0x078 */ mVec3_c field_0x78;
    /* 0x084 */ u8 _0x084[0x2A8 - 0x084];
    /* 0x2A8 */ f32 field_0x2A8;
    /* 0x2AC */ u8 _0x2AC[0xD98 - 0x2AC];
    /* 0xD98 */ void *field_0xD98;
    /* 0xD9C */ u8 _0xD9C[0xDA0 - 0xD9C];
    /* 0xDA0 */ void *field_0xDA0;
    /* 0xDA4 */ void *field_0xDA4;
    /* 0xDA8 */ UNKWORD field_0xDA8;
    /* 0xDAC */ u8 _0xDAC[0xDB0 - 0xDAC];
    /* 0xDB0 */ f32 mScreenShakeIntensity;
};

#endif
