#include "egg/gfx/eggCpuTexture.h"

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggGXUtility.h"
#include "egg/gfx/eggTexture.h"
#include "egg/math/eggVector.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/vec.h"
#include "rvl/OS/OSCache.h"

namespace EGG {

namespace {

void blendColor(GXColor &outColor, const GXColor &c1, const GXColor &c2, f32 amount) {
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

void makeGradient(int op, GXColor *outColor, u16 size, u16 start, u16 end, const GXColor &c1, const GXColor &c2) {
    for (int i = 0; i < start + 1; i++) {
        outColor[i] = c1;
    }

    for (int i = end - 1; i < size; i++) {
        outColor[i] = c2;
    }

    for (int i = start + 1; i < end - 1; i++) {
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
        blendColor(outColor[i], c1, c2, ratio);
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

    mTexFormat = GXGetTexObjFmt(pObj);
    mWrapS = GXGetTexObjWrapS(pObj);
    mWrapT = GXGetTexObjWrapT(pObj);
    mMinFilt = GXGetTexObjMinFilt(pObj);
    mMagFilt = GXGetTexObjMagFilt(pObj);
    // Convert Physical to Virtual Address
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
    size_t size = GXGetTexBufferSize(mWidth, mHeight, getFormat(), false, true);
    DCFlushRange(dataPtr, size);
}

void CpuTexture::buildHeader() const {
    ResTIMG *pHdr = getHeader();

    pHdr->format = mTexFormat;
    pHdr->alphaEnabled = 1;
    pHdr->width = mWidth;
    pHdr->height = mHeight;
    pHdr->wrapS = mWrapS;
    pHdr->wrapT = mWrapT;
    pHdr->indexTexture = 0;
    pHdr->colorFormat = 0;
    pHdr->numColors = 0;
    pHdr->paletteOffset = 0;
    pHdr->mipmapEnabled = 0;
    pHdr->doEdgeLOD = 0;
    pHdr->biasClamp = 0;
    pHdr->maxAnisotropy = 0;
    pHdr->minFilter = mMinFilt;
    pHdr->magFilter = mMagFilt;
    pHdr->minLOD = 0;
    pHdr->maxLOD = 0;
    pHdr->mipmapCount = 1;
    pHdr->LODBias = 0;
    pHdr->imageOffset = sizeof(ResTIMG);
}

extern "C" double sqrt(double);

void CpuTexture::fillNormalMapSphere(f32 f1, f32 f2) {
    f32 mid = static_cast<f32>(mWidth / 2);

    for (u16 y = 0; y < mHeight; y++) {
        for (u16 x = 0; x < mWidth; x++) {
            Vector3f vec;
            GXColor c;

            f32 fx = f1 * ((x + f2) - mid);
            f32 fy = f1 * -((y + f2) - mid);

            f32 mid2 = mid * mid;
            f32 fz = mid2 - (fx * fx + fy * fy);

            vec(0) = fx;
            vec(1) = fy;
            // TODO: std::sqrtf?
            vec(2) = fz < Math<f32>::zero() ? Math<f32>::zero() : (f32)sqrt(fz);

            PSVECNormalize(vec, vec);
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

    bool swapMode = unk == 0x73 || unk == 0x53;
    u16 width = swapMode ? mWidth : mHeight;
    u16 height = swapMode ? mHeight : mWidth;

    u16 mid = width / 2;

    makeGradient(op, gradient, width, start, end, c1, c2);
    if (b1) {
        for (int i = 0; i < width; i++) {
            colors[i] = gradient[i < mid ? i + mid : i - mid];
        }

        for (int i = 0; i < width; i++) {
            gradient[i] = colors[i];
        }
    }

    u16 start2 = b2 ? 0 : start;
    u32 end2 = b2 ? width : end;
    for (u16 x = start2; x < end2; x++) {
        for (u16 y = 0; y < height; y++) {
            if (swapMode) {
                setColor(x, y, gradient[x]);
            } else {
                setColor(y, x, gradient[x]);
            }
        }
    }
    flush();
}

void CpuTexture::alloc(Heap *pHeap) {
    Heap *heap = !pHeap ? Heap::getCurrentHeap() : pHeap;

    size_t size = GXGetTexBufferSize(mWidth, mHeight, getFormat(), false, true);

    mpBuffer = new (heap, 0x20) char[size];
    setBuffer(mpBuffer);

    mFlags |= NEEDS_BUFFER_FREE;
}

void CpuTexture::allocWithHeaderDebug(Heap *pHeap) {
    Heap *heap = !pHeap ? Heap::getCurrentHeap() : pHeap;

    size_t size = GXGetTexBufferSize(mWidth, mHeight, getFormat(), false, true);

    // Textures need 32-bit alignment ( ptr is represented with ptr >> 5 )
    mpBuffer = new (heap, 0x20) char[size + sizeof(ResTIMG)];

    // Texture has Header -> offset to actual image data
    void *imageData = mpBuffer + sizeof(ResTIMG);

    setBuffer(imageData);
    if (imageData != nullptr) {
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
            int pix = (x & 0x3) * 2 + (y & 0x3) * 8;
            int block = (x >> 2) + (y >> 2) * (getWidth() / 4);

            int offset = block * 0x40 + pix;

            u8 *dat = static_cast<u8 *>(getBuffer()) + offset;

            dat[0] = color.a;
            dat[1] = color.r;
            dat[0x20] = color.g;
            dat[0x21] = color.b;
            break;
        }
        case GX_TF_I8:
        case GX_TF_Z8: {
            int offset = (x & 0x7) + (y & 0x3) * 8;                    // pixel
            offset += ((x >> 3) + ((y >> 2) * (getWidth() / 8))) * 32; // block

            u8 *dat = static_cast<u8 *>(getBuffer()) + offset;
            dat[0] = color.r;

            break;
        }

        case GX_TF_IA8:
        case GX_TF_Z16: {
            int block = (x >> 2) + (y >> 2) * (getWidth() / 4);
            int pix = (x & 0x3) * 2 + (y & 0x3) * 8;

            int offset = block * 0x20 + pix;

            u8 *dat = static_cast<u8 *>(getBuffer()) + offset;

            dat[0] = color.a;
            dat[1] = color.r;
            break;
        }
        default: break;
    }
}

GXColor CpuTexture::getColor(u16 x, u16 y) const {
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
            // TODO - probably fake
            int idx = (x & 0x7);
            int pixel_idx = (y & 0x3) * 8;
            int block_idx = ((getWidth() / 8) * (y >> 2) + (x >> 3));
            idx = idx + pixel_idx + block_idx * 0x20;
            u8 *dat = static_cast<u8 *>(getBuffer()) + idx;
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
} // namespace EGG
