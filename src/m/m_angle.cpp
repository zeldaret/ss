#include <m/m_angle.h>

/** 80575c08 */
mAng3_c mAng3_c::ZERO = mAng3_c::mAng3_c(0, 0, 0);

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
