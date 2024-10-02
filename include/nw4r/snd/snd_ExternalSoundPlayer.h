#ifndef NW4R_SND_EXTERNAL_SOUND_PLAYER_H
#define NW4R_SND_EXTERNAL_SOUND_PLAYER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicSound.h>

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

    int GetPlayingSoundCount() const {
        return mSoundList.GetSize();
    }

    f32 detail_GetVolume() const {
        return mVolume;
    }
    BasicSound* GetLowestPrioritySound();

    void InsertSoundList(BasicSound* pSound);
    void RemoveSoundList(BasicSound* pSound);

    template <typename TForEachFunc>
    TForEachFunc ForEachSound(TForEachFunc pFunction, bool reverse) {
        if (reverse) {
            BasicSoundExtPlayList::RevIterator it =
                mSoundList.GetBeginReverseIter();

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
            NW4R_UT_LIST_SAFE_FOREACH(mSoundList,
                SoundHandle handle;
                handle.detail_AttachSoundAsTempHandle(&*it);
                pFunction(handle);
            );
        }

        return pFunction;
    }

private:
    BasicSoundExtPlayList mSoundList; // at 0x0
    u16 mPlayableCount;               // at 0xC
    f32 mVolume;                      // at 0x10
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
