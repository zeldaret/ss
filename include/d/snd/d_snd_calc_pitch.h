#ifndef D_SND_CALC_PITCH_H
#define D_SND_CALC_PITCH_H

#include "common.h"

f32 pitchScaleForPitchIdx(s32 idx);
f32 pitchScaleForPitchIdxTenths(f32 idx);
f32 pitchIdxTenthsForPitchScale(f32 scale);

#endif
