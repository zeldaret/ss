#ifndef NW4R_SND_EXTERNAL_SOUND_PLAYER_H
#define NW4R_SND_EXTERNAL_SOUND_PLAYER_H
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/types_nw4r.h"


namespace nw4r {
namespace snd {
namespace detail {

class ExternalSoundPlayer {
public:
    ExternalSoundPlayer();
    ~ExternalSoundPlayer();

    int GetPlayableSoundCount() const {
        return mPlayableCount;
    }
    void SetPlayableSoundCount(int count);
    void DetachSoundActorAll(SoundActor*);
    void StopAllSound(int);
    void PauseAllSound(bool, int);

    int GetPlayingSoundCount() const {
        return mSoundList.GetSize();
    }

    f32 detail_GetVolume() const {
        // TODO what happened to this?
    }
    BasicSound *GetLowestPrioritySound();

    void InsertSoundList(BasicSound *pSound);
    void RemoveSoundList(BasicSound *pSound);

    bool AppendSound(BasicSound *pSound);
    void RemoveSound(BasicSound *pSound);

    bool detail_CanPlaySound(int priority);

    template <typename TForEachFunc>
    TForEachFunc ForEachSound(TForEachFunc pFunction, bool reverse) {
        if (reverse) {
            BasicSoundExtPlayList::RevIterator it = mSoundList.GetBeginReverseIter();

            while (it != mSoundList.GetEndReverseIter()) {
                BasicSoundExtPlayList::RevIterator curr = it;

                SoundHandle handle;
                handle.detail_AttachSoundAsTempHandle(&*curr);
                pFunction(handle);

                if (handle.IsAttachedSound()) {
                    ++it;
                }
            }
        } else {
            NW4R_UT_LIST_SAFE_FOREACH(mSoundList, SoundHandle handle; handle.detail_AttachSoundAsTempHandle(&*it);
                                      pFunction(handle););
        }

        return pFunction;
    }

private:
    BasicSoundExtPlayList mSoundList; // at 0x0
    u32 mPlayableCount;               // at 0xC
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
