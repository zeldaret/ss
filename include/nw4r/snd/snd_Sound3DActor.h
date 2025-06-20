
#ifndef NW4R_SND_SOUND_3D_ACTOR_H
#define NW4R_SND_SOUND_3D_ACTOR_H

#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundActor.h"
#include "nw4r/types_nw4r.h"

#include "nw4r/math.h"

namespace nw4r {
namespace snd {

class Sound3DActor : public SoundActor, public detail::BasicSound::AmbientInfo::AmbientArgUpdateCallback {
public:
    Sound3DActor(SoundArchivePlayer &rPlayer, Sound3DManager &rManager);
    virtual ~Sound3DActor();

    virtual StartResult SetupSound(SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *) override;

    // AmbientArgUpdateCallback
    virtual void at_0x0c(void *arg, detail::BasicSound* sound) override;

    const nw4r::math::VEC3 &GetPosition() const {
        return mPosition;
    }

    const nw4r::math::VEC3 &GetVelocity() const {
        return mVelocity;
    }
    void SetPosition(const math::VEC3 &rPosition);

    void SetUserParam(u32 userParam) {
        mUserParam = userParam;
    }

private:
    static void ClearUpdateCallback(SoundHandle &rHandle);

private:
    Sound3DManager &m3DManager;              // at 0x58
    SoundArchivePlayer *mSoundArchivePlayer; // at 0x5C
    u32 mUserParam;                          // at 0x60
    math::VEC3 mPosition;                    // at 0x64
    math::VEC3 mVelocity;                    // at 0x70
    bool mSkipVelocityUpdate;                // at 0x7C
};

} // namespace snd
} // namespace nw4r

#endif
