#ifndef EFFECTS_STRUCT_H
#define EFFECTS_STRUCT_H

#include "common.h"
#include "d/d_base.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"

class EffectsStruct {
private:
    s32 field_0x00;
    u8 field_0x04[0x1C - 0x04];

public:
    // vt at 0x1C
    EffectsStruct();
    EffectsStruct(dBase_c *);
    virtual ~EffectsStruct();

    inline void init(dBase_c *owner) {
        mpOwner = owner;
    }

    void remove(bool);
    void fn_80027320(u8);
    void fn_80029920(u16 effect, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, void *, void *);
    void fn_80029980(u16 effect, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, void *, void *);
    void fn_800299F0(u16 effect, mMtx_c *mtx, void *, void *);
    void fn_80029A10(u16 effect, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, void *, void *);
    void fn_80029A70(u16 effect, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, void *, void *);
    void setMtx(const mMtx_c &);

    static void fn_800298C0(u16 effect, mVec3_c *pos, void*, void*, void*, void*);

    void fn_80026ED0();
    void fn_80026F10();

    bool checkField0x00() const {
        return field_0x00 != 0;
    }

private:
    u8 field_0x20[0x28 - 0x20];
    /* 0x28 */ dBase_c *mpOwner;
    u8 field_0x2C[0x34 - 0x2C];
};

// Not sure if these belongs here, but it uses EffectsStruct
// Related Functions in the 8002B100 area
class EffectsStruct_Ext {
public:
    EffectsStruct_Ext() : mField_0x00(1), mField_0x01(0) {}

    void init(dBase_c *, f32, f32, f32);

    void setField_0x00(u8 val) {
        mField_0x00 = val;
    }
    void setField_0x01(u8 val) {
        mField_0x01 = val;
    }

    void fn_8002B120(f32, f32);

    /* 0x00 */ u8 mField_0x00;
    /* 0x01 */ u8 mField_0x01;
    /* 0x04 */ f32 mField_0x04;
    /* 0x08 */ f32 mField_0x08;
    /* 0x0C */ f32 mField_0x0C;
    /* 0x10 */ EffectsStruct mEff;
    virtual ~EffectsStruct_Ext() {}
};

#endif
