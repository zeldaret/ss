#ifndef EGG_DRAW_PATH_DOF_H
#define EGG_DRAW_PATH_DOF_H

#include "egg/gfx/eggDrawPathBase.h"
#include "egg/gfx/eggTextureBuffer.h"
#include "egg/prim/eggBinary.h"
#include "egg/egg_types.h"

namespace EGG {

class DrawPathDOF : public DrawPathBase, public IBinary<DrawPathDOF> {
public:
    class BinData {
        // TODO
    };
    DrawPathDOF();
    virtual ~DrawPathDOF();

    virtual u16 getNumStep() const override {
        return 3;
    }
    virtual void internalResetForDraw() override;
    virtual void internalDraw(u16) override;

    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) override;


    void execute();

private:
    void internalDrawStandard(u16);
    void internalDrawLite(u16);

    void allocAndCaptureBlurBuffer(u16 x, u16 y, u16 width, u16 height);

    /* 0x1C */ u16 field_0x1C;
    /* 0x1E */ u8 field_0x1E;
    /* 0x1F */ u8 field_0x1F;
    /* 0x20 */ s32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ s32 field_0x34;
    /* 0x38 */ s32 field_0x38;

    /* 0x3C */ u8 _0x3C[0x5C - 0x3C];

    /* 0x5C */ f32 field_0x5C;
    /* 0x60 */ f32 field_0x60;
    /* 0x64 */ f32 field_0x64;
    /* 0x68 */ f32 field_0x68;
    /* 0x6C */ f32 field_0x6C;
    /* 0x70 */ f32 field_0x70;
    /* 0x74 */ f32 field_0x74;
    /* 0x78 */ f32 field_0x78;
    /* 0x7C */ f32 field_0x7C;
    /* 0x80 */ u8 field_0x80;
    /* 0x84 */ PostEffectBlur *mpBlur;
    /* 0x88 */ PostEffectMaskDOF *mpMaskDof;
    /* 0x8C */ TextureBuffer *mpTextureBuffer;
    /* 0x90 */ PostEffectMask *mpMask;
};

} // namespace EGG

#endif
