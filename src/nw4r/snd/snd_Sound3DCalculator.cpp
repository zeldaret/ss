#include "nw4r/snd/snd_Sound3DCalculator.h"

#include "nw4r/snd/snd_AxManager.h"
#include "nw4r/snd/snd_Sound3DListener.h"
#include "nw4r/snd/snd_Sound3DManager.h"

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

    return x * (cFactor - dFactor) / b + (dAngle * dFactor - cAngle * cFactor) / b;
}

} // namespace

void Sound3DCalculator::CalcVolumeAndPriority(
    const nw4r::snd::Sound3DManager &mgr, const nw4r::snd::Sound3DListener &listener,
    const nw4r::snd::Sound3DParam &param, f32 *pVolume, int *pPriority
) {
    math::VEC3 relativePos;
    math::VEC3Sub(&relativePos, &param.position, &listener.GetPosition());

    f32 distance = VEC3Len(&relativePos);
    f32 decayRatio = param.decayRatio / 256.0f;
    f32 maxVolumeDistance = listener.GetMaxVolumeDistance();
    f32 unitDistance = listener.GetUnitDistance();
    s32 maxPriorityReduction = mgr.GetMaxPriorityReduction();
    u8 decayCurve = param.decayCurve;

    f32 volume = 1.0f;
    if (distance > maxVolumeDistance) {
        switch (decayCurve) {
            case Sound3DManager::DECAY_CURVE_LOGARITHMIC: {
                f32 units = (distance - maxVolumeDistance) / unitDistance;

                volume = std::pow(decayRatio, units);
                break;
            }

            case Sound3DManager::DECAY_CURVE_LINEAR: {
                f32 units = (distance - maxVolumeDistance) / unitDistance * (1.0f - decayRatio);

                volume = 1.0f - units;
                if (volume < 0.0f) {
                    volume = 0.0f;
                }
                break;
            }
        }
    }
    *pVolume = volume;
    *pPriority = -static_cast<int>((1.0f - volume) * maxPriorityReduction);
}

inline void CalcPanInline(
    const nw4r::math::VEC3 &relativePos, f32 speakerAngleStereo, f32 interiorSize, f32 distance, f32 panRange,
    f32 frontSpeakerAngleDpl2, f32 rearSpeakerAngleDpl2, f32 initPan, f32 *pan, f32 *surroundPan
) {
    switch (detail::AxManager::GetInstance().GetOutputMode()) {
        case OUTPUT_MODE_SURROUND:
        case OUTPUT_MODE_DPL2:
            Sound3DCalculator::CalcPanDpl2(
                relativePos, interiorSize, distance, panRange, frontSpeakerAngleDpl2, rearSpeakerAngleDpl2, initPan,
                pan, surroundPan
            );
            break;
        case OUTPUT_MODE_STEREO:
            Sound3DCalculator::CalcPanStereo(
                relativePos, interiorSize, distance, panRange, speakerAngleStereo, pan, surroundPan
            );
            break;
        default:
            *pan = 0.0f;
            *surroundPan = 0.0f;
            break;
    }
}

void Sound3DCalculator::CalcPan(
    const Sound3DManager &mgr, const Sound3DListener &listener, const Sound3DParam &param,
    const Sound3DCalculator::CalcPanParam &panParam, f32 *pan, f32 *surroundPan
) {
    math::VEC3 relativePos;
    VEC3Transform(&relativePos, &listener.GetMatrix(), &param.position);
    f32 distance = math::VEC3Len(&relativePos);

    CalcPanInline(
        relativePos, panParam.speakerAngleStereo, listener.GetInteriorSize(), distance, mgr.GetField0x20(), panParam.frontSpeakerAngleDpl2,
        panParam.rearSpeakerAngleDpl2, panParam.initPan, pan, surroundPan
    );
}

void Sound3DCalculator::CalcPitch(
    const nw4r::snd::Sound3DManager &mgr, const nw4r::snd::Sound3DListener &listener,
    const nw4r::snd::Sound3DParam &param, f32 *pPitch
) {
    f32 f0 = mgr.GetField0x24();
    if (f0 == 0.0f) {
        *pPitch = 1.0f;
        return;
    }

    math::VEC3 relativePos;
    math::VEC3Sub(&relativePos, &param.position, &listener.GetPosition());
    f32 distance = VEC3Len(&relativePos);
    if (distance > 0.0f) {
        relativePos /= distance;
    }
    f32 f3 = param.field_0x1E / 32.0f;
    f32 f1, f2;
    if (distance > 0.0f) {
        f1 = -math::VEC3Dot(&relativePos, &param.velocity);
        f2 = -math::VEC3Dot(&relativePos, &listener.GetVelocity());
    } else {
        f1 = -math::VEC3Len(&param.velocity);
        f2 = math::VEC3Len(&listener.GetVelocity());
    }
    f32 pitch;
    f1 *= f3;
    f2 *= f3;
    if (f2 > f0) {
        pitch = 0.0f;
    } else if (f1 >= f0) {
        pitch = 65535.0f;
    } else {
        pitch = (f0 - f2) / (f0 - f1);
    }
    *pPitch = pitch;
}

void Sound3DCalculator::CalcBiquadFilterValue(
    const nw4r::snd::Sound3DManager &mgr, const nw4r::snd::Sound3DListener &listener,
    const nw4r::snd::Sound3DParam &param, f32 *pBiquadValue
) {
    math::VEC3 relativePos;
    math::VEC3Sub(&relativePos, &param.position, &listener.GetPosition());
    f32 distance = VEC3Len(&relativePos);

    f32 value = 0.0f;
    f32 maxVolumeDistance = listener.GetMaxVolumeDistance();
    f32 maxValue = listener.GetUnitBiquadFilterMax();

    if (distance > maxVolumeDistance) {
        value = ((distance - maxVolumeDistance) / listener.GetUnitDistance()) * listener.GetUnitBiquadFilterValue();
        if (value > maxValue) {
            value = maxValue;
        }
    }
    *pBiquadValue = value;
}

void Sound3DCalculator::CalcPanDpl2(
    const nw4r::math::VEC3 &relativePos, f32 interiorSize, f32 distance, f32 mPanRange, f32 mFrontSpeakerAngleDpl2,
    f32 mRearSpeakerAngleDpl2, f32 mInitPan, f32 *pan, f32 *surroundPan
) {
    f32 angle, distanceNrm;
    CalcAngleAndDistance(relativePos, distance, interiorSize, &angle, &distanceNrm);

    f32 panLR, panFR;

    f32 angleRearLeft = -mRearSpeakerAngleDpl2;
    f32 angleFrontLeft = -mFrontSpeakerAngleDpl2;
    f32 angleFrontRight = mFrontSpeakerAngleDpl2;
    f32 angleRearRight = mRearSpeakerAngleDpl2;

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

    f32 cosAvg = (std::cosf(mFrontSpeakerAngleDpl2) + std::cosf(mRearSpeakerAngleDpl2)) / 2.0f;

    f32 rearFactor = cosAvg / (cosAvg + -std::cosf(mRearSpeakerAngleDpl2));

    panLR *= mPanRange;
    panFR *= mPanRange;

    *pan = panLR * distanceNrm;

    *surroundPan = mInitPan + (1.0f + (panFR * distanceNrm + rearFactor * (1.0f - distanceNrm)));
}

void Sound3DCalculator::CalcPanStereo(
    const nw4r::math::VEC3 &relativePos, f32 interiorSize, f32 distance, f32 panRange, f32 speakerAngleStereo,
    f32 *surroundPan, f32 *pan
) {
    f32 angle, distanceNrm;
    CalcAngleAndDistance(relativePos, distance, interiorSize, &angle, &distanceNrm);

    f32 factor;

    f32 angleRearLeft = -NW4R_MATH_PI + speakerAngleStereo;
    f32 angleFrontLeft = -speakerAngleStereo;
    f32 angleFrontRight = speakerAngleStereo;
    f32 angleRearRight = NW4R_MATH_PI - speakerAngleStereo;

    if (angle < angleRearLeft) {
        factor = SolveLinerFunction(angle, -NW4R_MATH_PI, angleRearLeft, -1.0f, 0.0f);
    } else if (angle < angleFrontLeft) {
        factor = -1.0f;
    } else if (angle < angleFrontRight) {
        factor = SolveLinerFunction(angle, angleFrontLeft, angleFrontRight, 1.0f, -1.0f);
    } else if (angle < angleRearRight) {
        factor = 1.0f;
    } else {
        factor = SolveLinerFunction(angle, angleRearRight, NW4R_MATH_PI, 0.0f, 1.0f);
    }

    factor *= panRange;

    *surroundPan = factor * distanceNrm;
    *pan = 0.0f;
}

void Sound3DCalculator::CalcAngleAndDistance(
    const nw4r::math::VEC3 &relativePos, f32 distance, f32 interiorSize, f32 *pAngle, f32 *pDistance
) {
    math::VEC3 adjustedPos;

    if (0.0f == distance) {
        adjustedPos.z = 0.0f;
        adjustedPos.y = 0.0f;
        adjustedPos.x = 0.0f;
    } else {
        math::VEC3 relativeXZ(relativePos.x, 0.0f, relativePos.z);

        f32 distanceXZ = VEC3Len(&relativeXZ);
        if (distanceXZ > interiorSize) {
            relativeXZ.x *= interiorSize / distanceXZ;
            relativeXZ.z *= interiorSize / distanceXZ;
        }

        f32 ratioXZ = VEC3Len(&relativeXZ);
        adjustedPos.x = relativePos.x * ratioXZ / distance;
        adjustedPos.y = 0.0f;
        adjustedPos.z = relativePos.z * ratioXZ / distance;
    }

    *pAngle = atan2(adjustedPos.x, -adjustedPos.z);
    *pDistance = VEC3Len(&adjustedPos) / interiorSize;
}

} // namespace snd
} // namespace nw4r
