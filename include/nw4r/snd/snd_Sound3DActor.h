
#ifndef NW4R_SND_SOUND_3D_ACTOR_H
#define NW4R_SND_SOUND_3D_ACTOR_H
#include "nw4r/math.h"
#include "nw4r/math/math_types.h"
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundActor.h"
#include "nw4r/types_nw4r.h"


namespace nw4r {
namespace snd {

class Sound3DActor : public SoundActor, public detail::BasicSound::AmbientArgUpdateCallback {
public:
    Sound3DActor(SoundArchivePlayer &rPlayer, Sound3DManager &rManager);
    virtual ~Sound3DActor(); // at 0x8

    virtual StartResult SetupSound(
        SoundHandle *pHandle, u32 id,
        const StartInfo *pStartInfo,
        void*
    ) override; // at 0x14
    
    void SetPosition(const math::VEC3 &rPosition);

private:
    static void ClearUpdateCallback(SoundHandle &rHandle);

private:
    Sound3DManager &m3DManager;              // at 0x58
    SoundArchivePlayer *mSoundArchivePlayer; // at 0x5C
    u32 mUserParam;                          // at 0x60
    math::VEC3 mPosition;                    // at 0x64
    math::VEC3 mVelocity;                     // at 0x70
    bool field_0x7C;                         // at 0x7C
};

} // namespace snd
} // namespace nw4r

#endif
