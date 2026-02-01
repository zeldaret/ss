#ifndef D_SWORD_SWING_EFFECT_H
#define D_SWORD_SWING_EFFECT_H

#include "common.h"
#include "egg/gfx/eggTexture.h"
#include "m/m3d/m_proc.h"
#include "m/m_allocator.h"
#include "m/m_color.h"
#include "m/m_vec.h"

class dSwordSwingEffectProcBase_c : public m3d::proc_c {
protected:
    struct swing_entry {
        swing_entry() {
            mLifetime = 0.0f;
        }
        ~swing_entry() {}

        void set(const mVec3_c &pos1, const mColor &c1, const mVec3_c &pos2, const mColor &c2);

        /* 0x00 */ f32 mLifetime;
        /* 0x04 */ mVec3_c mv1;
        /* 0x10 */ mVec3_c mv2;
        /* 0x1C */ mColor mc1;
        /* 0x20 */ mColor mc2;
    };

public:
    dSwordSwingEffectProcBase_c() : mpEntries(nullptr), mCount(0), mHead(0) {}
    virtual ~dSwordSwingEffectProcBase_c() {
        remove();
    }

    virtual int entry() override;
    virtual void remove() override;
    virtual void drawXlu() override;

    bool create(s32 num, mAllocator_c *alloc);
    void addSwing(const mVec3_c &pos1, const mColor c1, const mVec3_c &pos2, const mColor c2);
    void calc(f32 f);

protected:
    bool hasSwings() const;

    /* 0x18 */ swing_entry *mpEntries;
    /* 0x1C */ u16 mNum;
    /* 0x1E */ u16 mCount;
    /* 0x20 */ u16 mHead;
};

class dSwordSwingEffectProc_c : public dSwordSwingEffectProcBase_c {
public:
    dSwordSwingEffectProc_c() : mpImg(nullptr) {}
    virtual ~dSwordSwingEffectProc_c() {}

    virtual void drawXlu() override;

    bool create(s32 num, mAllocator_c *alloc, EGG::ResTIMG *pImg, f32 scale);

private:
    /* 0x24 */ EGG::ResTIMG *mpImg;
    /* 0x28 */ f32 mTexScale;
};

#endif
