#include "nw4r/snd/snd_Sound3DEngine.h"

namespace nw4r { namespace snd {

Sound3DEngine::Sound3DEngine()
    : mSpeakerAngleStereo(NW4R_MATH_PI / 4),
      mFrontSpeakerAngleDpl2(NW4R_MATH_PI / 6),
      mRearSpeakerAngleDpl2(2 * NW4R_MATH_PI / 3),
      mInitPan(0.0f) {}

void Sound3DEngine::UpdateAmbientParam(
    const Sound3DManager *mgr, const Sound3DParam *pParam, u32, u32 flags, SoundAmbientParam *pAmbientParam
) {
    if (flags & 1)
        pAmbientParam->volume = 0.0f;

    if (flags & 8)
        pAmbientParam->priority = - mgr->GetMaxPriorityReduction();
}

void Sound3DEngine::UpdateAmbientParam(const Sound3DManager* mgr, const Sound3DParam* param, u32, int, SoundAmbientParam*) {

}

void Sound3DEngine::GetAmbientPriority(const Sound3DManager*, const Sound3DParam*, u32) {

}

s32 Sound3DEngine::GetRequiredVoiceOutCount(const Sound3DManager*, const Sound3DParam*, u32) {
    return 1;
}


}}
