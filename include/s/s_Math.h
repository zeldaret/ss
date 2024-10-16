#ifndef S_MATH_H
#define S_MATH_H

#include "common.h"

namespace sLib {

// Names from NSMBW
float addCalc(float *value, float target, float ratio, float maxStepSize, float minStepSize);
short addCalcAngle(short *value, short target, short ratio, short maxStepSize, short minStepSize);
BOOL chase(short *value, short target, short stepSize);
BOOL chase(int *value, int target, int stepSize);
BOOL chase(float *value, float target, float stepSize);
BOOL chaseAngle(short *value, short target, short stepSize);

// Inofficial names
float extrapolate(float start, float end, float scale);
BOOL isInRange(float val, float min, float max);
void addCalcScaledDiff(float *value, float target, float ratio, float maxStepSize);
void addCalcScaled(float *value, float stepSize, float maxStep);
int absDiff(short a1, short a2);
BOOL chaseUC(u8 *value, u8 target, u8 stepSize);

} // namespace sLib

#endif
