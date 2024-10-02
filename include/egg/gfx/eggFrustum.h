#ifndef EGG_FRUSTUM_H
#define EGG_FRUSTUM_H

#include <common.h>
#include <nw4r/g3d/g3d_camera.h>

namespace EGG {

// TODO: Fill out more
class Frustum {
public:
    enum CanvasMode {
    };
    enum ProjectionType {
        PROJ_ORTHO,
        PROJ_PERSP
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
