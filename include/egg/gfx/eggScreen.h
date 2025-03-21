#ifndef EGG_SCREEN_H
#define EGG_SCREEN_H

#include "egg/egg_types.h"
#include "egg/gfx/eggFrustum.h"
#include "egg/gfx/eggStateGX.h"

namespace EGG {

class Screen : public Frustum {
public:
    enum TVMode {
        TV_MODE_4_3,
        TV_MODE_16_9,
        TV_MODE_UNK_3,
        TV_MODE_MAX
    };

    typedef void (*ChangeTVModeFunc)(void *);

    struct UnkStruct {
        UNKWORD field_0x00;
        UNKWORD field_0x04;
    };

    struct DataEfb {
        // Required for struct copy
        struct Viewport {
            f32 x1; // at 0x0
            f32 y1; // at 0x4
            f32 x2; // at 0x8
            f32 y2; // at 0xC
            f32 z1; // at 0x10
            f32 z2; // at 0x14
        } vp;

        u32 sc_x;
        u32 sc_y;
        u32 sc_w;
        u32 sc_h;

        s32 sc_ox; // at 0x28
        s32 sc_oy; // at 0x2C
    };

    struct TVModeInfo {
        u16 width;
        u16 height;
        nw4r::math::VEC2 ratios;
    };

public:
    static void Initialize(const u16 *, const u16 *, Screen *);
    static void SetTVMode(TVMode);
    static void SetTVModeDefault();

    static u16 GetSizeXMax(TVMode mode) {
        return sTVModeInfo[mode].width;
    }
    static u16 GetSizeYMax(TVMode mode) {
        return sTVModeInfo[mode].height;
    }
    static u16 GetSizeXMax() {
        return sTVModeInfo[sTVMode].width;
    }
    static u16 GetSizeYMax() {
        return sTVModeInfo[sTVMode].height;
    }

    static inline f32 GetAdjustScale() {
        return (f32)sTVModeInfo[0].width / sTVModeInfo[Screen::sTVMode].width;
    }

    static TVMode GetTVMode() {
        return sTVMode;
    }

    TVMode GetComputedTVMode() const {
        if ((mFlags & FLAG_0x20) == 0) {
            return (mFlags & FLAG_0x08) == 0 ? sTVMode : TV_MODE_UNK_3;
        } else {
            return TV_MODE_4_3;
        }
    }

    Screen();
    Screen(f32, f32, f32, f32, const Screen *, CanvasMode);
    Screen(const Screen &);

    virtual ~Screen() {}                             // at 0x8
    virtual void SetProjectionGX() const;            // at 0xC
    virtual void CopyToG3D(nw4r::g3d::Camera) const; // at 0x10

    const Screen *GetParent() const {
        return mParent;
    }
    void SetParent(const Screen *parent);
    void SetUnkFlag8();

    void CopyFromAnother(const Screen &other);
    void Reset(f32, f32, f32, f32, CanvasMode);
    void OnDirectEfb() const;
    const DataEfb &GetDataEfb() const;
    bool IsChangeEfb() const;
    void CalcMatrixForDrawQuad(nw4r::math::MTX34 *, f32, f32, f32, f32) const;
    void FillBufferGX(u32, GXColor, u32) const;
    void GetGlobalPos(f32 *, f32 *) const;
    void fn_804B2EE0(f32 *, f32 *, f32, f32) const;

    f32 ScaleByX(f32 val) const {
        return val * StateGX::s_widthEfb / GetSizeXMax(GetComputedTVMode());
    }

    f32 ScaleByY(f32 val) const {
        return val * StateGX::s_heightEfb / GetSizeYMax(GetComputedTVMode());
    }

private:
    const Screen *mParent;      // at 0x3C
    nw4r::math::VEC2 mPosition; // at 0x40
    nw4r::math::VEC2 field_0x48; // at 0x48
    nw4r::math::VEC2 field_0x50; // at 0x50
    mutable DataEfb mDataEfb; // at 0x58

    static TVMode sTVMode;

    static Screen *spRoot;

    static ChangeTVModeFunc sChangeTVModeFunc;
    static void *spChangeTVModeFuncInfo;
    static TVModeInfo sTVModeInfo[Screen::TV_MODE_MAX];

    static nw4r::math::VEC2 sCanvasScale;
    static nw4r::math::VEC2 sCanvasOffset;
};

} // namespace EGG

#endif
