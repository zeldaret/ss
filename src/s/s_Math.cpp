#include <s/s_Math.h>
#include <math.h>

namespace sLib {

extern "C" float fn_802DE740(float f1, float f2, float f3) {
    float diff = f2 - f1;
    if (f3 < 1.0f) {
        return f2;
    }
    return f1 + diff / f3;
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
            *value = *value + step;
        } else if (step > 0.0f) {
            if (step < minStepSize) {
                *value = *value + minStepSize;
                if (*value > target) {
                    *value = target;
                }
            }
        } else if (step > -minStepSize) {
            *value = *value + (-minStepSize);
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

extern "C" void fn_802DE830(float *value, float target, float ratio, float maxStepSize) {
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

extern "C" void fn_802DE880(float *value, float p2, float p3) {
    float step = *value * p2;
    if (step > p3) {
        step = p3;
    } else if (step < -p3) {
        step = -p3;
    }
    *value -= step;
}

extern "C" int fn_802DE8C0(short a1, short a2) {
    return abs((short)(a1 - a2));
}

template <typename T>
BOOL chaseT(T *value, T target, T stepSize) {
    if (*value == target) {
        return 1;
    }

    if (stepSize != 0) {
        if (*value > target) {
            stepSize = -stepSize;
        }

        T step = *value + stepSize;
        *value = step;
        if (stepSize * (step - target) >= 0) {
            *value = target;
            return 1;
        }
    }

    return 0;
}

template <typename T>
T addCalcAngleT(T *p1, T p2, T p3, T p4, T p5) {}

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

short addCalcAngle(short *a, short b, short c, short d, short e) {
    return addCalcAngleT(a, b, c, d, e);
}

void addCalcAngle(short *value, short target, short ratio, short maxStepSize) {
    return addCalcAngleT(value, target, ratio, maxStepSize);
}

// template BOOL chaseT<char>(char*, char, char);

BOOL chase(short *value, short target, short stepSize) {
    return chaseT(value, target, stepSize);
}

BOOL chase(int *value, int target, int stepSize) {
    return chaseT(value, target, stepSize);
}

BOOL chase(float *value, float target, float stepSize) {
    return chaseT(value, target, stepSize);
}

BOOL chaseAngle(short *value, short target, short stepSize) {
    if (*value == target) {
        return 1;
    }

    if (stepSize != 0) {
        if ((short)(*value - target) > 0) {
            stepSize = -stepSize;
        }

        short step = *value + stepSize;
        *value = step;
        if (stepSize * (short)(step - target) >= 0) {
            *value = target;
            return 1;
        }
    }

    return 0;
}

} // namespace sLib
