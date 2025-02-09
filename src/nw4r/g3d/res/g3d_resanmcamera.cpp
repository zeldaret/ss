#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void ResAnmCamera::GetAnmResult(CameraAnmResult* pResult, f32 frame) const {
    const ResAnmCameraData& r = ref();
    u32 flags = r.flags;

    bool constantPosX = (flags & ResAnmCameraData::FLAG_POS_X_CONST) != 0;
    bool constantPosY = (flags & ResAnmCameraData::FLAG_POS_Y_CONST) != 0;
    bool constantPosZ = (flags & ResAnmCameraData::FLAG_POS_Z_CONST) != 0;

    bool constantAspect = (flags & ResAnmCameraData::FLAG_ASPECT_CONST) != 0;
    bool constantNear = (flags & ResAnmCameraData::FLAG_NEAR_CONST) != 0;
    bool constantFar = (flags & ResAnmCameraData::FLAG_FAR_CONST) != 0;

    CameraType type = static_cast<CameraType>(
        flags & ResAnmCameraData::FLAG_CAMERA_TYPE_MASK);

    pResult->flags = flags & (CameraAnmResult::FLAG_CAMERA_TYPE_MASK |
                              CameraAnmResult::FLAG_ANM_EXISTS);

    pResult->projType = r.projType;

    pResult->pos.x = detail::GetResAnmResult(&r.posX, frame, constantPosX);
    pResult->pos.y = detail::GetResAnmResult(&r.posY, frame, constantPosY);
    pResult->pos.z = detail::GetResAnmResult(&r.posZ, frame, constantPosZ);

    pResult->aspect = detail::GetResAnmResult(&r.aspect, frame, constantAspect);
    pResult->near = detail::GetResAnmResult(&r.near, frame, constantNear);
    pResult->far = detail::GetResAnmResult(&r.far, frame, constantFar);

    switch (type) {
    case CAMERATYPE_ROTATE: {
        bool constantRotX = (flags & ResAnmCameraData::FLAG_ROT_X_CONST) != 0;
        bool constantRotY = (flags & ResAnmCameraData::FLAG_ROT_Y_CONST) != 0;
        bool constantRotZ = (flags & ResAnmCameraData::FLAG_ROT_Z_CONST) != 0;

        pResult->rotate.rot.x =
            detail::GetResAnmResult(&r.rotX, frame, constantRotX);
        pResult->rotate.rot.y =
            detail::GetResAnmResult(&r.rotY, frame, constantRotY);
        pResult->rotate.rot.z =
            detail::GetResAnmResult(&r.rotZ, frame, constantRotZ);
        break;
    }

    case CAMERATYPE_AIM: {
        bool constantAimX = (flags & ResAnmCameraData::FLAG_AIM_X_CONST) != 0;
        bool constantAimY = (flags & ResAnmCameraData::FLAG_AIM_Y_CONST) != 0;
        bool constantAimZ = (flags & ResAnmCameraData::FLAG_AIM_Z_CONST) != 0;
        bool constantTwist = (flags & ResAnmCameraData::FLAG_TWIST_CONST) != 0;

        pResult->aim.aim.x =
            detail::GetResAnmResult(&r.aimX, frame, constantAimX);
        pResult->aim.aim.y =
            detail::GetResAnmResult(&r.aimY, frame, constantAimY);
        pResult->aim.aim.z =
            detail::GetResAnmResult(&r.aimZ, frame, constantAimZ);
        pResult->aim.twist =
            detail::GetResAnmResult(&r.twist, frame, constantTwist);
        break;
    }

    default: {
        break;
    }
    }

    switch (r.projType) {
    case GX_PERSPECTIVE: {
        bool constantFovy =
            (flags & ResAnmCameraData::FLAG_PERSP_FOVY_CONST) != 0;

        pResult->perspFovy =
            detail::GetResAnmResult(&r.perspFovy, frame, constantFovy);
        break;
    }

    case GX_ORTHOGRAPHIC: {
        bool constantHeight =
            (flags & ResAnmCameraData::FLAG_ORTHO_HEIGHT_CONST) != 0;

        pResult->orthoHeight =
            detail::GetResAnmResult(&r.orthoHeight, frame, constantHeight);
        break;
    }

    default: {
        break;
    }
    }
}

} // namespace g3d
} // namespace nw4r
