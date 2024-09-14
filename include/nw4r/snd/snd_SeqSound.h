#ifndef NW4R_SND_SEQ_SOUND_H
#define NW4R_SND_SEQ_SOUND_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicSound.h>
#include <nw4r/snd/snd_SeqPlayer.h>
#include <nw4r/snd/snd_Task.h>

#include <nw4r/ut.h>

#include <rvl/OS.h>

namespace nw4r {
namespace snd {

// Forward declarations
class SeqSoundHandle;

namespace detail {

// Forward declarations
template <typename T> class SoundInstanceManager;

class SeqSound : public BasicSound {
    friend class SeqSoundHandle;

public:
    NW4R_UT_RTTI_DECL(SeqSound);

public:
    explicit SeqSound(SoundInstanceManager<SeqSound>* pManager);

    virtual void Shutdown(); // at 0x28
    virtual bool IsPrepared() const {
        return mPreparedFlag;
    } // at 0x2C

    virtual void SetPlayerPriority(int priority); // at 0x4C

    virtual bool IsAttachedTempSpecialHandle(); // at 0x5C
    virtual void DetachTempSpecialHandle();     // at 0x60

    virtual void InitParam(); // at 0x64

    virtual BasicPlayer& GetBasicPlayer() {
        return mSeqPlayer;
    } // at 0x68
    virtual const BasicPlayer& GetBasicPlayer() const {
        return mSeqPlayer;
    } // at 0x6C

    SeqPlayer::SetupResult Setup(SeqTrackAllocator* pAllocator,
                                 u32 allocTrackFlags, int voices,
                                 NoteOnCallback* pCallback);

    void Prepare(const void* pBase, s32 seqOffset,
                 SeqPlayer::OffsetType startType, int startOffset);

    void Prepare(ut::FileStream* pStream, s32 seqOffset,
                 SeqPlayer::OffsetType startType, int startOffset);

    void Skip(SeqPlayer::OffsetType offsetType, int offset);

    void SetTempoRatio(f32 tempo);
    void SetChannelPriority(int priority);
    void SetReleasePriorityFix(bool flag);

    void SetTrackVolume(u32 trackFlags, f32 volume);
    void SetTrackPitch(u32 trackFlags, f32 pitch);

    bool WriteVariable(int i, s16 value);
    static bool WriteGlobalVariable(int i, s16 value);

    void* GetFileStreamBuffer() {
        return mFileStreamBuffer;
    }
    s32 GetFileStreamBufferSize() {
        return sizeof(mFileStreamBuffer);
    }

private:
    typedef void (*SeqLoadCallback)(bool success, const void* pBase,
                                    void* pCallbackArg);

    struct SeqLoadTask : public Task {
        SeqLoadTask();

        virtual void Execute();  // at 0xC
        virtual void Cancel();   // at 0x10
        virtual void OnCancel(); // at 0x14

        ut::FileStream* fileStream; // at 0x10
        void* buffer;               // at 0x14
        int bufferSize;             // at 0x18
        SeqLoadCallback callback;   // at 0x1C
        SeqSound* callbackData;     // at 0x20
    };

    static const int FILE_STREAM_BUFFER_SIZE = 512;

private:
    bool LoadData(SeqLoadCallback pCalllback, void* pCallbackArg);

    static void NotifyLoadAsyncEndSeqData(bool success, const void* pBase,
                                          void* pCallbackArg);

private:
    SeqPlayer mSeqPlayer;                     // at 0xD8
    SeqSoundHandle* mTempSpecialHandle;       // at 0x1F4
    SoundInstanceManager<SeqSound>* mManager; // at 0x1F8

    s32 mSeqOffset;                         // at 0x1FC
    SeqPlayer::OffsetType mStartOffsetType; // at 0x200
    int mStartOffset;                       // at 0x204

    bool mLoadingFlag;           // at 0x208
    volatile bool mPreparedFlag; // at 0x209

    ut::FileStream* mFileStream;                     // at 0x20C
    char mFileStreamBuffer[FILE_STREAM_BUFFER_SIZE]; // at 0x210

    SeqLoadTask mSeqLoadTask; // at 0x410
    mutable OSMutex mMutex;   // at 0x434
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
