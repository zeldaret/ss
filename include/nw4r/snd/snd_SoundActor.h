#ifndef NW4R_SND_SOUND_ACTOR_H
#define NW4R_SND_SOUND_ACTOR_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_ExternalSoundPlayer.h>
#include <nw4r/snd/snd_SoundStartable.h>

#include <climits>

namespace nw4r {
namespace snd {
namespace detail {

class SoundActor : public SoundStartable {
public:
    explicit SoundActor(SoundStartable& rStartable) : mStartable(rStartable) {
        detail_GetActorSoundPlayer(0)->SetPlayableSoundCount(INT_MAX);
    }

    virtual StartResult
    detail_SetupSound(SoundHandle* pHandle, u32 id,
                      detail::BasicSound::AmbientArgInfo* pArgInfo,
                      detail::ExternalSoundPlayer* pPlayer, bool hold,
                      const StartInfo* pStartInfo); // at 0xC

    virtual u32 detail_ConvertLabelStringToSoundId(const char* pLabel) {
        return mStartable.detail_ConvertLabelStringToSoundId(pLabel);
    } // at 0x10

    ExternalSoundPlayer* detail_GetActorSoundPlayer(int i) {
        if (i < 0 || i >= ACTOR_PLAYER_COUNT) {
            return NULL;
        }

        return &mActorPlayer[i];
    }

    template <typename TForEachFunc>
    TForEachFunc ForEachSound(TForEachFunc pFunction, bool reverse) {
        int i;
        ExternalSoundPlayer* pPlayer = detail_GetActorSoundPlayer(0);

        for (i = 0; i < ACTOR_PLAYER_COUNT; i++) {
            pPlayer->ForEachSound(pFunction, reverse);
            pPlayer++;
        }

        return pFunction;
    }

private:
    static const int ACTOR_PLAYER_COUNT = 8;

private:
    SoundStartable& mStartable;                           // at 0x4
    ExternalSoundPlayer mActorPlayer[ACTOR_PLAYER_COUNT]; // at 0x8
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
