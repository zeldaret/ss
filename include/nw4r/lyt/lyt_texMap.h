#ifndef NW4R_LYT_TEXMAP_H
#define NW4R_LYT_TEXMAP_H
#include "common.h"
#include <rvl/GX.h>
#include <rvl/TPL/TPL.h>

namespace nw4r {
namespace lyt {
namespace detail {
inline bool IsCITexelFormat(GXTexFmt fmt) {
    return (fmt == GX_TF_C4 || fmt == GX_TF_C8 || fmt == GX_TF_C14X2);
}
} // namespace detail

class TexMap {
public:
    TexMap() {}

    void Get(GXTexObj *) const;
    void Get(GXTlutObj *) const;

    void Set(const TexMap &t) {
        *this = t;
    }
    void Set(TPLPalette *, u32);
    void Set(const TPLDescriptor *);

    void SetNoWrap(const TexMap &);
    void SetNoWrap(const TPLDescriptor *);

    void *GetImage() const {
        return mpImage;
    }
    void SetImage(void *img) {
        mpImage = img;
    }

    void *GetPalette() const {
        return mpPalette;
    }
    void SetPalette(void *pal) {
        mpPalette = pal;
    }

    u16 GetWidth() const {
        return mWidth;
    }
    u16 GetHeight() const {
        return mHeight;
    }
    void SetSize(u16 w, u16 h) {
        mWidth = w;
        mHeight = h;
    }

    f32 GetMinLOD() const {
        return mMinLOD;
    }
    f32 GetMaxLOD() const {
        return mMaxLOD;
    }
    void SetLOD(f32 min, f32 max) {
        mMinLOD = min;
        mMaxLOD = max;
    }

    f32 GetLODBias() const {
        return mLODBias / 256.0f;
    }
    void SetLODBias(f32 val) {
        mLODBias = u16(val * 256.0f);
    }

    u16 GetPaletteEntryNum() const {
        return mPaletteEntryNum;
    }
    void SetPaletteEntryNum(u16 num) {
        mPaletteEntryNum = num;
    }

    GXTexFmt GetTexelFormat() const {
        return (GXTexFmt)mBits.mTexelFormat;
    }
    void SetTexelFormat(GXTexFmt fmt) {
        mBits.mTexelFormat = fmt;
    }

    bool IsMipMap() const {
        return mBits.mIsMipMap;
    }
    void SetMipMap(bool b) {
        mBits.mIsMipMap = b;
    }

    GXTexWrapMode GetWrapModeS() const {
        return (GXTexWrapMode)mBits.mWrapModeS;
    }
    GXTexWrapMode GetWrapModeT() const {
        return (GXTexWrapMode)mBits.mWrapModeT;
    }
    void SetWrapMode(GXTexWrapMode wrapS, GXTexWrapMode wrapT) {
        mBits.mWrapModeS = wrapS;
        mBits.mWrapModeT = wrapT;
    }

    GXTexFilter GetMinFilter() const {
        return (GXTexFilter)mBits.mMinFilter;
    }
    GXTexFilter GetMagFilter() const {
        return (GXTexFilter)mBits.mMagFilter;
    }
    void SetFilter(GXTexFilter minFlt, GXTexFilter magFlt) {
        mBits.mMinFilter = minFlt;
        mBits.mMagFilter = magFlt;
    }

    bool IsBiasClampEnable() const {
        return mBits.mIsBiasClampEnable;
    }
    void SetBiasClampEnable(bool b) {
        mBits.mIsBiasClampEnable = b;
    }

    bool IsEdgeLODEnable() const {
        return mBits.mIsEdgeLODEnable;
    }
    void SetEdgeLODEnable(bool b) {
        mBits.mIsEdgeLODEnable = b;
    }

    GXAnisotropy GetAnisotropy() const {
        return (GXAnisotropy)mBits.mAnisotropy;
    }
    void SetAnisotropy(GXAnisotropy a) {
        mBits.mAnisotropy = a;
    }

    GXTlutFmt GetPaletteFormat() const {
        return (GXTlutFmt)mBits.mPaletteFormat;
    }
    void SetPaletteFormat(GXTlutFmt fmt) {
        mBits.mPaletteFormat = fmt;
    }

private:
    void *mpImage;        // at 0x00
    void *mpPalette;      // at 0x04
    u16 mWidth;           // at 0x08
    u16 mHeight;          // at 0x0A
    f32 mMinLOD;          // at 0x0C
    f32 mMaxLOD;          // at 0x10
    u16 mLODBias;         // at 0x14
    u16 mPaletteEntryNum; // at 0x16
    struct {
        u32 mTexelFormat : 4;       // GXTexFmt
        u32 mIsMipMap : 1;          // bool
        u32 mWrapModeS : 2;         // GXTexWrapMode
        u32 mWrapModeT : 2;         // GXTexWrapMode
        u32 mMinFilter : 3;         // GXTexFilter
        u32 mMagFilter : 3;         // GXTexFilter
        u32 mIsBiasClampEnable : 1; // bool
        u32 mIsEdgeLODEnable : 1;   // bool
        u32 mAnisotropy : 2;        // GXAnisotropy
        u32 mPaletteFormat : 2;     // GXTlutFmt

    } mBits; // at 0x18
};
} // namespace lyt
} // namespace nw4r

#endif
