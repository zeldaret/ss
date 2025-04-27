#include "s/s_Math.h"

#include "math.h"

namespace sLib {

float extrapolate(float start, float end, float scale) {
    float diff = end - start;
    if (scale < 1.0f) {
        return end;
    }
    return start + diff / scale;
}

float addCalc(float *value, float target, float ratio, float maxStepSize, float minStepSize) {
    if (*value != target) {
        float step = ratio * (target - *value);
        if (step >= minStepSize || (step <= -minStepSize)) {
            if (step > maxStepSize) {
                step = maxStepSize;
            }
            if (step < -maxStepSize) {
                step = -maxStepSize;
            }
            *value += step;
        } else if (step > 0.0f) {
            if (step < minStepSize) {
                *value = *value + minStepSize;
                if (*value > target) {
                    *value = target;
                }
            }
        } else if (step > -minStepSize) {
            *value += -minStepSize;
            if (*value < target) {
                *value = target;
            }
        }
    }

    float ret = target - *value;
    if (ret > 0.0f) {
        return ret;
    } else {
        return -ret;
    }
}

void addCalcScaledDiff(float *value, float target, float ratio, float maxStepSize) {
    if (*value == target) {
        return;
    }

    float step = ratio * (target - *value);
    if (step > maxStepSize) {
        step = maxStepSize;
    } else if (step < -maxStepSize) {
        step = -maxStepSize;
    }
    *value += step;
}

void addCalcScaled(float *value, float stepSize, float maxStep) {
    float step = *value * stepSize;
    if (step > maxStep) {
        step = maxStep;
    } else if (step < -maxStep) {
        step = -maxStep;
    }
    *value -= step;
}

int absDiff(short a1, short a2) {
    return abs((short)(a1 - a2));
}

template <typename T>
T addCalcAngleT(T *value, T target, T ratio, T maxStepSize, T minStepSize) {
    T diff = target - *value;
    if (*value != target) {
        T step = diff / ratio;
        if (step > minStepSize || step < -minStepSize) {
            if (step > maxStepSize) {
                step = maxStepSize;
            } else if (step < -maxStepSize) {
                step = -maxStepSize;
            }
            *value += step;
        } else {
            if (0 <= diff) {
                T newVal = *value + minStepSize;
                *value = newVal;
                if ((T)(target - *value) <= 0) {
                    *value = target;
                }
            } else {
                T newVal = *value - minStepSize;
                *value = newVal;
                if ((T)(target - newVal) >= 0) {
                    *value = target;
                }
            }
        }
    }

    return target - *value;
}

template <typename T>
void addCalcAngleT(T *value, T target, T ratio, T maxStepSize) {
    T diff = target - *value;
    T step = diff / ratio;
    if (step > maxStepSize) {
        *value += maxStepSize;
    } else if (step < -maxStepSize) {
        *value -= maxStepSize;
    } else {
        *value += step;
    }
}

short addCalcAngle(short *value, short target, short ratio, short maxStepSize, short minStepSize) {
    return addCalcAngleT(value, target, ratio, maxStepSize, minStepSize);
}

void addCalcAngle(short *value, short target, short ratio, short maxStepSize) {
    return addCalcAngleT(value, target, ratio, maxStepSize);
}

short addCalcAngle2(short *value, short target, short ratio, short maxStepSize, short minStepSize) {
    s16 oldValue = *value;
    if (oldValue != target) {
        s32 unclampedStep;
        if (ratio > 0) {
            if (target < oldValue) {
                unclampedStep = 0x10000 - (oldValue - target);
            } else {
                unclampedStep = target - oldValue;
            }
        } else if (ratio < 0) {
            if (target < oldValue) {
                unclampedStep = oldValue - target;
            } else {
                unclampedStep = 0xFFFF - (target - oldValue);
            }
        } else {
            return (s16)(target - oldValue);
        }

        s32 scaledStep = labs(unclampedStep / ratio);
        s16 clampedStep = 0x7FFF;
        if (scaledStep <= 0x7FFF) {
            clampedStep = scaledStep;
        }
        if (clampedStep > minStepSize) {
            if (clampedStep > maxStepSize) {
                clampedStep = maxStepSize;
            }
            if (ratio < 0) {
                clampedStep = -clampedStep;
            }
            *value += clampedStep;
        } else if (ratio >= 0) {
            *value += minStepSize;
            if (((s16)(target - oldValue) > 0) && ((s16)(target - *value) <= 0)) {
                *value = target;
            }
        } else {
            *value -= minStepSize;
            if (((s16)(target - oldValue) < 0) && ((s16)(target - *value) >= 0)) {
                *value = target;
            }
        }
    }
    return (s16)(target - *value);
}

// Is the same as cLib_chaseUC
BOOL chaseUC(u8 *value, u8 target, u8 stepSize) {
    if (stepSize) {
        s16 val = *value;
        s16 tgt = target;
        s16 szs;

        if (val > tgt) {
            szs = -stepSize;
        } else {
            szs = stepSize;
        }

        val += szs;

        if (szs * (val - tgt) >= 0) {
            *value = target;
            return TRUE;
        } else {
            *value = val;
        }
    }

    else if (*value == target) {
        return TRUE;
    }
    return FALSE;
}

template <typename T>
BOOL chaseT(T *value, T target, T stepSize) {
    if (*value == target) {
        return 1;
    }

    if (stepSize) {
        if (*value > target) {
            stepSize = -stepSize;
        }

        *value += stepSize;
        if (stepSize * (*value - target) >= 0) {
            *value = target;
            return 1;
        }
    }

    return 0;
}

BOOL chase(short *value, short target, short stepSize) {
    return chaseT(value, target, stepSize);
}

BOOL chase(int *value, int target, int stepSize) {
    return chaseT(value, target, stepSize);
}

BOOL chase(float *value, float target, float stepSize) {
    return chaseT(value, target, stepSize);
}

template <typename T>
BOOL isInRangeT(T val, T min, T max) {
    BOOL ret;
    if (min < max) {
        return val >= min && val <= max;
    } else {
        return val >= max && val <= min;
    }
    return ret;
}

BOOL isInRange(float val, float min, float max) {
    return isInRangeT(val, min, max);
}

BOOL chaseAngle(short *value, short target, short stepSize) {
    if (*value == target) {
        return 1;
    }

    if (stepSize != 0) {
        if ((short)(*value - target) > 0) {
            stepSize = -stepSize;
        }

        *value += stepSize;
        if (stepSize * (short)(*value - target) >= 0) {
            *value = target;
            return 1;
        }
    }

    return 0;
}

BOOL chaseAngle2(short *value, short target, short stepSize) {
    if (*value == target) {
        return TRUE;
    }

    if (stepSize) {
        short step = stepSize;

        if (stepSize < 0) {
            if (stepSize != 0x8000) {
                stepSize = -stepSize;
            } else {
                stepSize = 0x7FFF;
            }
        }

        if ((short)(*value - target) > 0) {
            stepSize = -stepSize;
        }

        *value += step;

        if (step * stepSize > 0) {
            if (step * (short)(*value - target) >= 0) {
                *value = target;
                return TRUE;
            }
        }
    }
    return FALSE;
}

} // namespace sLib
