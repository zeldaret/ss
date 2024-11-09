#include "m/m_angle.h"

#include "math.h"

/** 80575c08 */
mAng3_c mAng3_c::Zero = mAng3_c(0, 0, 0);

const f32 mAng::sHalfCircleDeg = 360.0f / 2;
const f32 mAng::sAngToDeg = 360.0f / (1 << 16);
const f32 mAng::sAngToRad = 2 * M_PI / (1 << 16);
const f32 mAng::sAngToNorm = 2.0f / (1 << 16);
const f32 mAng::sDegToRad = M_PI / 180.0f;
const f32 mAng::sDegToAng = (1 << 16) / 360.0f;
const f32 mAng::sRadToAng = (1 << 16) / (2 * M_PI);
const f32 mAng::sRadToDeg = 180.0f / M_PI;

/** 802ee5f0 */
s32 mAng::step(s16 target, s32 steps, s16 max, s16 min) {
    if (mVal != target) {
        int stepSize;
        int diff = target - mVal;
        stepSize = diff / steps;
        if (stepSize > min || stepSize < -min) {
            mVal += stepSize < -max ? -max : stepSize > max ? max : stepSize;
        } else if (diff >= 0) {
            if (mVal + min >= target) {
                mVal = target;
            } else {
                mVal += min;
            }
        } else {
            if (mVal - min <= target) {
                mVal = target;
            } else {
                mVal -= min;
            }
        }
    }

    return target - mVal;
}
