#ifndef NW4R_SND_SOUND_PLAYER_H
#define NW4R_SND_SOUND_PLAYER_H
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_PlayerHeap.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h"
#include "rvl/OS.h"   // IWYU pragma: export
#include "rvl/WPAD.h" // IWYU pragma: export

namespace nw4r {
namespace snd {

// Forward declarations
namespace detail {
class ExternalSoundPlayer;
class SeqSound;
class StrmSound;
class WaveSound;
template <typename T>
class SoundInstanceManager;
} // namespace detail

class SoundPlayer {
public:
    SoundPlayer();
    ~SoundPlayer();

    void Update();

    void StopAllSound(int frames);
    void PauseAllSound(bool flag, int frames);

    void SetVolume(f32 volume);

    int detail_GetOutputLine() const;
    bool detail_IsEnabledOutputLine() const;

    f32 detail_GetRemoteOutVolume(int i) const;

    void detail_InsertSoundList(detail::BasicSound *pSound);
    void detail_RemoveSoundList(detail::BasicSound *pSound);

    void detail_InsertPriorityList(detail::BasicSound *pSound);
    void detail_RemovePriorityList(detail::BasicSound *pSound);

    void detail_SortPriorityList();
    void detail_SortPriorityList(detail::BasicSound *pSound);

    detail::SeqSound *detail_AllocSeqSound(
        int priority, int startPriority, detail::BasicSound::AmbientInfo *pArgInfo,
        detail::ExternalSoundPlayer *pExtPlayer, u32 id, detail::SoundInstanceManager<detail::SeqSound> *pManager
    );

    detail::StrmSound *detail_AllocStrmSound(
        int priority, int startPriority, detail::BasicSound::AmbientInfo *pArgInfo,
        detail::ExternalSoundPlayer *pExtPlayer, u32 id, detail::SoundInstanceManager<detail::StrmSound> *pManager
    );

    detail::WaveSound *detail_AllocWaveSound(
        int priority, int startPriority, detail::BasicSound::AmbientInfo *pArgInfo,
        detail::ExternalSoundPlayer *pExtPlayer, u32 id, detail::SoundInstanceManager<detail::WaveSound> *pManager
    );

    int CalcPriorityReduction(detail::BasicSound::AmbientInfo *pArgInfo, u32 id);

    void InitAmbientArg(detail::BasicSound *pSound, detail::BasicSound::AmbientInfo *pArgInfo);

    void SetLpfFreq(f32 freq);
    f32 GetRemoteOutVolume(int remote) const;
    void SetFxSend(AuxBus bus, f32 value);

    void SetPlayableSoundCount(int count);
    void detail_SetPlayableSoundLimit(int limit);
    bool detail_CanPlaySound(int startPriority);

    bool CheckPlayableSoundCount(int startPriority, detail::ExternalSoundPlayer *pExtPlayer);

    bool detail_AppendSound(detail::BasicSound *pSound);
    void detail_RemoveSound(detail::BasicSound *pSound);

    void detail_AppendPlayerHeap(detail::PlayerHeap *pHeap);
    detail::PlayerHeap *detail_AllocPlayerHeap(detail::BasicSound *pSound);
    void detail_FreePlayerHeap(detail::BasicSound *pSound);

    int GetPlayingSoundCount() const {
        return mSoundList.GetSize();
    }
    int GetPlayableSoundCount() const {
        return mPlayableCount;
    }

    f32 GetVolume() const {
        return mVolume;
    }

    detail::BasicSound *detail_GetLowestPrioritySound() {
        // @bug UB when the list is empty
        return &mPriorityList.GetFront();
    }

    f32 detail_GetMainOutVolume() const {
        return mMainOutVolume;
    }

    int detail_GetOutputLineFlag() const {
        return mOutputLineFlag;
    }

private:
    detail::BasicSoundPlayerPlayList mSoundList;    // at 0x0
    detail::BasicSoundPlayerPrioList mPriorityList; // at 0xC
    detail::PlayerHeapList mHeapList;               // at 0x18

    int mPlayableCount; // at 0x24
    int mPlayableLimit; // at 0x28

    f32 mVolume;                                // at 0x2C
    f32 mLpfFreq;                               // at 0x30
    int mOutputLineFlag;                        // at 0x34
    f32 mMainOutVolume;                         // at 0x38
    int mBiquadType;                            // at 0x3C
    f32 mBiquadValue;                           // at 0x40
    f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS]; // at 0x44
    f32 mMainSend;                              // at 0x54
    f32 mFxSend[AUX_BUS_NUM];                   // at 0x58
};

} // namespace snd
} // namespace nw4r

#endif
