#ifndef NW4R_SND_SOUND_ACTOR_H
#define NW4R_SND_SOUND_ACTOR_H
#include "limits.h"
#include "nw4r/snd/snd_ExternalSoundPlayer.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/types_nw4r.h"

namespace nw4r {
namespace snd {

class SoundActor : public SoundStartable {
public:
    explicit SoundActor(SoundArchivePlayer &rPlayer);
    virtual ~SoundActor();

    virtual StartResult detail_SetupSound(
        SoundHandle *pHandle, u32 id, bool hold,
        const StartInfo *pStartInfo
    ) override; // at 0xC

    virtual u32 detail_ConvertLabelStringToSoundId(const char *pLabel) override {
        return mSoundArchivePlayer.detail_ConvertLabelStringToSoundId(pLabel);
    } // at 0x10

    virtual StartResult
    SetupSound(nw4r::snd::SoundHandle *pHandle, u32 id, const nw4r::snd::SoundStartable::StartInfo *pStartInfo, void *); // at 0x14

    virtual StartResult
    detail_SetupSoundWithAmbientInfo(nw4r::snd::SoundHandle *pHandle, u32 id, const nw4r::snd::SoundStartable::StartInfo *pStartInfo, detail::BasicSound::AmbientInfo *ambientArg, void *); // at 0x18

    void StopAllSound(int);
    void PauseAllSound(bool, int);
    int GetPlayingSoundCount(int i) const;

    detail::ExternalSoundPlayer *detail_GetActorSoundPlayer(int i) {
        if (i < 0 || i >= ACTOR_PLAYER_COUNT) {
            return NULL;
        }

        return &mActorPlayer[i];
    }

    template <typename TForEachFunc>
    TForEachFunc ForEachSound(TForEachFunc pFunction, bool reverse) {
        for (int i = 0; i < ACTOR_PLAYER_COUNT; i++) {
            mActorPlayer[i].ForEachSound(pFunction, reverse);
        }

        return pFunction;
    }

private:
    static const int ACTOR_PLAYER_COUNT = 4;

private:
    SoundArchivePlayer &mSoundArchivePlayer;                      // at 0x4
    detail::ExternalSoundPlayer mActorPlayer[ACTOR_PLAYER_COUNT]; // at 0x8
    detail::SoundActorParam mActorParam;                                  // at 0x48
};

} // namespace snd
} // namespace nw4r

#endif
