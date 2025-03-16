#ifndef EGG_CPU_TEXTURE_H
#define EGG_CPU_TEXTURE_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class CpuTexture {
    void makeGradient(UNKTYPE);

public:
    struct Header {
        u8 mTexFormat;
        u8 BYTE_0x1;
        u16 mWidth;
        u16 mHeight;
        u8 mWrapS;
        u8 mWrapT;
        u8 BYTE_0x8;
        u8 BYTE_0x9;
        u16 SHORT_0xA;
        u32 WORD_0xC;
        u8 BYTE_0xD;
        u8 BYTE_0xE;
        u8 BYTE_0xF;
        u8 BYTE_0x10;
        u8 mMinFilt;
        u8 mMagFilt;
        u8 BYTE_0x13;
        u8 BYTE_0x14;
        u8 BYTE_0x15;
        u16 SHORT_0x16;
        u32 WORD_0x18;
    };

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
    Header *initHeader();
    void fillNormalMapSphere(f32, f32);
    void fillGradient(int op, int, u16, u16, const GXColor&, const GXColor&, bool, bool);
    UNKTYPE func_80086C8C(UNKTYPE);
    void allocTexBuffer();
    void allocTexBufferAndHeader();
    void setColor(u16, u16, GXColor);

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

    Header *getHeader() const {
        return (Header *)((u8 *)dataPtr - sizeof(Header));
    }

    char *getBuffer() const {
        return mpBuffer;
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
