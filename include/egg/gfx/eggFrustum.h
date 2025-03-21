#ifndef EGG_FRUSTUM_H
#define EGG_FRUSTUM_H

#include "nw4r/g3d/g3d_camera.h"

namespace EGG {
class Frustum {
public:
    enum Flag {
        FLAG_DIRTY = (1 << 0),
        FLAG_0x02 = (1 << 1),
        FLAG_0x04 = (1 << 2),
        FLAG_0x08 = (1 << 3),
        FLAG_0x20 = (1 << 5),
        FLAG_0x40 = (1 << 6),
    };

    enum ProjectionType {
        PROJ_ORTHO,
        PROJ_PERSP
    };

    enum CanvasMode {
        CANVASMODE_0,
        CANVASMODE_1,
    };

protected:
    ProjectionType mProjType; // at 0x0
    CanvasMode mCanvasMode;   // at 0x4
    nw4r::math::VEC2 mSize;   // at 0x8
    f32 mFovY;                // at 0x10
    f32 mTanFovY;             // at 0x14
    f32 mNearZ;               // at 0x18
    f32 mFarZ;                // at 0x1C
    nw4r::math::VEC2 mOffset; // at 0x20
    nw4r::math::VEC3 mScale;  // at 0x28
    mutable u16 mFlags;       // at 0x34

public:
    Frustum(ProjectionType, const nw4r::math::VEC2 &, f32, f32, CanvasMode);
    Frustum(const Frustum &);

    virtual ~Frustum() {}                            // at 0x8
    virtual void SetProjectionGX() const;            // at 0xC
    virtual void CopyToG3D(nw4r::g3d::Camera) const; // at 0x10
    void ResetOrthographic(f32, f32, f32, f32, f32, f32);
    void CopyFromAnother(const Frustum &);
    void SetFovy(f32);
    void ConvertToCanvasLU(f32, f32, f32 *, f32 *) const;

    static void SetGlobalScaleOffset(f32, f32, f32, f32);
    static void GetGlobalScaleOffset(f32 *, f32 *, f32 *, f32 *);

    f32 GetScreenSizeToViewSize(f32, f32) const;

    void SetDirty(bool dirty) const {
        if (dirty) {
            mFlags |= FLAG_DIRTY;
        } else {
            mFlags &= ~FLAG_DIRTY;
        }
    }

    ProjectionType GetProjectionType() const {
        return mProjType;
    }
    void SetProjectionType(ProjectionType type) {
        mProjType = type;
    }

    CanvasMode GetCanvasMode() const {
        return mCanvasMode;
    }
    void SetCanvasMode(CanvasMode mode) {
        if (mode != mCanvasMode) {
            SetDirty(true);
            mCanvasMode = mode;
        }
    }

    const nw4r::math::VEC2 &GetSize() const {
        return mSize;
    }

    u16 GetWidth() const {
        return mSize.x;
    }
    f32 GetSizeX() const {
        return mSize.x;
    }
    void SetSizeX(f32 sizeX) {
        SetDirty(true);
        mSize.x = sizeX;
    }

    u16 GetHeight() const {
        return mSize.y;
    }
    f32 GetSizeY() const {
        return mSize.y;
    }
    void SetSizeY(f32 sizeY) {
        SetDirty(true);
        mSize.y = sizeY;
    }

    void SetSize(const nw4r::math::VEC2 &size) {
        SetSizeX(size.x);
        SetSizeY(size.y);
    }

    void SetNearZ(f32 nearZ) {
        mNearZ = nearZ;
    }
    f32 GetNearZ() const {
        return mNearZ;
    }

    void SetFarZ(f32 farZ) {
        mFarZ = farZ;
    }
    f32 GetFarZ() const {
        return mFarZ;
    }

    void SetNearFar(f32 near, f32 far) {
        mNearZ = near;
        mFarZ = far;
    }

    void SetScale(const nw4r::math::VEC3 &scale) {
        mScale = scale;
    }
    void SetOffset(const nw4r::math::VEC2 &offset) {
        mOffset = offset;
    }

    void SetFlag(u32 flag) {
        mFlags |= flag;
    }

private:
    void SetProjectionPerspectiveGX_() const;
    void SetProjectionOrthographicGX_() const;

    void CopyToG3D_Perspective_(nw4r::g3d::Camera) const;
    void CopyToG3D_Orthographic_(nw4r::g3d::Camera) const;

    void CalcMtxPerspective_(nw4r::math::MTX44 *) const;

    void GetOrthographicParam_(f32 *) const;
    void GetPerspectiveParam_(f32 *) const;
    void GetOrthographicParam_(f32 *, f32 *, f32 *, f32 *) const;

private:
    static nw4r::math::VEC2 sGlobalScale;
    static nw4r::math::VEC2 sGlobalOffset;
};
} // namespace EGG

#endif
