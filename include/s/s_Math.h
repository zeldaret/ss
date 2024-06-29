#ifndef S_MATH_H
#define S_MATH_H

#include <common.h>

namespace sLib {

float addCalc(float *value, float target, float ratio, float maxStepSize, float minStepSize);

BOOL chase(short *value, short target, short stepSize);
BOOL chase(int *value, int target, int stepSize);
BOOL chase(float *value, float target, float stepSize);

BOOL chaseAngle(short *value, short target, short stepSize);

} // namespace sLib

#endif
