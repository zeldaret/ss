#ifndef NW4R_LYT_TEXMAP_H
#define NW4R_LYT_TEXMAP_H
#include "common.h"
#include <rvl/GX.h>
#include <rvl/TPL/TPL.h>

namespace nw4r {
namespace lyt {
namespace detail {
bool IsCITexelFormat(GXTexFmt fmt) {
    return (fmt == GX_TF_C4 || fmt == GX_TF_C8 || fmt == GX_TF_C14X2);
}
} // namespace detail

class TexMap {
public:
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
        return mImage;
    }
    void SetImage(void *img) {
        mImage = img;
    }

    void *GetPalette() const {
        return mPalette;
    }
    void SetPalette(void *pal) {
        mPalette = pal;
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
        return (GXTexFmt)mTexelFormat;
    }
    void SetTexelFormat(GXTexFmt fmt) {
        mTexelFormat = fmt;
    }

    bool IsMipMap() const {
        return mIsMipMap;
    }
    void SetMipMap(bool b) {
        mIsMipMap = b;
    }

    GXTexWrapMode GetWrapModeS() const {
        return (GXTexWrapMode)mWrapModeS;
    }
    GXTexWrapMode GetWrapModeT() const {
        return (GXTexWrapMode)mWrapModeT;
    }
    void SetWrapMode(GXTexWrapMode wrapS, GXTexWrapMode wrapT) {
        mWrapModeS = wrapS;
        mWrapModeT = wrapT;
    }

    GXTexFilter GetMinFilter() const {
        return (GXTexFilter)mMinFilter;
    }
    GXTexFilter GetMagFilter() const {
        return (GXTexFilter)mMagFilter;
    }
    void SetFilter(GXTexFilter minFlt, GXTexFilter magFlt) {
        mMinFilter = minFlt;
        mMagFilter = magFlt;
    }

    bool IsBiasClampEnable() const {
        return mIsBiasClampEnable;
    }
    void SetBiasClampEnable(bool b) {
        mIsBiasClampEnable = b;
    }

    bool IsEdgeLODEnable() const {
        return mIsEdgeLODEnable;
    }
    void SetEdgeLODEnable(bool b) {
        mIsEdgeLODEnable = b;
    }

    GXAnisotropy GetAnisotropy() const {
        return (GXAnisotropy)mAnisotropy;
    }
    void SetAnisotropy(GXAnisotropy a) {
        mAnisotropy = a;
    }

    GXTlutFmt GetPaletteFormat() const {
        return (GXTlutFmt)mPaletteFormat;
    }
    void SetPaletteFormat(GXTlutFmt fmt) {
        mPaletteFormat = fmt;
    }

private:
    void *mImage;         // at 0x0
    void *mPalette;       // at 0x4
    u16 mWidth;           // at 0x8
    u16 mHeight;          // at 0xA
    f32 mMinLOD;          // at 0xC
    f32 mMaxLOD;          // at 0x10
    u16 mLODBias;         // at 0x14
    u16 mPaletteEntryNum; // at 0x16

    // at 0x18
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
};
} // namespace lyt
} // namespace nw4r

#endif
