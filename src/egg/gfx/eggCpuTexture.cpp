#include "egg/gfx/eggCpuTexture.h"

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggGXUtility.h"
#include "egg/math/eggVector.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/vec.h"
#include "rvl/OS/OSCache.h"

namespace EGG {

namespace {

void calcColorGradient(GXColor &outColor, const GXColor &c1, const GXColor &c2, f32 amount) {
    if (amount <= 0.0f) {
        outColor = c1;
    } else if (amount >= 1.0f) {
        outColor = c2;
    } else {
        outColor.r = c1.r + amount * (c2.r - c1.r);
        outColor.g = c1.g + amount * (c2.g - c1.g);
        outColor.b = c1.b + amount * (c2.b - c1.b);
        outColor.a = c1.a + amount * (c2.a - c1.a);
    }
}

void makeGradient(int op, GXColor *outColor, int size, int start, int end, const GXColor &c1, const GXColor &c2) {
    int start_p1 = start + 1;
    // Weird instruction calculation
    for (int i = 0; i < start_p1; i++) {
        outColor[i] = c1;
    }

    int end_1 = end - 1;
    for (int i = end_1; i < size; i++) {
        outColor[i] = c2;
    }

    for (int i = start_p1; i < end_1; i++) {
        f32 ratio = (f32)(i - start) / (f32)(end - start);
        switch (op) {
            case 0: break;
            case 1: ratio *= ratio; break;
            case 2:
                ratio *= ratio;
                ratio *= ratio;
                break;
            case 3:
                ratio *= ratio;
                ratio *= ratio;
                ratio *= ratio;
                break;
            case 4:
                ratio *= ratio;
                ratio *= ratio;
                ratio *= ratio;
                ratio *= ratio;
                break;
            case 5: ratio = -(ratio - 1.0f) * (ratio - 1.0f) + 1.0f; break;
            case 6:
                ratio -= 1.0f;
                ratio = -ratio * ratio * ratio * ratio + 1.0f;
                break;
        }
        calcColorGradient(outColor[i], c1, c2, ratio);
    }
}

} // namespace

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
            c.a = dat[0];
            c.b = dat[1];
            c.g = c.b;
            c.r = c.b;
            break;
        }
        default: break;
    }
    return c;
}

extern "C" double sqrt(double);

void CpuTexture::fillNormalMapSphere(f32 f1, f32 f2) {
    // NONMATCHING
    // fpr regswaps
    f32 mid = static_cast<f32>(mWidth / 2);
    for (u16 y = 0; y < mHeight; y++) {
        for (u16 x = 0; x < mWidth; x++) {
            Vector3f vec;
            vec.x = f1 * ((x + f2) - mid);
            vec.y = f1 * -((y + f2) - mid);
            f32 z = mid * mid - (vec.x * vec.x + vec.y * vec.y);
            vec.z = z < 0.0f ? 0.0f : (f32)sqrt(z);
            PSVECNormalize(vec, vec);
            GXColor c;
            GXUtility::getNormalColor(c, vec);
            c.r = c.a;
            setColor(x, y, c);
        }
    }
    flush();
}

void CpuTexture::fillGradient(
    int op, int unk, u16 start, u16 end, const GXColor &c1, const GXColor &c2, bool b1, bool b2
) {
    GXColor gradient[1024];
    GXColor colors[256];


    // NONMATCHING - regswaps
    bool swapMode = unk == 0x73 || unk == 0x53;

    u16 width = swapMode ? mWidth : mHeight;
    int size = width;
    u16 height = swapMode ? mHeight : mWidth;
    int mid = width / 2;
    makeGradient(op, gradient, size, start, end, c1, c2);
    if (b1) {
        int i2 = mid;
        for (int i = 0; i < size; i++, i2++) {
            colors[i].r = gradient[i < mid ? i2 : i - mid].r;
            colors[i].g = gradient[i < mid ? i2 : i - mid].g;
            colors[i].b = gradient[i < mid ? i2 : i - mid].b;
            colors[i].a = gradient[i < mid ? i2 : i - mid].a;
        }

        for (int i = 0; i < size; i++) {
            gradient[i] = colors[i];
        }
    }

    u16 start2 = b2 ? 0 : start;
    u16 end2 = b2 ? size : end;

    for (u16 x = start2; x < end2; x++) {
        for (u16 y = 0; y < height; y++) {
            if (swapMode) {
                setColor(x, y, gradient[x & 0xFFFF]);
            } else {
                setColor(y, x, gradient[x & 0xFFFF]);
            }
        }
    }
    flush();
}

} // namespace EGG
