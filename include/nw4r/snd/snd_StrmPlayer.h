#ifndef NW4R_SND_STRM_PLAYER_H
#define NW4R_SND_STRM_PLAYER_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_BasicPlayer.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/snd/snd_InstancePool.h"
#include "nw4r/snd/snd_Task.h"
#include "nw4r/snd/snd_SoundThread.h"
#include "nw4r/snd/snd_StrmChannel.h"
#include "nw4r/snd/snd_StrmFile.h"
#include "nw4r/snd/snd_Voice.h"

#include "nw4r/ut/ut_LinkList.h"

#include <rvl/OS/OSMutex.h>

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace ut { class FileStream; }}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd {

struct StrmDataInfo
{
	/* 0x00 */ bool loopFlag;
	/* 0x04 */ int sampleRate;
	/* 0x08 */ u32 loopStart;
	/* 0x0C */ u32 loopEnd;
};
	
namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2fcda
	class StrmPlayer : public BasicPlayer, public SoundThread::PlayerCallback
	{
	// enums
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f3ca
		enum StartOffsetType
		{
			START_OFFSET_TYPE_SAMPLE,
			START_OFFSET_TYPE_MILLISEC,
		};

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2fc67
		enum SetupResult
		{
			SETUP_SUCCESS,

			SETUP_ERR_CANNOT_ALLOCATE_BUFFER,

			SETUP_ERR_UNKNOWN
		};

	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2fb36
		struct StrmTrack
		{
			bool							mActiveFlag;	// size 0x01, offset 0x00
			Voice							*mVoice;		// size 0x04, offset 0x04
			StrmFileReader::StrmTrackInfo	mTrackInfo;		// size 0x28, offset 0x08
			f32								mVolume;		// size 0x04, offset 0x30
			f32								mPan;			// size 0x04, offset 0x34
		}; // size 0x38

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f430
		class StrmHeaderLoadTask : public Task
		{
		// methods
		public:
			// cdtors
			StrmHeaderLoadTask();

			// virtual function ordering
			// vtable Task
			virtual void Execute();
			virtual void Cancel();
			virtual void OnCancel();

		// members
		public:
			/* base Task */						// size 0x10, offset 0x00
			StrmPlayer		*player;			// size 0x04, offset 0x10
			ut::FileStream	*fileStream;		// size 0x04, offset 0x14
			StartOffsetType	startOffsetType;	// size 0x04, offset 0x18
			s32				startOffset;		// size 0x04, offset 0x1c
		}; // size 0x20

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f537
		class StrmDataLoadTask : public Task
		{
		// typedefs
		public:
			typedef ut::LinkList<StrmDataLoadTask, 0x2c> LinkList;

		// methods
		public:
			// cdtors
			StrmDataLoadTask();

			// virtual function ordering
			// vtable Task
			virtual void Execute();
			virtual void Cancel();
			virtual void OnCancel();

		// members
		public:
			/* base Task */								// size 0x10, offset 0x00
			StrmPlayer			*mStrmPlayer;			// size 0x04, offset 0x10
			ut::FileStream		*fileStream;			// size 0x04, offset 0x14
			u32					mSize;					// size 0x04, offset 0x18
			s32					mOffset;				// size 0x04, offset 0x1c
			u32					mBlockSize;				// size 0x04, offset 0x20
			s32					mBufferBlockIndex;		// size 0x04, offset 0x24
			bool				mNeedUpdateAdpcmLoop;	// size 0x01, offset 0x28
			/* 3 bytes padding */
			ut::LinkListNode	link;					// size 0x08, offset 0x2c
		}; // size 0x34

	// methods
	public:
		StrmPlayer();
		virtual ~StrmPlayer();

		// virtual function ordering
		// vtable BasicPlayer
		virtual bool Start();
		virtual void Stop();
		virtual void Pause(bool flag);
		virtual bool IsActive() const { return mActiveFlag; }
		virtual bool IsStarted() const { return mStartedFlag; }
		virtual bool IsPause() const { return mPauseFlag; }

		// vtable SoundThread::PlayerCallback
		virtual void OnUpdateFrameSoundThread() { Update(); }
		virtual void OnUpdateVoiceSoundThread() { UpdateBuffer(); }
		virtual void OnShutdownSoundThread() { Stop(); }

		// methods
		SetupResult Setup(StrmBufferPool *bufferPool, int allocChannelCount,
		                  u16 allocTrackFlag, int voiceOutCount);
		void Update();
		void Shutdown();

		bool IsPrepared() const { return mPreparedFlag; }
		StrmChannel *GetTrackChannel(StrmTrack const &track, int channelIndex);
		StrmTrack *GetPlayerTrack(int trackNo);

		void SetTaskErrorFlag() { mTaskErrorFlag = true; }
		void SetTaskCancelFlag() { mTaskCancelFlag = true; }

		bool IsAdpcm() const
		{
			return mStrmInfo.sampleFormat == SAMPLE_FORMAT_DSP_ADPCM;
		}
		bool CheckDiskDriveError() const;

		void SetAdpcmLoopContext(int channelNum, u16 *predScale);
		void SetTrackVolume(u32 trackBitFlag, f32 volume);
		u32 GetPlaySamplePosition() const;

		bool ReadStrmDataInfo(StrmDataInfo*) const;
		void InitParam();
		bool SetupPlayer();
		bool Prepare(ut::FileStream *fileStream,
		             StartOffsetType startOffsetType, int startOffset);

		bool LoadHeader(ut::FileStream *fileStream,
		                StartOffsetType startOffsetType, int startOffset);
		bool LoadStreamData(ut::FileStream *fileStream, int offset, u32 size,
		                    u32 blockSize, int bufferBlockIndex,
		                    bool needUpdateAdpcmLoop);

		bool AllocStrmBuffers();
		void FreeStrmBuffers();

		bool AllocVoices(int voiceOutCount);
		void FreeVoices();

		int CalcLoadingBufferBlockCount() const;
		bool CalcStartOffset(s32 *startBlockIndex, u32 *startBlockOffset,
		                     s32 *loopCount);

		void SetLoopEndToZeroBuffer(int endBufferBlockIndex);

		void UpdateBuffer();
		void UpdatePauseStatus();
		void UpdateLoadingBlockIndex();
		void UpdatePlayingBlockIndex();
		void UpdateLoopAddress(u32 loopStartSamples, u32 loopEndSamples);
		void UpdateDataLoopAddress(s32 endBlockBufferIndex);
		void UpdateVoiceParams(StrmTrack *track);

		static u32 GetSampleByByte(u32 byte, SampleFormat format);

	private:
		static void VoiceCallbackFunc(Voice *voice,
		                              Voice::VoiceCallbackStatus status,
		                              void *arg);

	// static members
	public:
		static int const LOAD_BUFFER_SIZE = 0x4000;
		static int const LOAD_BUFFER_CHANNEL_NUM;
		static int const BUFFER_BLOCK_COUNT_MAX = 32;
		static int const STRM_CHANNEL_NUM_PER_TRACK;
		static int const STRM_CHANNEL_NUM = 16;
		static int const DATA_BLOCK_SIZE_MAX = 0x2000;
		static int const STRM_TRACK_NUM = 8;

	private:
		static bool sStaticInitFlag;
		static OSMutex sLoadBufferMutex;
		static byte_t sLoadBuffer[LOAD_BUFFER_SIZE];

	// members
	private:
		/* base BasicPlayer */															// size 0x0a4, offset 0x000
		/* base SoundThread::PlayerCallback */											// size 0x00c, offset 0x0a4
		StrmFileReader::StrmInfo		mStrmInfo;										// size 0x040, offset 0x0b0
		bool							volatile mSetupFlag;							// size 0x001, offset 0x0f0
		bool							mActiveFlag;									// size 0x001, offset 0x0f1
		bool							mStartedFlag;									// size 0x001, offset 0x0f2
		bool							mPreparedFlag;									// size 0x001, offset 0x0f3
		bool							mTaskErrorFlag;									// size 0x001, offset 0x0f4
		bool							mTaskCancelFlag;								// size 0x001, offset 0x0f5
		bool							mLoadingDelayFlag;								// size 0x001, offset 0x0f6
		bool							mPauseFlag;										// size 0x001, offset 0x0f7
		bool							mPauseStatus;									// size 0x001, offset 0x0f8
		bool							mLoadWaitFlag;									// size 0x001, offset 0x0f9
		bool							mNoRealtimeLoadFlag;							// size 0x001, offset 0x0fa
		bool							mSkipUpdateAdpcmLoop;							// size 0x001, offset 0x0fb
		bool							mValidAdpcmLoop;								// size 0x001, offset 0x0fc
		bool							mPlayFinishFlag;								// size 0x001, offset 0x0fd
		bool							mLoadFinishFlag;								// size 0x001, offset 0x0fe
		bool							mAllocStrmBufferFlag;							// size 0x001, offset 0x0ff
		s32								mLoopCounter;									// size 0x004, offset 0x100
		int								mPrepareCounter;								// size 0x004, offset 0x104
		int								mChangeNumBlocks;								// size 0x004, offset 0x108
		int								mDataBlockSize;									// size 0x004, offset 0x10c
		int								mBufferBlockCount;								// size 0x004, offset 0x110
		int								mBufferBlockCountBase;							// size 0x004, offset 0x114
		int								mLoadingBufferBlockCount;						// size 0x004, offset 0x118
		int								mLoadingBufferBlockIndex;						// size 0x004, offset 0x11c
		int								mLoadingDataBlockIndex;							// size 0x004, offset 0x120
		int								mPlayingBufferBlockCount;						// size 0x004, offset 0x124
		int								mPlayingBufferBlockIndex;						// size 0x004, offset 0x128
		int								mPlayingDataBlockIndex;							// size 0x004, offset 0x12c
		int								mLoopStartBlockIndex;							// size 0x004, offset 0x130
		int								mLastBlockIndex;								// size 0x004, offset 0x134
		int								mLoadWaitCount;									// size 0x004, offset 0x138
		StartOffsetType					mStartOffsetType;								// size 0x004, offset 0x13c
		int								mStartOffset;									// size 0x004, offset 0x140
		StrmHeaderLoadTask				mStrmHeaderLoadTask;							// size 0x020, offset 0x144
		StrmDataLoadTask::LinkList		mStrmDataLoadTaskList;							// size 0x00c, offset 0x164
		InstancePool<StrmDataLoadTask>	mStrmDataLoadTaskPool;							// size 0x004, offset 0x170
		StrmDataLoadTask				mStrmDataLoadTaskArea[BUFFER_BLOCK_COUNT_MAX];	// size 0x680, offset 0x174
		StrmBufferPool					*mBufferPool;									// size 0x004, offset 0x7f4
		ut::FileStream					*mFileStream;									// size 0x004, offset 0x7f8
		s32								mTrackCount;									// size 0x004, offset 0x7fc
		s32								mChannelCount;									// size 0x004, offset 0x800
		s32								mVoiceOutCount;									// size 0x004, offset 0x804
		StrmChannel						mChannels[STRM_CHANNEL_NUM];					// size 0x340, offset 0x808
		StrmTrack						mTracks[STRM_TRACK_NUM];						// size 0x1c0, offset 0xb48
	}; // size 0xd08
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_STRM_PLAYER_H
