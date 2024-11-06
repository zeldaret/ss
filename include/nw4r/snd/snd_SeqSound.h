#ifndef NW4R_SND_SEQ_SOUND_H
#define NW4R_SND_SEQ_SOUND_H
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SeqPlayer.h"
#include "nw4r/snd/snd_Task.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h" // IWYU pragma: export
#include "rvl/OS.h"  // IWYU pragma: export

namespace nw4r {
namespace snd {

// Forward declarations
class SeqSoundHandle;

namespace detail {

// Forward declarations
template <typename T>
class SoundInstanceManager;

class SeqSound : public BasicSound {
    friend class SeqSoundHandle;

public:
    NW4R_UT_RTTI_DECL(SeqSound);

public:
    explicit SeqSound(SoundInstanceManager<SeqSound> *pManager, int priority, int ambientPriority);

    virtual void Shutdown() override; // at 0x10
    virtual bool IsPrepared() override const {
        return mPreparedFlag;
    } // at 0x14

    void SetPlayerPriority(int priority) override;

    virtual bool IsAttachedTempSpecialHandle() override; // at 0x18
    virtual void DetachTempSpecialHandle() override;     // at 0x1C

    virtual void InitParam() override; // at 0x20

    virtual BasicPlayer &GetBasicPlayer() override {
        return mSeqPlayer;
    } // at 0x24
    virtual const BasicPlayer &GetBasicPlayer() override const {
        return mSeqPlayer;
    } // at 0x28

    virtual void OnUpdatePlayerPriority() override; // at 0x2C

    SeqPlayer::SetupResult
    Setup(SeqTrackAllocator *pAllocator, u32 allocTrackFlags, NoteOnCallback *pCallback);

    void Prepare(const void *pBase, s32 seqOffset, SeqPlayer::OffsetType startType, int startOffset);

    void Prepare(ut::FileStream *pStream, s32 seqOffset, SeqPlayer::OffsetType startType, int startOffset);

    void Skip(SeqPlayer::OffsetType offsetType, int offset);

    void SetTempoRatio(f32 tempo);
    void SetChannelPriority(int priority);
    void SetReleasePriorityFix(bool flag);

    void SetSeqUserprocCallback(SeqUserprocCallback cb, void *cbArg);
    void SetTrackMute(u32 trackFlags, SeqMute mute);
    void SetTrackSilence(u32 trackFlags, bool b, int i);
    bool ReadVariable(int, s16 *outValue) const;

    void SetTrackVolume(u32 trackFlags, f32 volume);
    void SetTrackPitch(u32 trackFlags, f32 pitch);

    bool WriteTrackVariable(int track, int i, s16 value);
    bool WriteVariable(int i, s16 value);
    static bool WriteGlobalVariable(int i, s16 value);

    u32 GetTick() const;

    void *GetFileStreamBuffer() {
        return mFileStreamBuffer;
    }
    s32 GetFileStreamBufferSize() {
        return sizeof(mFileStreamBuffer);
    }

private:
    typedef void (*SeqLoadCallback)(bool success, const void *pBase, void *pCallbackArg);

    struct SeqLoadTask : public Task {
        SeqLoadTask();

        virtual void Execute();  // at 0xC
        virtual void Cancel();   // at 0x10
        virtual void OnCancel(); // at 0x14

        ut::FileStream *fileStream; // at 0x10
        void *buffer;               // at 0x14
        int bufferSize;             // at 0x18
        SeqLoadCallback callback;   // at 0x1C
        SeqSound *callbackData;     // at 0x20
    };

    static const int FILE_STREAM_BUFFER_SIZE = 512;

private:
    bool LoadData(SeqLoadCallback pCalllback, void *pCallbackArg);

    static void NotifyLoadAsyncEndSeqData(bool success, const void *pBase, void *pCallbackArg);

private:
    SeqPlayer mSeqPlayer;                     // at 0x110
    SeqSoundHandle *mTempSpecialHandle;       // at 0x274
    SoundInstanceManager<SeqSound> *mManager; // at 0x278

    s32 mSeqOffset;                         // at 0x27C
    SeqPlayer::OffsetType mStartOffsetType; // at 0x280
    int mStartOffset;                       // at 0x284

    volatile bool mLoadingFlag;  // at 0x288
    volatile bool mPreparedFlag; // at 0x289

    ut::FileStream *mFileStream;                     // at 0x28C
    char mFileStreamBuffer[FILE_STREAM_BUFFER_SIZE]; // at 0x290

    SeqLoadTask mSeqLoadTask; // at 0x490
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
