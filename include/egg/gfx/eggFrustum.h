#ifndef EGG_FRUSTUM_H
#define EGG_FRUSTUM_H

#include "common.h"
#include "nw4r/g3d/g3d_camera.h"

namespace EGG {

// TODO: Fill out more
class Frustum {
public:
    enum CanvasMode {
        CANVAS_0,
        CANVAS_1,
    };
    enum ProjectionType {
        PROJ_ORTHO,
        PROJ_PERSP
    };

    enum Flag {
        FLAG_DIRTY = (1 << 0),
        FLAG_0x40 = 0x40,
    };

private:
    /* 0x00 */ ProjectionType mProjType;
    /* 0x04 */ CanvasMode mCanvasMode;
    /* 0x08 */ nw4r::math::VEC2 mSize;
    /* 0x10 */ f32 mFovY;
    /* 0x14 */ f32 mTanFovY;
    /* 0x18 */ f32 mNearZ;
    /* 0x1C */ f32 mFarZ;
    /* 0x20 */ nw4r::math::VEC2 mOffset;
    /* 0x28 */ nw4r::math::VEC3 mScale;
    /* 0x34 */ mutable u16 mFlags;

public:
    // vt at 0x38
    virtual ~Frustum() {}
    virtual void SetProjectionGX() const;
    virtual void CopyToG3D(nw4r::g3d::Camera) const;

    ProjectionType GetProjectionType() const {
        return mProjType;
    }
    void SetProjectionType(ProjectionType type) {
        mProjType = type;
    }

    void SetDirty(bool dirty) const {
        if (dirty) {
            mFlags |= FLAG_DIRTY;
        } else {
            mFlags &= ~FLAG_DIRTY;
        }
    }

    void SetFlag(Flag f) const {
        mFlags |= f;
    }

    CanvasMode GetCanvasMode() const {
        return mCanvasMode;
    }
    void SetCanvasMode(CanvasMode mode) {
        if (mCanvasMode != mode) {
            SetDirty(true);
            mCanvasMode = mode;
        }
    }

    void SetNearFar(f32 near, f32 far) {
        mNearZ = near;
        mFarZ = far;
    }

    void ResetOrthographic(f32, f32, f32, f32, f32, f32);

    void SetScale(const nw4r::math::VEC3 &scale) {
        mScale = scale;
    }
    void SetOffset(const nw4r::math::VEC2 &offset) {
        mOffset = offset;
    }
};

} // namespace EGG

#endif
