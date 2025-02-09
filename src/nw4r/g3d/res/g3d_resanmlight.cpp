#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void ResAnmLight::GetAnmResult(LightAnmResult* pResult, f32 frame) const {
    const ResAnmLightData& r = ref();
    u32 flags = r.flags;

    const ResAnmScnInfoData& rInfoData =
        ofs_to_ptr<ResAnmScnData>(r.toResAnmScnData)->info;

    f32 clippedFrame = detail::ClipFrame(rInfoData, frame);

    pResult->flags = flags & (LightAnmResult::FLAG_LIGHT_TYPE_MASK |
                              LightAnmResult::FLAG_LIGHT_ENABLE |
                              LightAnmResult::FLAG_SPECULAR_ENABLE |
                              LightAnmResult::FLAG_COLOR_ENABLE |
                              LightAnmResult::FLAG_ALPHA_ENABLE);

    bool enableConstant = (flags & ResAnmLightData::FLAG_ENABLE_CONST) != 0;
    if (!enableConstant) {
        int iClippedFrame = static_cast<int>(math::FFloor(clippedFrame));

        bool enable = detail::GetResBoolAnmResult(&r.enable, iClippedFrame);
        if (enable) {
            pResult->flags |= ResAnmLightData::FLAG_LIGHT_ENABLE;
        }
    }

    if (pResult->flags & ResAnmLightData::FLAG_LIGHT_ENABLE) {
        LightType type = static_cast<LightType>(
            flags & ResAnmLightData::FLAG_LIGHT_TYPE_MASK);

        bool hasSpecular = (flags & ResAnmLightData::FLAG_SPECULAR_ENABLE) != 0;
        bool hasAim = type != LIGHTTYPE_POINT || hasSpecular;

        bool constantPosX = (flags & ResAnmLightData::FLAG_POS_X_CONST) != 0;
        bool constantPosY = (flags & ResAnmLightData::FLAG_POS_Y_CONST) != 0;
        bool constantPosZ = (flags & ResAnmLightData::FLAG_POS_Z_CONST) != 0;
        bool constantColor = (flags & ResAnmLightData::FLAG_COLOR_CONST) != 0;

        pResult->pos.x = detail::GetResAnmResult(&r.posX, frame, constantPosX);
        pResult->pos.y = detail::GetResAnmResult(&r.posY, frame, constantPosY);
        pResult->pos.z = detail::GetResAnmResult(&r.posZ, frame, constantPosZ);
        pResult->color =
            detail::GetResColorAnmResult(&r.color, clippedFrame, constantColor);

        if (hasAim) {
            bool constantAimX =
                (flags & ResAnmLightData::FLAG_AIM_X_CONST) != 0;
            bool constantAimY =
                (flags & ResAnmLightData::FLAG_AIM_Y_CONST) != 0;
            bool constantAimZ =
                (flags & ResAnmLightData::FLAG_AIM_Z_CONST) != 0;

            pResult->aim.x =
                detail::GetResAnmResult(&r.aimX, frame, constantAimX);
            pResult->aim.y =
                detail::GetResAnmResult(&r.aimY, frame, constantAimY);
            pResult->aim.z =
                detail::GetResAnmResult(&r.aimZ, frame, constantAimZ);
        }

        switch (type) {
        case LIGHTTYPE_POINT: {
            bool refDistanceConstant =
                (flags & ResAnmLightData::FLAG_REF_DISTANCE_CONST) != 0;
            bool refBrightnessConstant =
                (flags & ResAnmLightData::FLAG_REF_BRIGHTNESS_CONST) != 0;

            pResult->distFunc = r.distFunc;

            pResult->refDistance = detail::GetResAnmResult(
                &r.refDistance, frame, refDistanceConstant);

            pResult->refBrightness = detail::GetResAnmResult(
                &r.refBrightness, frame, refBrightnessConstant);
            break;
        }

        case LIGHTTYPE_DIRECTIONAL: {
            break;
        }

        case LIGHTTYPE_SPOT: {
            bool cutoffConstant =
                (flags & ResAnmLightData::FLAG_CUTOFF_CONST) != 0;
            bool refDistanceConstant =
                (flags & ResAnmLightData::FLAG_REF_DISTANCE_CONST) != 0;
            bool refBrightnessConstant =
                (flags & ResAnmLightData::FLAG_REF_BRIGHTNESS_CONST) != 0;

            pResult->distFunc = r.distFunc;
            pResult->spotFunc = r.spotFunc;

            pResult->cutoff =
                detail::GetResAnmResult(&r.cutoff, frame, cutoffConstant);

            pResult->refDistance = detail::GetResAnmResult(
                &r.refDistance, frame, refDistanceConstant);

            pResult->refBrightness = detail::GetResAnmResult(
                &r.refBrightness, frame, refBrightnessConstant);
            break;
        }
        }

        if (hasSpecular) {
            bool specColorConstant =
                (flags & ResAnmLightData::FLAG_SPEC_COLOR_CONST) != 0;
            bool shininessConstant =
                (flags & ResAnmLightData::FLAG_SHININESS_CONST) != 0;

            pResult->specColor = detail::GetResColorAnmResult(
                &r.specColor, clippedFrame, specColorConstant);

            pResult->shininess =
                detail::GetResAnmResult(&r.shininess, frame, shininessConstant);

            pResult->specIdx = r.specLightObjIdx;
        }
    }
}

} // namespace g3d
} // namespace nw4r
