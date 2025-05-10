#ifndef EGG_CPU_TEXTURE_H
#define EGG_CPU_TEXTURE_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggTexture.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class CpuTexture {
public:
    enum TexFlag {
        CONFIGURED = 0x1,
        HAS_HEADER = 0x2,
        NEEDS_BUFFER_FREE = 0x4,
    };

    virtual ~CpuTexture();    // at 0x8
    virtual void configure(); // at 0xC

    void getTexObj(GXTexObj *) const; // at 0x10
    void load(GXTexMapID) const;      // at 0x14

    CpuTexture();
    CpuTexture(u16 width, u16 height, GXTexFmt texFmt);
    CpuTexture(const GXTexObj *pObj);

    void checkTexBuffer() const;
    void invalidate() const;
    void flush() const;
    void alloc(Heap *heap);
    void allocWithHeaderDebug(Heap *heap);
    void allocate(Heap *heap);

    u32 getTexBufferSize() const;
    void buildHeader() const;
    ResTIMG *initHeader();

    void fillNormalMapSphere(f32, f32);
    void fillGradient(int op, int, u16, u16, const GXColor &, const GXColor &, bool, bool);

    void setColor(u16 x, u16 y, GXColor color);
    GXColor getColor(u16 x, u16 y) const;

    bool checkIsConfigure() const {
        return mFlags & CONFIGURED;
    }
    bool checkHasHeader() const {
        return mFlags & HAS_HEADER;
    }

    void setFlag(u8 flag) {
        mFlags |= flag;
    }
    void clearFlag(u8 flag) {
        mFlags &= ~flag;
    }

    u16 getWidth() const {
        return mWidth;
    }
    void setWidth(u16 w) {
        mWidth = w;
    }

    u16 getHeight() const {
        return mHeight;
    }
    // Needed for PostEffectMaskDOF::setUpGradation
    u16 getHeight2() {
        return mHeight;
    }
    void setHeight(u16 h) {
        mHeight = h;
    }

    GXTexFmt getFormat() const {
        return (GXTexFmt)mTexFormat;
    }
    void setFormat(GXTexFmt fmt) {
        mTexFormat = fmt;
    }

    void setWrapS(GXTexWrapMode wrap) {
        mWrapS = wrap;
    }
    void setWrapT(GXTexWrapMode wrap) {
        mWrapT = wrap;
    }

    GXTexWrapMode getWrapS() const {
        return (GXTexWrapMode)mWrapS;
    }
    GXTexWrapMode getWrapT() const {
        return (GXTexWrapMode)mWrapT;
    }

    void setWrap(GXTexWrapMode wrapS, GXTexWrapMode wrapT) {
        mWrapS = wrapS;
        mWrapT = wrapT;
    }

    void setFilt(GXTexFilter min, GXTexFilter mag) {
        mMinFilt = min;
        mMagFilt = mag;
    }

    void setMinFilt(GXTexFilter filt) {
        mMinFilt = filt;
    }
    void setMagFilt(GXTexFilter filt) {
        mMagFilt = filt;
    }

    GXTexFilter getMinFilt() const {
        return (GXTexFilter)mMinFilt;
    }
    GXTexFilter getMagFilt() const {
        return (GXTexFilter)mMagFilt;
    }

    ResTIMG *getHeader() const {
        return (ResTIMG *)((u8 *)dataPtr - sizeof(ResTIMG));
    }

    void *getBuffer() const {
        return dataPtr;
    }
    void setBuffer(void *pBuffer);

protected:
    u16 mWidth;    // at 0x4
    u16 mHeight;   // at 0x6
    u8 mFlags;     // at 0x8
    u8 mTexFormat; // at 0x9
    u8 mWrapS;     // at 0xA
    u8 mWrapT;     // at 0xB
    u8 mMinFilt;   // at 0xC
    u8 mMagFilt;   // at 0xD
    u8 UNK_0xE[2];
    void *dataPtr;  // at 0x10
    char *mpBuffer; // at 0x14
};

} // namespace EGG

#endif
