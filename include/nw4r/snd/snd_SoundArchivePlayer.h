#ifndef NW4R_SND_SOUND_ARCHIVE_PLAYER_H
#define NW4R_SND_SOUND_ARCHIVE_PLAYER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_DisposeCallback.h>
#include <nw4r/snd/snd_MmlParser.h>
#include <nw4r/snd/snd_MmlSeqTrackAllocator.h>
#include <nw4r/snd/snd_NoteOnCallback.h>
#include <nw4r/snd/snd_SeqSound.h>
#include <nw4r/snd/snd_SoundArchive.h>
#include <nw4r/snd/snd_SoundInstanceManager.h>
#include <nw4r/snd/snd_SoundStartable.h>
#include <nw4r/snd/snd_StrmChannel.h>
#include <nw4r/snd/snd_StrmSound.h>
#include <nw4r/snd/snd_Util.h>
#include <nw4r/snd/snd_WaveSound.h>
#include <nw4r/snd/snd_WsdPlayer.h>

namespace nw4r {
namespace snd {

// Forward declarations
namespace detail {
class SeqTrackAllocator;
}
class SoundMemoryAllocatable;
class SoundPlayer;

class SoundArchivePlayer_FileManager {
public:
    virtual const void* GetFileAddress(u32 id) = 0;         // at 0x8
    virtual const void* GetFileWaveDataAddress(u32 id) = 0; // at 0x8
};

class SoundArchivePlayer : public detail::DisposeCallback,
                           public SoundStartable {
public:
    SoundArchivePlayer();
    virtual ~SoundArchivePlayer(); // at 0x8

    virtual void InvalidateData(const void* pStart,
                                const void* pEnd); // at 0xC

    virtual void InvalidateWaveData(const void* pStart,
                                    const void* pEnd); // at 0x10

    virtual StartResult
    detail_SetupSound(SoundHandle* pHandle, u32 id,
                      detail::BasicSound::AmbientArgInfo* pArgInfo,
                      detail::ExternalSoundPlayer* pPlayer, bool hold,
                      const StartInfo* pStartInfo); // at 0x28

    virtual u32 detail_ConvertLabelStringToSoundId(const char* pLabel) {
        return mSoundArchive->ConvertLabelStringToSoundId(pLabel);
    } // at 0x2C

    bool IsAvailable() const;

    bool Setup(const SoundArchive* pArchive, void* pMramBuffer,
               u32 mramBufferSize, void* pStrmBuffer, u32 strmBufferSize);

    void Shutdown();

    u32 GetRequiredMemSize(const SoundArchive* pArchive);
    u32 GetRequiredStrmBufferSize(const SoundArchive* pArchive);

    void Update();

    const SoundArchive& GetSoundArchive() const;

    SoundPlayer& GetSoundPlayer(u32 i);
    SoundPlayer& GetSoundPlayer(int i) {
        return GetSoundPlayer(static_cast<u32>(i));
    }

    const void* detail_GetFileAddress(u32 id) const;
    const void* detail_GetFileWaveDataAddress(u32 id) const;

    const void* GetGroupAddress(u32 id) const;
    void SetGroupAddress(u32 id, const void* pAddr);

    const void* GetGroupWaveDataAddress(u32 id) const;
    void SetGroupWaveDataAddress(u32 id, const void* pAddr);

    bool LoadGroup(u32 id, SoundMemoryAllocatable* pAllocatable, u32 blockSize);
    bool LoadGroup(const char* pLabel, SoundMemoryAllocatable* pAllocatable,
                   u32 blockSize);

    bool LoadGroup(int id, SoundMemoryAllocatable* pAllocatable,
                   u32 blockSize) {
        return LoadGroup(static_cast<u32>(id), pAllocatable, blockSize);
    }
    bool LoadGroup(unsigned int id, SoundMemoryAllocatable* pAllocatable,
                   u32 blockSize) {
        return LoadGroup(static_cast<u32>(id), pAllocatable, blockSize);
    }

    u32 GetSoundPlayerCount() const {
        return mSoundPlayerCount;
    }

    u32 GetFreeSeqSoundCount() const {
        return mSeqSoundInstanceManager.GetFreeCount();
    }
    u32 GetFreeStrmSoundCount() const {
        return mStrmSoundInstanceManager.GetFreeCount();
    }
    u32 GetFreeWaveSoundCount() const {
        return mWaveSoundInstanceManager.GetFreeCount();
    }

private:
    struct Group {
        const void* address;         // at 0x0
        const void* waveDataAddress; // at 0x4
    };

    typedef detail::Util::Table<Group> GroupTable;

    class SeqNoteOnCallback : public detail::NoteOnCallback {
    public:
        explicit SeqNoteOnCallback(const SoundArchivePlayer& rPlayer)
            : mSoundArchivePlayer(rPlayer) {}

        virtual detail::Channel*
        NoteOn(detail::SeqPlayer* pPlayer, int bankNo,
               const detail::NoteOnInfo& rInfo); // at 0xC

    private:
        const SoundArchivePlayer& mSoundArchivePlayer; // at 0x0
    };

    class WsdCallback : public detail::WsdPlayer::WsdCallback {
    public:
        explicit WsdCallback(const SoundArchivePlayer& rPlayer)
            : mSoundArchivePlayer(rPlayer) {}

        virtual bool GetWaveSoundData(detail::WaveSoundInfo* pSoundInfo,
                                      detail::WaveSoundNoteInfo* pNoteInfo,
                                      detail::WaveData* pWaveData,
                                      const void* pWsdData, int index,
                                      int noteIndex,
                                      u32 callbackArg) const; // at 0xC

    private:
        const SoundArchivePlayer& mSoundArchivePlayer; // at 0x0
    };

private:
    bool SetupMram(const SoundArchive* pArchive, void* pBuffer, u32 bufferSize);

    detail::PlayerHeap* CreatePlayerHeap(void* pBuffer, u32 bufferSize);

    bool SetupSoundPlayer(const SoundArchive* pArchive, void** ppBuffer,
                          void* pEnd);

    bool CreateGroupAddressTable(const SoundArchive* pArchive, void** ppBuffer,
                                 void* pEnd);

    bool SetupSeqSound(const SoundArchive* pArchive, int sounds,
                       void** ppBuffer, void* pEnd);
    bool SetupWaveSound(const SoundArchive* pArchive, int sounds,
                        void** ppBuffer, void* pEnd);
    bool SetupStrmSound(const SoundArchive* pArchive, int sounds,
                        void** ppBuffer, void* pEnd);
    bool SetupSeqTrack(const SoundArchive* pArchive, int tracks,
                       void** ppBuffer, void* pEnd);
    bool SetupStrmBuffer(const SoundArchive* pArchive, void* pBuffer,
                         u32 bufferSize);

    StartResult
    PrepareSeqImpl(detail::SeqSound* pSound,
                   const SoundArchive::SoundInfo* pSndInfo,
                   const SoundArchive::SeqSoundInfo* pSeqInfo,
                   SoundStartable::StartInfo::StartOffsetType startType,
                   int startOffset, int voices);

    StartResult
    PrepareStrmImpl(detail::StrmSound* pSound,
                    const SoundArchive::SoundInfo* pSndInfo,
                    const SoundArchive::StrmSoundInfo* pStrmInfo,
                    SoundStartable::StartInfo::StartOffsetType startType,
                    int startOffset, int voices);

    StartResult
    PrepareWaveSoundImpl(detail::WaveSound* pSound,
                         const SoundArchive::SoundInfo* pSndInfo,
                         const SoundArchive::WaveSoundInfo* pWsdInfo,
                         SoundStartable::StartInfo::StartOffsetType startType,
                         int startOffset, int voices);

private:
    const SoundArchive* mSoundArchive;            // at 0x10
    GroupTable* mGroupTable;                      // at 0x14
    SoundArchivePlayer_FileManager* mFileManager; // at 0x18

    detail::SeqTrackAllocator* mSeqTrackAllocator; // at 0x1C
    SeqNoteOnCallback mSeqCallback;                // at 0x20
    WsdCallback mWsdCallback;                      // at 0x28

    u32 mSoundPlayerCount;      // at 0x30
    SoundPlayer* mSoundPlayers; // at 0x34

    detail::SoundInstanceManager<detail::SeqSound>
        mSeqSoundInstanceManager; // at 0x38

    detail::SoundInstanceManager<detail::StrmSound>
        mStrmSoundInstanceManager; // at 0x60

    detail::SoundInstanceManager<detail::WaveSound>
        mWaveSoundInstanceManager; // at 0x88

    detail::MmlSeqTrackAllocator mMmlSeqTrackAllocator; // at 0xB0
    detail::StrmBufferPool mStrmBufferPool;             // at 0xBC
    detail::MmlParser mMmlParser;                       // at 0xD4

    void* mSetupBufferAddress; // at 0xD8
    u32 mSetupBufferSize;      // at 0xDC
};

} // namespace snd
} // namespace nw4r

#endif
