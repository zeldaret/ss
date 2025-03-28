#ifndef EGG_DRAW_PATH_LIGHTMAP_H
#define EGG_DRAW_PATH_LIGHTMAP_H

#include "egg/gfx/eggDrawPathBase.h"
#include "egg/gfx/eggTextureBuffer.h"

namespace EGG {

// A guess based of what is seen in ogws. Has been Updated
class DrawPathLightMap : public DrawPathBase {
public:
    DrawPathLightMap();

    virtual u16 getNumStep() const override {
        return 3;
    }
    virtual void internalResetForDraw() override;
    virtual void internalDraw(u16) override;
    virtual ~DrawPathLightMap();
    void freeTextureBuffers();

private:
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 mAlpha;
    /* 0x1C */ int field_0x1C;
    /* 0x20 */ int field_0x20;
    /* 0x24 */ u32 field_0x24;
    /* 0x28 */ TextureBuffer *mpTextureBuffer_0x28;
    /* 0x2C */ TextureBuffer *mpTextureBuffer_0x2C;
    /* 0x30 */ TextureBuffer *mpTextureBuffer_0x30;
    /* 0x34 */ u32 mOldDrawFlag;
};

} // namespace EGG

#endif
