#ifndef EGG_TEXTURE_H
#define EGG_TEXTURE_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggPalette.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"
namespace EGG {

// Copy of ResTIMG from JUTTexture
struct ResTIMG {
    /* 0x00 */ u8 format;
    /* 0x01 */ u8 alphaEnabled;
    /* 0x02 */ u16 width;
    /* 0x04 */ u16 height;
    /* 0x06 */ u8 wrapS;
    /* 0x07 */ u8 wrapT;
    /* 0x08 */ u8 indexTexture;
    /* 0x09 */ u8 colorFormat;
    /* 0x0A */ u16 numColors;
    /* 0x0C */ u32 paletteOffset;
    /* 0x10 */ u8 mipmapEnabled;
    /* 0x11 */ u8 doEdgeLOD;
    /* 0x12 */ u8 biasClamp;
    /* 0x13 */ u8 maxAnisotropy;
    /* 0x14 */ u8 minFilter;
    /* 0x15 */ u8 magFilter;
    /* 0x16 */ s8 minLOD;
    /* 0x17 */ s8 maxLOD;
    /* 0x18 */ u8 mipmapCount;
    /* 0x19 */ u8 unknown;
    /* 0x1A */ s16 LODBias;
    /* 0x1C */ u32 imageOffset;
};

class Texture {
public:
    Texture() {
        setCaptureFlag(false);
        mEmbPalette = NULL;
        mTexInfo = NULL;
    }

    Texture(const ResTIMG *p_timg, u8 param_1) {
        mEmbPalette = NULL;
        storeTIMG(p_timg, param_1);
        setCaptureFlag(false);
    }

    Texture(int, int, GXTexFmt, Heap *);
    ~Texture();

    void storeTIMG(ResTIMG const *pTIMG, u8 param0);
    void storeTIMG(ResTIMG const *pTIMG, Palette *pPalette);
    void storeTIMG(ResTIMG const *pTIMG, Palette *pPalette, GXTlut tlut);
    void attachPalette(Palette *pPalette);
    void init();
    void initTexObj();
    void initTexObj(GXTlut tlut);
    void load(GXTexMapID texMapId);
    void capture(int, int, GXTexFmt, bool, u8);
    void captureTexture(int, int, int, int, bool, GXTexFmt, GXTexFmt);
    void captureDolTexture(int, int, int, int, bool, GXTexFmt);

    const ResTIMG *getTexInfo() const {
        return mTexInfo;
    }
    u8 getFormat() const {
        return mTexInfo->format;
    }
    s32 getTransparency() {
        return mTexInfo->alphaEnabled;
    }
    s32 getWidth() const {
        return mTexInfo->width;
    }
    s32 getHeight() const {
        return mTexInfo->height;
    }
    void setCaptureFlag(bool flag) {
        mFlags &= 2 | flag;
    }
    u8 getCaptureFlag() const {
        return mFlags & 1;
    }
    u8 getEmbPaletteDelFlag() const {
        return mFlags & 2;
    }
    void setEmbPaletteDelFlag(bool flag) {
        mFlags = (mFlags & 1) | (flag << 1);
    }
    bool operator==(const Texture &other) {
        return mTexInfo == other.mTexInfo && mpPalette == other.mpPalette && mWrapS == other.mWrapS &&
               mWrapT == other.mWrapT && mMinFilter == other.mMinFilter && mMagFilter == other.mMagFilter &&
               mMinLOD == other.mMinLOD && mMinLOD == other.mMinLOD && mLODBias == other.mLODBias;
    }
    bool operator!=(const Texture &other) {
        return !operator==(other);
    }

private:
    /* 0x00 */ GXTexObj mTexObj;
    /* 0x20 */ const ResTIMG *mTexInfo;
    /* 0x24 */ void *mTexData;
    /* 0x28 */ Palette *mEmbPalette;
    /* 0x2C */ Palette *mpPalette;
    /* 0x30 */ u8 mWrapS;
    /* 0x31 */ u8 mWrapT;
    /* 0x32 */ u8 mMinFilter;
    /* 0x33 */ u8 mMagFilter;
    /* 0x34 */ u16 mMinLOD;
    /* 0x36 */ u16 mMaxLOD;
    /* 0x38 */ s16 mLODBias;
    /* 0x3A */ u8 mTlutName;
    /* 0x3B */ u8 mFlags;
    /* 0x3C */ void *field_0x3c;
};

} // namespace EGG

#endif
