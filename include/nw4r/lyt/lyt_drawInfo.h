#ifndef NW4R_LYT_DRAWINFO_H
#define NW4R_LYT_DRAWINFO_H
#include "common.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/math/math_types.h"
#include "nw4r/ut/ut_Rect.h"

namespace nw4r {
namespace lyt {
struct DrawInfo {
    virtual ~DrawInfo();
    DrawInfo();

    bool isYAxisUp() const {
        // NYI
        return false;
    }
    // field accessors
    const math::VEC2 &GetLocationAdjustScale() const {
        return mScale;
    }
    void SetLocationAdjustScale(const math::VEC2 &scale) {
        this->mScale = scale;
    }

    const math::MTX34 &GetViewMtx() const {
        return mViewMtx;
    }
    void SetViewMtx(const math::MTX34 &value) {
        this->mViewMtx = value;
    }

    f32 GetGlobalAlpha() const {
        return mGlobalAlpha;
    }
    void SetGlobalAlpha(f32 alpha) {
        this->mGlobalAlpha = alpha;
    }

    const ut::Rect &GetViewRect() const {
        return this->mViewRect;
    }
    void SetViewRect(const ut::Rect &value) {
        this->mViewRect = value;
    }

    // mFlags accessors
    bool IsInvisiblePaneCalculateMtx() const {
        return mFlags.invisiblePaneCalculateMtx;
    }
    void SetInvisiblePaneCalculateMtx(bool bEnable) {
        this->mFlags.invisiblePaneCalculateMtx = bEnable;
    }

    bool IsLocationAdjust() const {
        return mFlags.locationAdjust;
    }
    void SetLocationAdjust(bool bEnable) {
        this->mFlags.locationAdjust = bEnable;
    }

    bool IsMultipleViewMtxOnDraw() const {
        return mFlags.mulViewDraw;
    }
    void SetMultipleViewMtxOnDraw(bool bEnable) {
        this->mFlags.mulViewDraw = bEnable;
    }

    bool IsInfluencedAlpha() const {
        return mFlags.influencedAlpha;
    }
    void SetInfluencedAlpha(bool bEnable) {
        this->mFlags.influencedAlpha = bEnable;
    }

    bool IsDebugDrawMode() const {
        return mFlags.debugDrawMode;
    }
    void SetDebugDrawMode(bool bEnable) {
        this->mFlags.debugDrawMode = bEnable;
    }

private:
    math::MTX34 mViewMtx; // at 0x0
    ut::Rect mViewRect;   // at 0x34
    math::VEC2 mScale;    // at 0x44
    f32 mGlobalAlpha;     // at 0x48
    struct {
        u8 mulViewDraw : 1;
        u8 influencedAlpha : 1;
        u8 locationAdjust : 1;
        u8 invisiblePaneCalculateMtx : 1;
        u8 debugDrawMode : 1;
    } mFlags; // at 0x50
};
} // namespace lyt
} // namespace nw4r

#endif
