#ifndef D_SWORD_SWING_EFFECT_MGR_H
#define D_SWORD_SWING_EFFECT_MGR_H

#include "m/m_color.h"
#include "m/m_vec.h"
#include "toBeSorted/d_sword_swing_effect.h"

// TODO better name
class dSwordSwingEffectProcMgr_c {
private:
    /* 0x00 */ dSwordSwingEffectProc_c mProc;
    /* 0x2C */ bool mIsActive;
    /* 0x30 */ mColor mC1;
    /* 0x34 */ mColor mC2;
    /* 0x38 */ mVec3_c field_0x38;
    /* 0x44 */ mVec3_c field_0x44;

    void createSwingEntries(const mVec3_c &v1, const mVec3_c &v2);

public:
    dSwordSwingEffectProcMgr_c() : mC1(0xFF, 0xFF, 0xFF, 0x40), mC2(0xFF, 0xFF, 0xFF, 0x20) {}
    virtual ~dSwordSwingEffectProcMgr_c() {}

    void setColor1(mColor c1) {
        mC1 = c1;
    }

    void setColor2(mColor c1) {
        mC2 = c1;
    }

    void setActive(bool v) {
        mIsActive = v;
    } 

    bool create(s32 num, mAllocator_c *alloc, EGG::ResTIMG *pImg, f32 scale) {
        return mProc.create(num, alloc, pImg, scale);
    }
    void calc(const mVec3_c &v1, const mVec3_c &v2);
    void entry();
};

#endif
