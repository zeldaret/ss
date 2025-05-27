#ifndef NW4R_SND_SOUND_3D_ENGINE_H
#define NW4R_SND_SOUND_3D_ENGINE_H

#include "nw4r/math.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/snd/snd_Sound3DManager.h"
#include "nw4r/snd/snd_Sound3DCalculator.h"


namespace nw4r {
namespace snd {

class Sound3DManager;

class Sound3DEngine {
public:
    Sound3DEngine();
    virtual ~Sound3DEngine() {}

    virtual void UpdateAmbientParam(const Sound3DManager*, const Sound3DParam*, u32, int, SoundAmbientParam*);
    virtual s32 GetAmbientPriority(const Sound3DManager*, const Sound3DParam*, u32);
    virtual s32 GetRequiredVoiceOutCount(const Sound3DManager*, const Sound3DParam*, u32);
    virtual void UpdateAmbientParam(const Sound3DManager*, const Sound3DParam*, u32, u32, SoundAmbientParam*);

private:
    Sound3DCalculator::CalcPanParam mPanParam; // at 0x04
};

} // namespace snd
} // namespace nw4r

#endif
