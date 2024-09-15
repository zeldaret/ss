
#ifndef NW4R_SND_SOUND_3D_ACTOR_H
#define NW4R_SND_SOUND_3D_ACTOR_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicSound.h>
#include <nw4r/snd/snd_SoundActor.h>

#include <nw4r/math.h>

namespace nw4r {
namespace snd {

class Sound3DActor : public detail::SoundActor,
                     public detail::BasicSound::AmbientArgUpdateCallback {
public:
    Sound3DActor(SoundArchivePlayer& rPlayer, Sound3DManager& rManager);
    virtual ~Sound3DActor(); // at 0x8

    virtual StartResult
    detail_SetupSound(SoundHandle* pHandle, u32 id,
                      detail::BasicSound::AmbientArgInfo* pArgInfo,
                      detail::ExternalSoundPlayer* pPlayer, bool hold,
                      const StartInfo* pStartInfo); // at 0xC

    virtual void detail_Update(void* pArg,
                               const detail::BasicSound* pSound); // at 0xC

    void SetPosition(const math::VEC3& rPosition);

private:
    static void ClearUpdateCallback(SoundHandle& rHandle);

private:
    Sound3DManager& m3DManager;              // at 0xAC
    SoundArchivePlayer& mSoundArchivePlayer; // at 0xB0
    u32 mUserParam;                          // at 0xB4
    math::VEC3 mPosition;                    // at 0xB8
};

} // namespace snd
} // namespace nw4r

#endif
