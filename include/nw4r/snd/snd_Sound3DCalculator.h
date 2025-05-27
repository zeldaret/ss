#ifndef NW4R_SND_SOUND_3D_CALCULATOR_H
#define NW4R_SND_SOUND_3D_CALCULATOR_H

#include "nw4r/snd/snd_Sound3DManager.h"

namespace nw4r {
namespace snd {

class Sound3DCalculator {
public:
    struct CalcPanParam {
#if 0
        f32 mSpeakerAngleStereo;    // at 0x04
        f32 mFrontSpeakerAngleDpl2; // at 0x08
        f32 mRearSpeakerAngleDpl2;  // at 0x0C
        f32 mInitPan;  // at 0x10
#endif
        f32 field_0x00;
        f32 field_0x04;
        f32 field_0x08;
        f32 field_0x0C;

        CalcPanParam()
            : field_0x00(NW4R_MATH_PI / 4),
              field_0x04(NW4R_MATH_PI / 6),
              field_0x08(2 * NW4R_MATH_PI / 3),
              field_0x0C(0.0f) {}
    };

    static void
    CalcVolumeAndPriority(const Sound3DManager &, const Sound3DListener &, const Sound3DParam &, float *, int *);
    static void
    CalcPan(const Sound3DManager &, const Sound3DListener &, const Sound3DParam &, const Sound3DCalculator::CalcPanParam &, float *pan, float *surroundPan);
    static void CalcPitch(const Sound3DManager &, const Sound3DListener &, const Sound3DParam &, float *);
    static void CalcBiquadFilterValue(const Sound3DManager &, const Sound3DListener &, const Sound3DParam &, float *);
    static void CalcPanDpl2(const nw4r::math::VEC3 &, float, float, float, float, float, float, float *, float *);
    static void CalcPanStereo(const nw4r::math::VEC3 &, float, float, float, float, float *, float *);
    static void CalcAngleAndDistance(const nw4r::math::VEC3 &, float, float, float *, float *);
};

} // namespace snd
} // namespace nw4r

#endif
