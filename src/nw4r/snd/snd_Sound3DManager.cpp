#include "nw4r/snd/snd_Sound3DManager.h"
#include "nw4r/snd/snd_AxManager.h"
#include "nw4r/snd/snd_Sound3DListener.h"
#include "nw4r/snd/snd_SoundHandle.h"

#include "nw4r/math.h"

#include <cmath>

namespace nw4r {
namespace snd {
namespace {

/**
 * Solves the linear equation:
 *         a*x + b*y + c = d
 *
 * Where:
 *         a = dFactor - cFactor
 *         b = dAngle - cAngle
 *         c = cAngle * cFactor
 *         d = dAngle * dFactor
 */
inline f32 SolveLinerFunction(f32 x, f32 dAngle, f32 cAngle, f32 dFactor,
                              f32 cFactor) { // @typo
    if (dAngle == cAngle) {
        return (dFactor + cFactor) / 2;
    }

    f32 b = dAngle - cAngle;

    return x * (cFactor - dFactor) / b +
           (dAngle * dFactor - cAngle * cFactor) / b;
}

} // namespace

Sound3DManager::Sound3DManager()
    : mMaxPriorityReduction(32),
      mSpeakerAngleStereo(NW4R_MATH_PI / 4),
      mFrontSpeakerAngleDpl2(NW4R_MATH_PI / 6),
      mRearSpeakerAngleDpl2(2 * NW4R_MATH_PI / 3),
      mInitPan(0.0f),
      mPanRange(0.9f) {}

u32 Sound3DManager::GetRequiredMemSize(const SoundArchive* pArchive) {
    u32 numParam = 0;

    SoundArchive::SoundArchivePlayerInfo playerInfo;
    if (pArchive->ReadSoundArchivePlayerInfo(&playerInfo)) {
        numParam += playerInfo.seqSoundCount;
        numParam += playerInfo.strmSoundCount;
        numParam += playerInfo.waveSoundCount;
    }

    return numParam * sizeof(Sound3DActorParam);
}

bool Sound3DManager::Setup(const SoundArchive* pArchive, void* pBuffer,
                           u32 size) {
#pragma unused(pArchive)

    mParamPool.Create(pBuffer, size);
    return true;
}

void Sound3DManager::detail_Update(SoundParam* pParam, u32 id,
                                   detail::BasicSound* pSound, const void* pArg,
                                   u32 flags) {
    SoundHandle handle;
    if (pSound != NULL) {
        handle.detail_AttachSoundAsTempHandle(pSound);
    }

    Update(pParam, id, pSound != NULL ? &handle : NULL, pArg, flags);
}

void Sound3DManager::Update(SoundParam* pParam, u32 id, SoundHandle* pHandle,
                            const void* pArg, u32 flags) {
#pragma unused(id)
#pragma unused(pHandle)

    f32 angle;
    f32 panLR, panFR;
    f32 pan, surroundPan;

    const Sound3DActorParam* pActorParam =
        static_cast<const Sound3DActorParam*>(pArg);

    Sound3DListener* pListener = GetListener();
    if (pListener == NULL) {
        pParam->volume = 0.0f;
        return;
    }

    math::VEC3 relativePos;
    VEC3Transform(&relativePos, &pListener->GetMatrix(),
                  &pActorParam->position);

    f32 distance = VEC3Len(&relativePos);
    f32 volume = 1.0f;

    if ((flags & (PARAM_UPDATE_VOLUME | PARAM_UPDATE_PRIORITY)) &&
        distance > pListener->GetMaxVolumeDistance()) {

        switch (pActorParam->soundParam.decayCurve) {
        case DECAY_CURVE_LOGARITHMIC: {
            f32 units = (distance - pListener->GetMaxVolumeDistance()) /
                        pListener->GetUnitDistance();

            volume =
                std::pow(pActorParam->soundParam.decayRatio / 256.0f, units);
            break;
        }

        case DECAY_CURVE_LINEAR: {
            f32 units = (distance - pListener->GetMaxVolumeDistance()) /
                        pListener->GetUnitDistance() *
                        pActorParam->soundParam.decayRatio;

            units = units / 256.0f;
            volume = ut::Max(0.0f, 1.0f - units);
            break;
        }
        }
    }

    if ((flags & PARAM_UPDATE_VOLUME) &&
        !(pActorParam->soundParam.flags & PARAM_UPDATE_VOLUME)) {

        pParam->volume = volume;
    }

    if (flags & (PARAM_UPDATE_PAN | PARAM_UPDATE_SURROUND_PAN)) {
        math::VEC3 adjustedPos;

        if (0.0f == distance) {
            adjustedPos.z = 0.0f;
            adjustedPos.y = 0.0f;
            adjustedPos.x = 0.0f;
        } else {
            math::VEC3 relativeXZ(relativePos.x, 0.0f, relativePos.z);

            f32 distanceXZ = VEC3Len(&relativeXZ);
            if (distanceXZ > pListener->GetInteriorSize()) {
                relativeXZ.x *= pListener->GetInteriorSize() / distanceXZ;
                relativeXZ.z *= pListener->GetInteriorSize() / distanceXZ;
            }

            f32 ratioXZ = VEC3Len(&relativeXZ);
            adjustedPos.x = relativePos.x * ratioXZ / distance;
            adjustedPos.y = 0.0f;
            adjustedPos.z = relativePos.z * ratioXZ / distance;
        }

        angle = atan2(adjustedPos.x, -adjustedPos.z);
        f32 distanceNrm = VEC3Len(&adjustedPos) / pListener->GetInteriorSize();

        switch (detail::AxManager::GetInstance().GetOutputMode()) {
        case OUTPUT_MODE_SURROUND:
        case OUTPUT_MODE_DPL2: {
            static f32 angleRearLeft = -mRearSpeakerAngleDpl2;
            static f32 angleFrontLeft = -mFrontSpeakerAngleDpl2;
            static f32 angleFrontRight = mFrontSpeakerAngleDpl2;
            static f32 angleRearRight = mRearSpeakerAngleDpl2;

            // clang-format off
            if (angle < angleRearLeft) {
                panLR = SolveLinerFunction(angle, -NW4R_MATH_PI, angleRearLeft, -1.0f, 0.0f);
                panFR = 1.0f;
            } else if (angle < -NW4R_MATH_PI / 2) {
                panLR = -1.0f;
                panFR = SolveLinerFunction(angle, angleRearLeft, -NW4R_MATH_PI / 2, 0.0f, 1.0f);
            } else if (angle < angleFrontLeft) {
                panLR = -1.0f;
                panFR = SolveLinerFunction(angle, -NW4R_MATH_PI / 2, angleFrontLeft, -1.0f, 0.0f);
            } else if (angle < angleFrontRight) {
                panLR = SolveLinerFunction(angle, angleFrontLeft, angleFrontRight, 1.0f, -1.0f);
                panFR = -1.0f;
            } else if (angle < NW4R_MATH_PI / 2) {
                panLR = 1.0f;
                panFR = SolveLinerFunction(angle, angleFrontRight, NW4R_MATH_PI / 2, 0.0f, -1.0f);
            } else if (angle < angleRearRight) {
                panLR = 1.0f;
                panFR = SolveLinerFunction(angle, NW4R_MATH_PI / 2, angleRearRight, 1.0f, 0.0f);
            } else {
                panLR = SolveLinerFunction(angle, angleRearRight, NW4R_MATH_PI, 0.0f, 1.0f);
                panFR = 1.0f;
            }
            // clang-format on

            f32 cosAvg = (std::cosf(mFrontSpeakerAngleDpl2) +
                          std::cosf(mRearSpeakerAngleDpl2)) /
                         2.0f;

            f32 rearFactor =
                cosAvg / (cosAvg + -std::cosf(mRearSpeakerAngleDpl2));

            panLR *= mPanRange;
            panFR *= mPanRange;

            pan = panLR * distanceNrm;

            surroundPan =
                mInitPan + (1.0f + (panFR * distanceNrm +
                                    rearFactor * (1.0f - distanceNrm)));
            break;
        }

        case OUTPUT_MODE_STEREO: {
            static f32 angleRearLeft = -NW4R_MATH_PI + mSpeakerAngleStereo;
            static f32 angleFrontLeft = -mSpeakerAngleStereo;
            static f32 angleFrontRight = mSpeakerAngleStereo;
            static f32 angleRearRight = NW4R_MATH_PI - mSpeakerAngleStereo;

            // clang-format off
            if (angle < angleRearLeft) {
                panLR = SolveLinerFunction(angle, -NW4R_MATH_PI, angleRearLeft, -1.0f, 0.0f);
                panFR = 1.0f;
            } else if (angle < angleFrontLeft) {
                panLR = -1.0f;
                panFR = SolveLinerFunction(angle, angleRearLeft, angleFrontLeft, -1.0f, 1.0f);
            } else if (angle < angleFrontRight) {
                panLR = SolveLinerFunction(angle, angleFrontLeft, angleFrontRight, 1.0f, -1.0f);
                panFR = -1.0f;
            } else if (angle < angleRearRight) {
                panLR = 1.0f;
                panFR = SolveLinerFunction(angle, angleFrontRight, angleRearRight, 1.0f, -1.0f);
            } else {
                panLR = SolveLinerFunction(angle, angleRearRight, NW4R_MATH_PI, 0.0f, 1.0f);
                panFR = 1.0f;
            }
            // clang-format on

            panLR *= mPanRange;
            panFR *= mPanRange;

            surroundPan = 1.0f + panFR * distanceNrm;
            pan = panLR * distanceNrm;
            break;
        }

        case OUTPUT_MODE_MONO:
        default: {
            pan = 0.0f;
            surroundPan = 0.0f;
            break;
        }
        }
    }

    if ((flags & PARAM_UPDATE_PAN) &&
        !(pActorParam->soundParam.flags & PARAM_UPDATE_PAN)) {

        pParam->pan = pan;
    }

    if ((flags & PARAM_UPDATE_SURROUND_PAN) &&
        !(pActorParam->soundParam.flags & PARAM_UPDATE_SURROUND_PAN)) {

        pParam->surroundPan = surroundPan;
    }

    if ((flags & PARAM_UPDATE_PRIORITY) &&
        !(pActorParam->soundParam.flags & PARAM_UPDATE_PRIORITY)) {

        pParam->priority =
            -static_cast<int>((1.0f - volume) * GetMaxPriorityReduction());
    }
}

void* Sound3DManager::detail_AllocAmbientArg(u32 size) {
    if (size != sizeof(Sound3DActorParam)) {
        return NULL;
    }

    return mParamPool.Alloc();
}

void Sound3DManager::detail_FreeAmbientArg(void* pArg,
                                           const detail::BasicSound* pSound) {
#pragma unused(pSound)

    mParamPool.Free(static_cast<Sound3DActorParam*>(pArg));
}

Sound3DManager::Sound3DActorParam::Sound3DActorParam() : userParam(-1) {
    soundParam.flags = 0;
    soundParam.decayCurve = DECAY_CURVE_LOGARITHMIC;
    soundParam.decayRatio = 128;
}

} // namespace snd
} // namespace nw4r
