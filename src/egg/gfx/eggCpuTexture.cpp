#include "egg/gfx/eggCpuTexture.h"

#include "common.h"
#include "egg/core/eggHeap.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/OS/OSCache.h"

namespace EGG {

CpuTexture::CpuTexture() {
    mWidth = 0;
    mHeight = 0;
    mFlags = 0;
    mTexFormat = GX_TF_RGBA8;
    mWrapS = 0;
    mWrapT = 0;
    mMinFilt = 1;
    mMagFilt = 1;
    dataPtr = nullptr;
    mpBuffer = nullptr;
}

CpuTexture::CpuTexture(u16 width, u16 height, GXTexFmt texFmt) {
    mWidth = width;
    mHeight = height;
    mFlags = 0;
    mTexFormat = texFmt;
    mWrapS = 0;
    mWrapT = 0;
    mMinFilt = 1;
    mMagFilt = 1;
    dataPtr = nullptr;
    mpBuffer = nullptr;
}

CpuTexture::CpuTexture(const GXTexObj *pObj) {
    mWidth = GXGetTexObjWidth(pObj);
    mHeight = GXGetTexObjHeight(pObj);

    mTexFormat = GXGetTexObjFormat(pObj);
    mWrapS = GXGetTexObjWrapS(pObj);
    mWrapT = GXGetTexObjWrapT(pObj);
    mMinFilt = GXGetTexObjMinFilt(pObj);
    mMagFilt = GXGetTexObjMagFilt(pObj);
    // why?
    dataPtr = (char *)GXGetTexObjData(pObj) - 0x80000000;
    mpBuffer = nullptr;
}

CpuTexture::~CpuTexture() {
    if ((mFlags & NEEDS_BUFFER_FREE) != 0) {
        delete[] mpBuffer;
    }
}

void CpuTexture::configure() {
    mFlags = CONFIGURED;
}

void CpuTexture::setBuffer(void *buf) {
    if (buf != nullptr) {
        dataPtr = buf;
    }
    mFlags &= ~HAS_HEADER;
}

void CpuTexture::load(GXTexMapID id) const {
    GXTexObj obj;
    getTexObj(&obj);
    GXLoadTexObj(&obj, id);
}

void CpuTexture::getTexObj(GXTexObj *pObj) const {
    GXInitTexObj(pObj, dataPtr, mWidth, mHeight, getFormat(), (GXTexWrapMode)mWrapS, (GXTexWrapMode)mWrapT, false);
    GXInitTexObjLOD(pObj, (GXTexFilter)mMinFilt, (GXTexFilter)mMagFilt, 0.0f, 0.0f, 0.0f, false, false, GX_ANISO_1);
}

void CpuTexture::flush() const {
    u32 size = GXGetTexBufferSize(mWidth, mHeight, getFormat(), false, true);
    DCFlushRange(dataPtr, size);
}

void CpuTexture::buildHeader() const {
    Header *pHdr = getHeader();

    pHdr->mTexFormat = mTexFormat;
    pHdr->BYTE_0x1 = 1;
    pHdr->mWidth = mWidth;
    pHdr->mHeight = mHeight;
    pHdr->mWrapS = mWrapS;
    pHdr->mWrapT = mWrapT;
    pHdr->BYTE_0x8 = 0;
    pHdr->BYTE_0x9 = 0;
    pHdr->SHORT_0xA = 0;
    pHdr->WORD_0xC = 0;
    pHdr->BYTE_0xD = 0;
    pHdr->BYTE_0xE = 0;
    pHdr->BYTE_0xF = 0;
    pHdr->BYTE_0x10 = 0;
    pHdr->mMinFilt = mMinFilt;
    pHdr->mMagFilt = mMagFilt;
    pHdr->BYTE_0x13 = 0;
    pHdr->BYTE_0x14 = 0;
    pHdr->BYTE_0x15 = 1;
    pHdr->SHORT_0x16 = 0;
    pHdr->WORD_0x18 = 32;
}

void CpuTexture::alloc(Heap *pHeap) {
    Heap *heap = pHeap == nullptr ? Heap::getCurrentHeap() : pHeap;
    u32 size = GXGetTexBufferSize(mWidth, mHeight, getFormat(), false, true);
    mpBuffer = new (heap, 0x20) char[size];
    setBuffer(mpBuffer);
    mFlags |= NEEDS_BUFFER_FREE;
}

void CpuTexture::allocWithHeaderDebug(Heap *pHeap) {
    Heap *heap = pHeap == nullptr ? Heap::getCurrentHeap() : pHeap;
    u32 size = GXGetTexBufferSize(mWidth, mHeight, getFormat(), false, true);
    mpBuffer = new (heap, 0x20) char[size + 0x20];
    char *actualBuf = mpBuffer + 0x20;
    setBuffer(mpBuffer + 0x20);
    if (actualBuf != nullptr) {
        mFlags |= HAS_HEADER;
    }
    mFlags |= NEEDS_BUFFER_FREE;
    buildHeader();
}

void CpuTexture::allocate(Heap *pHeap) {
    alloc(pHeap);
}

void CpuTexture::setColor(u16 x, u16 y, GXColor color) {
    // NONMATCHING
    switch (getFormat()) {
        case GX_TF_RGBA8:
        case GX_TF_Z24X8: {
            int offset = ((x >> 2) + (y >> 2) * (getWidth() / 4)) * 0x40 + (y & 3) * 8 + (x & 3) * 2;
            u8 *dat = static_cast<u8 *>(getBuffer()) + offset;
            dat[0] = color.a;
            dat[1] = color.r;
            dat[0x20] = color.g;
            dat[0x21] = color.b;
            break;
        }
        case GX_TF_I8:
        case GX_TF_Z8: {
            // TODO
            int offset = ((x >> 3) + (y >> 2) * (getWidth() / 8)) * 0x20 + (x & 3) * 8 + (x & 7);
            u8 *dat = static_cast<u8 *>(getBuffer()) + offset;
            dat[0] = color.r;
            break;
        }

        case GX_TF_IA8:
        case GX_TF_Z16: {
            int offset = ((x >> 2) + (y >> 2) * (getWidth() / 4)) * 0x20 + (y & 3) * 8 + (x & 3) * 2;
            u8 *dat = static_cast<u8 *>(getBuffer()) + offset;
            dat[0] = color.a;
            dat[1] = color.r;
            break;
        }
        default: break;
    }
}

GXColor CpuTexture::getColor(u16 x, u16 y) const {
    // NONMATCHING
    GXColor c;
    switch (getFormat()) {
        case GX_TF_RGBA8:
        case GX_TF_Z24X8: {
            int offset = ((x >> 2) + (y >> 2) * (getWidth() / 4)) * 0x40 + (y & 3) * 8 + (x & 3) * 2;
            u8 *dat = static_cast<u8 *>(getBuffer()) + offset;
            c.r = dat[1];
            c.g = dat[0x20];
            c.b = dat[0x21];
            c.a = dat[0];
            break;
        }
        case GX_TF_I8:
        case GX_TF_Z8: {
            // TODO
            int offset = ((x >> 3) + (y >> 2) * (getWidth() / 8)) * 0x20 + (x & 3) * 8 + (x & 7);
            u8 *dat = static_cast<u8 *>(getBuffer()) + offset;
            c.a = dat[0];
            c.b = dat[0];
            c.g = dat[0];
            c.r = dat[0];
            break;
        }

        case GX_TF_IA8:
        case GX_TF_Z16: {
            int offset = ((x >> 2) + (y >> 2) * (getWidth() / 4)) * 0x20 + (y & 3) * 8 + (x & 3) * 2;
            u8 *dat = static_cast<u8 *>(getBuffer()) + offset;
            // TODO
            c.a = dat[0];
            c.b = dat[1];
            c.g = dat[1];
            c.r = dat[1];
            break;
        }
        default: break;
    }
    return c;
}

} // namespace EGG
