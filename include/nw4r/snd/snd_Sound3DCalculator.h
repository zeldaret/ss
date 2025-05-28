#ifndef NW4R_SND_SOUND_3D_CALCULATOR_H
#define NW4R_SND_SOUND_3D_CALCULATOR_H

#include "common.h"
#include "nw4r/math/math_triangular.h"

namespace nw4r {
namespace snd {

class Sound3DManager;
class Sound3DParam;

class Sound3DCalculator {
public:
    struct CalcPanParam {
        f32 speakerAngleStereo;    // at 0x00
        f32 frontSpeakerAngleDpl2; // at 0x04
        f32 rearSpeakerAngleDpl2;  // at 0x08
        f32 initPan;               // at 0x0C

        CalcPanParam()
            : speakerAngleStereo(NW4R_MATH_PI / 4),
              frontSpeakerAngleDpl2(NW4R_MATH_PI / 6),
              rearSpeakerAngleDpl2(2 * NW4R_MATH_PI / 3),
              initPan(0.0f) {}
    };

    static void
    CalcVolumeAndPriority(const Sound3DManager &, const Sound3DListener &, const Sound3DParam &, float *, int *);
    static void CalcPan(
        const Sound3DManager &, const Sound3DListener &, const Sound3DParam &, const Sound3DCalculator::CalcPanParam &,
        float *pan, float *surroundPan
    );
    static void CalcPitch(const Sound3DManager &, const Sound3DListener &, const Sound3DParam &, float *);
    static void CalcBiquadFilterValue(const Sound3DManager &, const Sound3DListener &, const Sound3DParam &, float *);
    static void CalcPanDpl2(const nw4r::math::VEC3 &, float, float, float, float, float, float, float *, float *);
    static void CalcPanStereo(const nw4r::math::VEC3 &, float, float, float, float, float *, float *);
    static void CalcAngleAndDistance(const nw4r::math::VEC3 &, float, float, float *, float *);
};

} // namespace snd
} // namespace nw4r

#endif
