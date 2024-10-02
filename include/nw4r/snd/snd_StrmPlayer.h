#ifndef NW4R_SND_STRM_PLAYER_H
#define NW4R_SND_STRM_PLAYER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicPlayer.h>
#include <nw4r/snd/snd_InstancePool.h>
#include <nw4r/snd/snd_SoundThread.h>
#include <nw4r/snd/snd_StrmChannel.h>
#include <nw4r/snd/snd_StrmFile.h>
#include <nw4r/snd/snd_Task.h>
#include <nw4r/snd/snd_Voice.h>

#include <nw4r/ut.h>

#include <rvl/OS.h>

namespace nw4r {
namespace snd {
namespace detail {

class StrmPlayer : public BasicPlayer, public SoundThread::PlayerCallback {
    friend class StrmHeaderLoadTask;
    friend class StrmDataLoadTask;

public:
    enum StartOffsetType {
        START_OFFSET_TYPE_SAMPLE,
        START_OFFSET_TYPE_MILLISEC
    };

    struct StrmHeader {
        // TODO: Why 8 if the player only supports 2???
        static const int STRM_CHANNEL_MAX = 8;

        StrmInfo strmInfo;                     // at 0x0
        AdpcmInfo adpcmInfo[STRM_CHANNEL_MAX]; // at 0x38
        u16 loopYn1[STRM_CHANNEL_MAX];         // at 0x1B8
        u16 loopYn2[STRM_CHANNEL_MAX];         // at 0x1D8
    };

public:
    StrmPlayer();
    virtual ~StrmPlayer(); // at 0x8

    virtual bool Start();          // at 0xc
    virtual void Stop();           // at 0x10
    virtual void Pause(bool flag); // at 0x14

    virtual bool IsActive() const {
        return mActiveFlag;
    } // at 0x18
    virtual bool IsStarted() const {
        return mStartedFlag;
    } // at 0x1c
    virtual bool IsPause() const {
        return mPauseFlag;
    }; // at 0x20

    virtual void OnUpdateFrameSoundThread() {
        Update();
    } // at 0xc
    virtual void OnUpdateVoiceSoundThread() {
        UpdateBuffer();
    } // at 0x10
    virtual void OnShutdownSoundThread() {
        Stop();
    } // at 0x14

    bool IsPrepared() const {
        return mPreparedFlag;
    }

    void SetTaskErrorFlag() {
        mTaskErrorFlag = true;
    }
    void SetTaskCancelFlag() {
        mTaskCancelFlag = true;
    }

    bool Setup(StrmBufferPool* pBufferPool);
    void Shutdown();

    bool Prepare(ut::FileStream* pFileStream, int voices,
                 StartOffsetType offsetType, int offset);
    void InitParam();

    void Update();

private:
    struct StrmHeaderLoadTask : public Task {
        StrmHeaderLoadTask();

        virtual void Execute();  // at 0xC
        virtual void Cancel();   // at 0x10
        virtual void OnCancel(); // at 0x14

        StrmPlayer* strmPlayer;          // at 0x10
        ut::FileStream* fileStream;      // at 0x14
        StartOffsetType startOffsetType; // at 0x18
        s32 startOffset;                 // at 0x1C
    };

    struct StrmDataLoadTask : public Task {
        StrmDataLoadTask();

        virtual void Execute();  // at 0xC
        virtual void Cancel();   // at 0x10
        virtual void OnCancel(); // at 0x14

        StrmPlayer* strmPlayer;     // at 0x10
        ut::FileStream* fileStream; // at 0x14
        u32 size;                   // at 0x18
        s32 offset;                 // at 0x1C
        u32 blockSize;              // at 0x20
        s32 bufferBlockIndex;       // at 0x24
        bool needUpdateAdpcmLoop;   // at 0x28

        NW4R_UT_LIST_NODE_DECL(); // at 0x2C
    };

    NW4R_UT_LIST_TYPEDEF_DECL(StrmDataLoadTask);

    static const int DATA_BLOCK_COUNT_MIN = 4;
    static const int DATA_BLOCK_COUNT_MAX = 32;
    static const int DATA_BLOCK_SIZE_MAX = 0x2000;

    // TODO: How is this calculated?
    static const int LOAD_BUFFER_SIZE = 0x4000 + 32;

private:
    bool LoadHeader(ut::FileStream* pFileStream, StartOffsetType offsetType,
                    int offset);
    bool LoadStreamData(ut::FileStream* pFileStream, int offset, u32 size,
                        u32 blockSize, int blockIndex,
                        bool needUpdateAdpcmLoop);

    bool SetupPlayer(const StrmHeader* pStrmHeader);

    bool AllocChannels(int channels, int voices);
    void FreeChannels();

    void UpdateBuffer();
    void UpdateLoopAddress(u32 startSample, u32 endSample);
    void UpdatePlayingBlockIndex();
    void UpdateDataLoopAddress(s32 endBlock);
    void SetLoopEndToZeroBuffer(int endBlock);
    void UpdateLoadingBlockIndex();
    void UpdatePauseStatus();

    int CalcLoadingBufferBlockCount() const;
    bool CalcStartOffset(s32* pBlockIndex, u32* pBlockOffset, s32* pLoopCount);

    static void VoiceCallbackFunc(Voice* pDropVoice,
                                  Voice::VoiceCallbackStatus status,
                                  void* pCallbackArg);

    void SetAdpcmLoopContext(int channels, u16* pPredScale);

private:
    StrmInfo mStrmInfo; // at 0x80

    bool mSetupFlag;           // at 0xB8
    bool mActiveFlag;          // at 0xB9
    bool mStartedFlag;         // at 0xBA
    bool mPreparedFlag;        // at 0xBB
    bool mTaskErrorFlag;       // at 0xBC
    bool mTaskCancelFlag;      // at 0xBD
    bool mLoadingDelayFlag;    // at 0xBE
    bool mPauseFlag;           // at 0xBF
    bool mPauseStatus;         // at 0xC0
    bool mLoadWaitFlag;        // at 0xC1
    bool mNoRealtimeLoadFlag;  // at 0xC2
    bool mSkipUpdateAdpcmLoop; // at 0xC3
    bool mValidAdpcmLoop;      // at 0xC4
    bool mPlayFinishFlag;      // at 0xC5
    bool mLoadFinishFlag;      // at 0xC6

    s32 mLoopCounter;    // at 0xC8
    int mPrepareCounter; // at 0xCC

    int mChangeNumBlocks;      // at 0xD0
    int mDataBlockSize;        // at 0xD4
    int mBufferBlockCount;     // at 0xD8
    int mBufferBlockCountBase; // at 0xDC

    int mLoadingBufferBlockCount; // at 0xE0
    int mLoadingBufferBlockIndex; // at 0xE4
    int mLoadingDataBlockIndex;   // at 0xE8

    int mPlayingBufferBlockCount; // at 0xEC
    int mPlayingBufferBlockIndex; // at 0xF0
    int mPlayingDataBlockIndex;   // at 0xF4
    int mLoopStartBlockIndex;     // at 0xF8
    int mLastBlockIndex;          // at 0xFC

    StartOffsetType mStartOffsetType; // at 0x100
    int mStartOffset;                 // at 0x104

    StrmHeaderLoadTask mStrmHeaderLoadTask;                       // at 0x108
    StrmDataLoadTaskList mStrmDataLoadTaskList;                   // at 0x128
    InstancePool<StrmDataLoadTask> mStrmDataLoadTaskPool;         // at 0x134
    StrmDataLoadTask mStrmDataLoadTaskArea[DATA_BLOCK_COUNT_MAX]; // at 0x138

    StrmBufferPool* mBufferPool; // at 0x7B8
    ut::FileStream* mFileStream; // at 0x7BC
    Voice* mVoice;               // at 0x7C0
    s32 mChannelCount;           // at 0x7C4
    s32 mVoiceOutCount;          // at 0x7C8

    StrmChannel mChannels[CHANNEL_MAX];   // at 0x7CC
    u16 mAdpcmLoopPredScale[CHANNEL_MAX]; // at 0x83C
    u16 mAdpcmLoopYn1[CHANNEL_MAX];       // at 0x840
    u16 mAdpcmLoopYn2[CHANNEL_MAX];       // at 0x844

    static u8 sLoadBuffer[LOAD_BUFFER_SIZE] ALIGN(32);
    static OSMutex sLoadBufferMutex;

    static bool sStaticInitFlag;
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
