#include "nw4r/g3d.h" // IWYU pragma: export

#include "nw4r/math.h" // IWYU pragma: export

#include "rvl/GX.h"  // IWYU pragma: export
#include "rvl/MTX.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {

Camera::Camera(CameraData *pData) : ResCommon(pData) {}

void Camera::Init() {
    const GXRenderModeObj *pObj = G3DState::GetRenderModeObj();

    Init(pObj->fbWidth, pObj->efbHeight, pObj->fbWidth, pObj->xfbHeight, pObj->viWidth, pObj->viHeight);
}

void Camera::Init(u16 efbWidth, u16 efbHeight, u16 xfbWidth, u16 xfbHeight, u16 viWidth, u16 viHeight) {
    if (!IsValid()) {
        return;
    }

    CameraData &r = ref();

    r.flags = CameraData::FLAG_CAM_LOOKAT | CameraData::FLAG_PROJ_PERSP;

    r.cameraPos.x = 0.0f;
    r.cameraPos.y = 0.0f;
    r.cameraPos.z = 15.0f;
    r.cameraUp.x = 0.0f;
    r.cameraUp.y = 1.0f;
    r.cameraUp.z = 0.0f;
    r.cameraTarget.x = 0.0f;
    r.cameraTarget.y = 0.0f;
    r.cameraTarget.z = 0.0f;
    r.cameraRotate.x = 0.0f;
    r.cameraRotate.y = 0.0f;
    r.cameraRotate.z = 0.0f;
    r.cameraTwist = 0.0f;

    r.projType = GX_PERSPECTIVE;
    r.projFovy = 60.0f;
    r.projAspect = 4.0f / 3.0f;
    r.projNear = 0.1f;
    r.projFar = 1000.f;
    r.projTop = 0.0f;
    r.projBottom = static_cast<f32>(viHeight);
    r.projLeft = 0.0f;
    r.projRight = static_cast<f32>(viWidth);

    r.lightScaleS = 0.5f;
    r.lightScaleT = 0.5f;
    r.lightTransS = 0.5f;
    r.lightTransT = 0.5f;

    r.viewportOrigin.x = 0.0f;
    r.viewportOrigin.y = 0.0f;
    r.viewportSize.x = static_cast<f32>(xfbWidth);
    r.viewportSize.y = static_cast<f32>(xfbHeight);
    r.viewportNear = 0.0f;
    r.viewportFar = 1.0f;

    r.scissorX = 0;
    r.scissorY = 0;
    r.scissorWidth = efbWidth;
    r.scissorHeight = efbHeight;
    r.scissorOffsetX = 0;
    r.scissorOffsetY = 0;
}

void Camera::SetPosition(f32 x, f32 y, f32 z) {
    if (!IsValid()) {
        return;
    }

    CameraData &r = ref();

    r.cameraPos.x = x;
    r.cameraPos.y = y;
    r.cameraPos.z = z;
    r.flags &= ~CameraData::FLAG_CAM_MTX_READY;
}

void Camera::SetPosition(const math::VEC3 &rPos) {
    if (!IsValid()) {
        return;
    }

    CameraData &r = ref();

    r.cameraPos = rPos;
    r.flags &= ~CameraData::FLAG_CAM_MTX_READY;
}

void Camera::SetPosture(const PostureInfo &rInfo) {
    if (!IsValid()) {
        return;
    }

    CameraData &r = ref();

    switch (rInfo.tp) {
        case POSTURE_LOOKAT: {
            if (r.flags & CameraData::FLAG_CAM_LOOKAT) {
                if (!(rInfo.cameraUp != r.cameraUp) && !(rInfo.cameraTarget != r.cameraTarget)) {
                    return;
                }
            }

            r.flags &= ~(CameraData::FLAG_CAM_LOOKAT | CameraData::FLAG_CAM_ROTATE | CameraData::FLAG_CAM_AIM);

            r.flags |= CameraData::FLAG_CAM_LOOKAT;

            r.cameraUp = rInfo.cameraUp;
            r.cameraTarget = rInfo.cameraTarget;

            r.flags &= ~CameraData::FLAG_CAM_MTX_READY;
            break;
        }

        case POSTURE_ROTATE: {
            if (r.flags & CameraData::FLAG_CAM_ROTATE) {
                if (!(rInfo.cameraRotate != r.cameraRotate)) {
                    return;
                }
            }

            r.flags &= ~(CameraData::FLAG_CAM_LOOKAT | CameraData::FLAG_CAM_ROTATE | CameraData::FLAG_CAM_AIM);

            r.flags |= CameraData::FLAG_CAM_ROTATE;

            r.cameraRotate = rInfo.cameraRotate;

            r.flags &= ~CameraData::FLAG_CAM_MTX_READY;
            break;
        }

        case POSTURE_AIM: {
            if (r.flags & CameraData::FLAG_CAM_AIM) {
                if (!(rInfo.cameraTarget != r.cameraTarget) && rInfo.cameraTwist == r.cameraTwist) {
                    return;
                }
            }

            r.flags &= ~(CameraData::FLAG_CAM_LOOKAT | CameraData::FLAG_CAM_ROTATE | CameraData::FLAG_CAM_AIM);

            r.flags |= CameraData::FLAG_CAM_AIM;

            r.cameraTarget = rInfo.cameraTarget;
            r.cameraTwist = rInfo.cameraTwist;

            r.flags &= ~CameraData::FLAG_CAM_MTX_READY;
            break;
        }

        default: {
            break;
        }
    }
}

void Camera::SetCameraMtxDirectly(const math::MTX34 &rMtx) {
    if (!IsValid()) {
        return;
    }

    CameraData &r = ref();

    math::MTX34Copy(&r.cameraMtx, &rMtx);
    r.flags |= CameraData::FLAG_CAM_MTX_READY;
}

void Camera::SetPerspective(f32 fovy, f32 aspect, f32 near, f32 far) {
    if (!IsValid()) {
        return;
    }

    CameraData &r = ref();

    r.projType = GX_PERSPECTIVE;

    r.projFovy = fovy;
    r.projAspect = aspect;
    r.projNear = near;
    r.projFar = far;

    r.flags &=
        ~(CameraData::FLAG_PROJ_FRUSTUM | CameraData::FLAG_PROJ_PERSP | CameraData::FLAG_PROJ_ORTHO |
          CameraData::FLAG_PROJ_MTX_READY);

    r.flags |= CameraData::FLAG_PROJ_PERSP;
}

void Camera::SetOrtho(f32 top, f32 bottom, f32 left, f32 right, f32 near, f32 far) {
    if (!IsValid()) {
        return;
    }

    CameraData &r = ref();

    r.projType = GX_ORTHOGRAPHIC;

    r.projTop = top;
    r.projBottom = bottom;
    r.projLeft = left;
    r.projRight = right;
    r.projNear = near;
    r.projFar = far;

    r.flags &=
        ~(CameraData::FLAG_PROJ_FRUSTUM | CameraData::FLAG_PROJ_PERSP | CameraData::FLAG_PROJ_ORTHO |
          CameraData::FLAG_PROJ_MTX_READY);

    r.flags |= CameraData::FLAG_PROJ_ORTHO;
}

void Camera::SetProjectionMtxDirectly(const math::MTX44 *pMtx) {
    if (pMtx != NULL && IsValid()) {
        CameraData &r = ref();

        math::MTX44Copy(&r.projMtx, pMtx);
        r.flags |= CameraData::FLAG_PROJ_MTX_READY;
    }
}

void Camera::SetScissor(u32 x, u32 y, u32 width, u32 height) {
    if (!IsValid()) {
        return;
    }

    CameraData &r = ref();

    r.scissorX = x;
    r.scissorY = y;
    r.scissorWidth = width;
    r.scissorHeight = height;
}

void Camera::SetScissorBoxOffset(s32 ox, s32 oy) {
    if (!IsValid()) {
        return;
    }

    CameraData &r = ref();

    r.scissorOffsetX = ox;
    r.scissorOffsetY = oy;
}

void Camera::SetViewport(f32 x, f32 y, f32 width, f32 height) {
    if (!IsValid()) {
        return;
    }

    CameraData &r = ref();

    r.viewportOrigin.x = x;
    r.viewportOrigin.y = y;
    r.viewportSize.x = width;
    r.viewportSize.y = height;

    SetScissor(static_cast<u32>(x), static_cast<u32>(y), static_cast<u32>(width), static_cast<u32>(height));
}

void Camera::SetViewportZRange(f32 near, f32 far) {
    if (!IsValid()) {
        return;
    }

    CameraData &r = ref();

    r.viewportNear = near;
    r.viewportFar = far;
}

void Camera::GetViewport(f32 *pX, f32 *pY, f32 *pWidth, f32 *pHeight, f32 *pNear, f32 *pFar) const {
    if (!IsValid()) {
        return;
    }

    const CameraData &r = ref();

    if (pX != NULL) {
        *pX = r.viewportOrigin.x;
    }
    if (pY != NULL) {
        *pY = r.viewportOrigin.y;
    }

    if (pWidth != NULL) {
        *pWidth = r.viewportSize.x;
    }
    if (pHeight != NULL) {
        *pHeight = r.viewportSize.y;
    }

    if (pNear != NULL) {
        *pNear = r.viewportNear;
    }
    if (pFar != NULL) {
        *pFar = r.viewportFar;
    }
}

void Camera::GetCameraMtx(math::MTX34 *pMtx) const {
    if (pMtx != NULL && IsValid()) {
        const CameraData &r = ref();

        if (!(r.flags & CameraData::FLAG_CAM_MTX_READY)) {
            UpdateCameraMtx();
        }

        math::MTX34Copy(pMtx, &r.cameraMtx);
    }
}

void Camera::GetProjectionMtx(math::MTX44 *pMtx) const {
    if (pMtx != NULL && IsValid()) {
        const CameraData &r = ref();

        if (!(r.flags & CameraData::FLAG_PROJ_MTX_READY)) {
            UpdateProjectionMtx();
        }

        math::MTX44Copy(pMtx, &r.projMtx);
    }
}

void Camera::GetProjectionTexMtx(math::MTX34 *pMtx) const {
    if (pMtx != NULL && IsValid()) {
        const CameraData &r = ref();

        if (r.flags & CameraData::FLAG_PROJ_ORTHO) {
            C_MTXLightOrtho(
                *pMtx, r.projTop, r.projBottom, r.projLeft, r.projRight, r.lightScaleS, -r.lightScaleT, r.lightTransS,
                r.lightTransT
            );
        } else if (r.flags & CameraData::FLAG_PROJ_FRUSTUM) {
            C_MTXLightFrustum(
                *pMtx, r.projTop, r.projBottom, r.projLeft, r.projRight, r.projNear, r.lightScaleS, -r.lightScaleT,
                r.lightTransS, r.lightTransT
            );
        } else /* FLAG_PROJ_PERSP */ {
            C_MTXLightPerspective(
                *pMtx, r.projFovy, r.projAspect, r.lightScaleS, -r.lightScaleT, r.lightTransS, r.lightTransT
            );
        }
    }
}

void Camera::GetEnvironmentTexMtx(math::MTX34 *pMtx) const {
    if (pMtx != NULL && IsValid()) {
        const CameraData &r = ref();

        math::MTX34Identity(pMtx);
        pMtx->_00 = r.lightScaleS;
        pMtx->_03 = r.lightTransS;
        pMtx->_11 = -r.lightScaleT;
        pMtx->_13 = r.lightTransT;
        pMtx->_22 = 0.0f;
        pMtx->_23 = 1.0f;
    }
}

void Camera::GXSetViewport() const {
    if (!IsValid()) {
        return;
    }

    const CameraData &r = ref();
    const GXRenderModeObj *pObj = G3DState::GetRenderModeObj();

    if (pObj->field_rendering) {
        ::GXSetViewportJitter(
            r.viewportOrigin.x, r.viewportOrigin.y, r.viewportSize.x, r.viewportSize.y, r.viewportNear, r.viewportFar,
            r.flags & CameraData::FLAG_VI_ODD_FIELD ? GX_FIELD_ODD : GX_FIELD_EVEN
        );
    } else {
        ::GXSetViewport(
            r.viewportOrigin.x, r.viewportOrigin.y, r.viewportSize.x, r.viewportSize.y, r.viewportNear, r.viewportFar
        );
    }
}

void Camera::GXSetProjection() const {
    if (!IsValid()) {
        return;
    }

    const CameraData &r = ref();

    if (!(r.flags & CameraData::FLAG_PROJ_MTX_READY)) {
        UpdateProjectionMtx();
    }

    ::GXSetProjection(r.projMtx, r.projType);
}

void Camera::GXSetScissor() const {
    if (!IsValid()) {
        return;
    }

    const CameraData &r = ref();
    ::GXSetScissor(r.scissorX, r.scissorY, r.scissorWidth, r.scissorHeight);
}

void Camera::GXSetScissorBoxOffset() const {
    if (!IsValid()) {
        return;
    }

    const CameraData &r = ref();
    ::GXSetScissorBoxOffset(r.scissorOffsetX, r.scissorOffsetY);
}

void Camera::UpdateCameraMtx() const {
    CameraData &r = const_cast<CameraData &>(ref());

    if (r.flags & CameraData::FLAG_CAM_LOOKAT) {
        C_MTXLookAt(r.cameraMtx, r.cameraPos, r.cameraUp, r.cameraTarget);
    } else if (r.flags & CameraData::FLAG_CAM_AIM) {
        math::MTX34 &rMtx = r.cameraMtx;
        math::VEC3 &rPos = r.cameraPos;
        math::VEC3 &rTarget = r.cameraTarget;

        math::VEC3 back(rPos.x - rTarget.x, rPos.y - rTarget.y, rPos.z - rTarget.z);

        if (back.x == 0.0f && back.z == 0.0f) {
            rMtx._00 = 1.0f;
            rMtx._01 = 0.0f;
            rMtx._02 = 0.0f;
            rMtx._03 = -rPos.x;

            rMtx._10 = 0.0f;
            rMtx._11 = 0.0f;
            rMtx._20 = 0.0f;
            rMtx._22 = 0.0f;

            if (back.y <= 0.0f) {
                rMtx._12 = 1.0f;
                rMtx._13 = -rPos.z;
                rMtx._21 = -1.0f;
                rMtx._23 = rPos.y;
            } else {
                rMtx._12 = -1.0f;
                rMtx._13 = rPos.z;
                rMtx._21 = 1.0f;
                rMtx._23 = -rPos.y;
            }
        } else {
            math::VEC3 _r(back.z, 0.0f, -back.x);

            math::VEC3 u;
            math::VEC3Normalize(&back, &back);
            math::VEC3Normalize(&_r, &_r);
            math::VEC3Cross(&u, &back, &_r);

            f32 st, ct;
            math::SinCosDeg(&st, &ct, r.cameraTwist);

            math::VEC3 right, up;
            right.x = st * u.x + ct * _r.x;
            right.y = st * u.y;
            right.z = st * u.z + ct * _r.z;

            up.x = ct * u.x - st * _r.x;
            up.y = ct * u.y;
            up.z = ct * u.z - st * _r.z;

            rMtx._00 = right.x;
            rMtx._01 = right.y;
            rMtx._02 = right.z;
            rMtx._03 = -math::VEC3Dot(&rPos, &right);

            rMtx._10 = up.x;
            rMtx._11 = up.y;
            rMtx._12 = up.z;
            rMtx._13 = -math::VEC3Dot(&rPos, &up);

            rMtx._20 = back.x;
            rMtx._21 = back.y;
            rMtx._22 = back.z;
            rMtx._23 = -math::VEC3Dot(&rPos, &back);
        }
    } else /* FLAG_CAM_ROTATE */ {
        math::MTX34 &rMtx = r.cameraMtx;
        math::VEC3 &rPos = r.cameraPos;

        f32 sx, sy, sz, cx, cy, cz;
        math::SinCosDeg(&sx, &cx, r.cameraRotate.x);
        math::SinCosDeg(&sy, &cy, r.cameraRotate.y);
        math::SinCosDeg(&sz, &cz, r.cameraRotate.z);

        math::VEC3 right, up, back;

        right.x = sx * sy * sz + cy * cz;
        right.y = cx * sz;
        right.z = sx * cy * sz - sy * cz;

        up.x = sx * sy * cz - cy * sz;
        up.y = cx * cz;
        up.z = sx * cy * cz + sy * sz;

        back.x = cx * sy;
        back.y = -sx;
        back.z = cx * cy;

        rMtx._00 = right.x;
        rMtx._01 = right.y;
        rMtx._02 = right.z;
        rMtx._03 = -math::VEC3Dot(&rPos, &right);

        rMtx._10 = up.x;
        rMtx._11 = up.y;
        rMtx._12 = up.z;
        rMtx._13 = -math::VEC3Dot(&rPos, &up);

        rMtx._20 = back.x;
        rMtx._21 = back.y;
        rMtx._22 = back.z;
        rMtx._23 = -math::VEC3Dot(&rPos, &back);
    }

    r.flags |= CameraData::FLAG_CAM_MTX_READY;
}

void Camera::UpdateProjectionMtx() const {
    CameraData &r = const_cast<CameraData &>(ref());

    if (r.flags & CameraData::FLAG_PROJ_ORTHO) {
        C_MTXOrtho(r.projMtx, r.projTop, r.projBottom, r.projLeft, r.projRight, r.projNear, r.projFar);
    } else if (r.flags & CameraData::FLAG_PROJ_FRUSTUM) {
        C_MTXFrustum(r.projMtx, r.projTop, r.projBottom, r.projLeft, r.projRight, r.projNear, r.projFar);
    } else /* FLAG_PROJ_PERSP */ {
        C_MTXPerspective(r.projMtx, r.projFovy, r.projAspect, r.projNear, r.projFar);
    }

    r.flags |= CameraData::FLAG_PROJ_MTX_READY;
}

} // namespace g3d
} // namespace nw4r
