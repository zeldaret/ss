#ifndef EGG_DRAW_PATH_BLOOM_H
#define EGG_DRAW_PATH_BLOOM_H

#include "egg/gfx/eggDrawPathBase.h"
#include "egg/gfx/eggPostEffectBlur.h"
#include "egg/gfx/eggPostEffectBlurGather.h"
#include "egg/gfx/eggPostEffectMask.h"
#include "egg/gfx/eggPostEffectSimple.h"
#include "egg/prim/eggBinary.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class DrawPathBloom : public DrawPathBase, public IBinary<DrawPathBloom> {
    struct BinData {};

    // Size 0x20?
    struct SubData {
        /* 0x00 */ f32 field_0x00;
        /* 0x04 */ f32 field_0x04;
        /* 0x08 */ u8 field_0x08;
        /* 0x09 */ u8 _0x09[0x20 - 0x09];
    };

    // Not sure about this union thing but need to prevent compiler
    // from trying fancy struct copying
    struct Data {
        union {
            struct {
                /* 0x00 */ f32 field_0x00;
                /* 0x04 */ GXColor field_0x04;
                /* 0x08 */ GXColor field_0x08;
                /* 0x0C */ u16 field_0x0C;
                /* 0x10 */ SubData field_0x10[3];
                /* 0x50 */ u8 field_0x50;
                /* 0x51 */ u8 field_0x51;
                /* 0x52 */ u8 field_0x52;
                /* 0x53 */ u8 _0x53[0x6C - 0x53];
                /* 0x6C */ f32 field_0x6C;
                /* 0x70 */ f32 field_0x70;

            };
        };
    };

public:
    DrawPathBloom();
    virtual ~DrawPathBloom();

    virtual u16 getNumStep() const override {
        return 3;
    }

    virtual void internalResetForDraw() override;
    virtual void internalDraw(u16) override;

    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) override;

private:
    /* 0x01C */ u8 field_0x01C;
    /* 0x01D */ u8 field_0x01D;
    /* 0x020 */ PostEffectMask mMask;
    /* 0x1E0 */ PostEffectBlur mBlur1;
    /* 0x258 */ PostEffectBlur mBlur2;
    /* 0x2D0 */ PostEffectBlurGather mBlurGather;
    /* 0x340 */ PostEffectSimple mSimple;
    /* 0x37C */ u32 field_0x37C;
    /* 0x380 */ Data field_0x380;
};

} // namespace EGG

#endif
