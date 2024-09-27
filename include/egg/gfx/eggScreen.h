#ifndef EGG_SCREEN_H
#define EGG_SCREEN_H

#include <common.h>
#include <egg/gfx/eggFrustum.h>

namespace EGG {

// TODO: Fill out more
class Screen : public Frustum {
public:
    Screen();
    Screen(f32, f32, f32, f32, Screen *, CanvasMode);

    virtual ~Screen() {}
    virtual void SetProjectionGX() const override;
    virtual void CopyToG3D(nw4r::g3d::Camera) const override;

    static void Initialize(const u16 *, const u16 *, Screen *);
    static void SetTVModeDefault();

    u8 TODO_0x3C[0x88 - 0x3C];

    enum TVMode { TV_MODE_1, TV_MODE_2, TV_MODE_3, TV_MODE_4, TV_MODE_MAX };

    struct TVModeInfo {
        TVModeInfo() {}
        u16 width;
        u16 height;
        f32 w_ratio;
        f32 h_ratio;
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

        s32 sc_ox; // at 0x18
        s32 sc_oy; // at 0x1C
    };

    static u16 GetSizeXMax(TVMode mode) { return sTVModeInfo[mode].width; }
    static u16 GetSizeYMax(TVMode mode) { return sTVModeInfo[mode].height; }
    static u16 GetSizeXMax() { return sTVModeInfo[sTVMode].width; }
    static u16 GetSizeYMax() { return sTVModeInfo[sTVMode].height; }

    static inline f32 GetAdjustScale() { return (f32)sTVModeInfo[0].width / sTVModeInfo[Screen::sTVMode].width; }

    static TVMode GetTVMode() { return sTVMode; }

    static TVMode sTVMode;

    static Screen *spRoot;

    typedef void (*ChangeTVModeFunc)(void *);
    static ChangeTVModeFunc sChangeTVModeFunc;
    static void *spChangeTVModeFuncInfo;
    static TVModeInfo sTVModeInfo[Screen::TV_MODE_MAX];

    static nw4r::math::VEC2 sCanvasScale;
    static nw4r::math::VEC2 sCanvasOffset;
};

} // namespace EGG

#endif
