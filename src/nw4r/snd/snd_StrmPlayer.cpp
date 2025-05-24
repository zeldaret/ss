#include "nw4r/snd/snd_StrmPlayer.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_StrmPlayer.cpp
 */

/*******************************************************************************
 * headers
 */

#include <climits> // LONG_MAX
#include <cstring> // std::memcpy

#include <decomp.h>
#include "common.h"

#include "nw4r/snd/snd_StrmSound.h"
#include "nw4r/snd/snd_adpcm.h"
#include "nw4r/snd/snd_AxVoice.h"
#include "nw4r/snd/snd_BasicPlayer.h"
#include "nw4r/snd/snd_Channel.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/snd/snd_InstancePool.h"
#include "nw4r/snd/snd_SoundThread.h"
#include "nw4r/snd/snd_StrmChannel.h"
#include "nw4r/snd/snd_StrmFile.h"
#include "nw4r/snd/snd_TaskManager.h"
#include "nw4r/snd/snd_Voice.h"
#include "nw4r/snd/snd_VoiceManager.h"
#include "nw4r/snd/snd_WaveFile.h"

#include "nw4r/ut/ut_DvdFileStream.h"
#include "nw4r/ut/ut_FileStream.h"
#include "nw4r/ut/ut_algorithm.h"
#include "nw4r/ut/ut_Lock.h"
#include "nw4r/ut/ut_RuntimeTypeInfo.h"

#include <rvl/OS/OSCache.h>
#include <rvl/DVD/dvd.h>

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd { namespace detail
{
	// .bss
	byte_t StrmPlayer::sLoadBuffer[LOAD_BUFFER_SIZE];
	OSMutex StrmPlayer::sLoadBufferMutex;

	// .sbss
	bool StrmPlayer::sStaticInitFlag;
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

StrmPlayer::StrmPlayer() :
	mSetupFlag	(false),
	mActiveFlag	(false),
	mFileStream	(nullptr)
{
	if (!sStaticInitFlag)
	{
		OSInitMutex(&sLoadBufferMutex);
		sStaticInitFlag = true;
	}

	u32 taskCount = mStrmDataLoadTaskPool.Create(mStrmDataLoadTaskArea,
	                                             sizeof mStrmDataLoadTaskArea);

	NW4RAssert_Line(70, taskCount == BUFFER_BLOCK_COUNT_MAX);
}

StrmPlayer::~StrmPlayer()
{
	Shutdown();
}

StrmPlayer::SetupResult StrmPlayer::Setup(StrmBufferPool *bufferPool,
                                          int allocChannelCount,
                                          u16 allocTrackFlag,
                                          int voiceOutCount)
{
	SoundThread::AutoLock lock;

	NW4RAssertPointerNonnull_Line(105, bufferPool);

	if (mSetupFlag)
		Shutdown();

	InitParam();
	mChannelCount = ut::Min(allocChannelCount, STRM_CHANNEL_NUM);

	u32 bitMask = allocTrackFlag;

	int trackIndex;
	for (trackIndex = 0; bitMask; bitMask >>= 1, trackIndex++)
	{
		if (!(bitMask & 1))
			continue;

		if (trackIndex >= 8)
		{
			NW4RWarningMessage_Line(
				133, "Too large track index (%d). Max track index is %d.",
				trackIndex, STRM_TRACK_NUM - 1);

			break;
		}

		mTracks[trackIndex].mActiveFlag = true;
	}

	mTrackCount = ut::Min(trackIndex, STRM_TRACK_NUM);
	if (mTrackCount == 0)
		return SETUP_ERR_UNKNOWN;

	mVoiceOutCount	= voiceOutCount;
	mBufferPool		= bufferPool;

	{
		ut::AutoInterruptLock lockIntr;

		if (mChannelCount > 0)
		{
			if (!AllocStrmBuffers())
				return SETUP_ERR_CANNOT_ALLOCATE_BUFFER;
			mAllocStrmBufferFlag = true;
		}
	}

	mSetupFlag = true;

	return SETUP_SUCCESS;
}

void StrmPlayer::Shutdown()
{
	Stop();

	SoundThread::AutoLock lock;

	if (!mSetupFlag)
		return;

	mBufferPool = nullptr;

	NW4RAssert_Line(191,
	                mStrmDataLoadTaskPool.Count() == BUFFER_BLOCK_COUNT_MAX);
	mStrmDataLoadTaskPool.Destroy(mStrmDataLoadTaskArea,
	                              sizeof mStrmDataLoadTaskArea);

	mSetupFlag = false;
}

bool StrmPlayer::Prepare(ut::FileStream *fileStream,
                         StartOffsetType startOffsetType, int startOffset)
{
	SoundThread::AutoLock lock;

	NW4RAssert_Line(218, mSetupFlag);
	NW4RAssertPointerNonnull_Line(219, fileStream);
	NW4RAssert_Line(220, fileStream->CanRead());
	NW4RAssert_Line(221, fileStream->CanSeek());

	mFileStream			= fileStream;
	mStartOffsetType	= startOffsetType;
	mStartOffset		= startOffset;
	mTaskErrorFlag		= false;
	mTaskCancelFlag		= false;
	mLoadingDelayFlag	= false;
	mActiveFlag			= true;

	SoundThread::GetInstance().RegisterPlayerCallback(this);

	StrmHeaderLoadTask *task = &mStrmHeaderLoadTask;

	task->player			= this;
	task->fileStream		= mFileStream;
	task->startOffsetType	= mStartOffsetType;
	task->startOffset		= mStartOffset;

	TaskManager::GetInstance().AppendTask(task, TaskManager::PRIORITY_MIDDLE);

	return true;
}

bool StrmPlayer::Start()
{
	SoundThread::AutoLock lock;

	if (!mPreparedFlag)
		return false;

	if (!mStartedFlag)
	{
		if (!AllocVoices(mVoiceOutCount))
		{
			FreeStrmBuffers();
			return false;
		}

		s32 blockIndex = 0;
		u32 blockOffset = 0;
		s32 loopCount = 0;

		if (!CalcStartOffset(&blockIndex, &blockOffset, &loopCount))
		{
			NW4RPanic_Line(276);
			return false;
		}

		mLoopCounter += loopCount;

		u32 sampleBufferLen = mDataBlockSize * mPlayingBufferBlockCount;
		u32 sampleCount =
			GetSampleByByte(sampleBufferLen, mStrmInfo.sampleFormat);

		for (int trackIndex = 0; trackIndex < mTrackCount; trackIndex++)
		{
			StrmTrack &track = mTracks[trackIndex];
			if (!track.mActiveFlag)
				continue;

			WaveInfo waveData;

			waveData.sampleFormat	= mStrmInfo.sampleFormat;
			waveData.loopFlag		= true;
			waveData.numChannels	= track.mTrackInfo.channelCount;
			waveData.sampleRate		= mStrmInfo.sampleRate;
			waveData.loopStart		= 0;
			waveData.loopEnd		= sampleCount;

			for (int channelIndex = 0;
			     channelIndex < track.mTrackInfo.channelCount; channelIndex++)
			{
				StrmChannel *channel = GetTrackChannel(track, channelIndex);
				if (!channel)
					continue;

				ChannelParam &channelParam =
					waveData.channelParam[channelIndex];

				channelParam.dataAddr		= channel->bufferAddress;
				channelParam.adpcmParam		= channel->adpcmParam;
				channelParam.adpcmLoopParam	= channel->adpcmLoopParam;
				channelParam.adpcmParam.pred_scale =
					*static_cast<u8 *>(channel->bufferAddress);
			}

			{
				ut::AutoInterruptLock lock;

				if (track.mVoice)
				{
					track.mVoice->Setup(waveData, blockOffset);
					track.mVoice->SetVoiceType(AxVoice::VOICE_TYPE_STREAM);
					track.mVoice->Start();
				}
			}
		}

		if (blockIndex == mStrmInfo.numBlocks - 2)
			UpdateDataLoopAddress(1);
		else if (blockIndex == mStrmInfo.numBlocks - 1)
			UpdateDataLoopAddress(0);

		UpdatePauseStatus();

		mStartedFlag = true;
	}

	return true;
}

u32 StrmPlayer::GetSampleByByte(u32 byte, SampleFormat format)
{
	u32 samples = 0;

	switch (format)
	{
	case SAMPLE_FORMAT_DSP_ADPCM:
	{
		samples = (byte >> 3) * 14;

		if (u32 frac = byte & 0x07)
			samples += (frac - 1) * 2;
	}
		break;

	case SAMPLE_FORMAT_PCM_S8:
		samples = byte;
		break;

	case SAMPLE_FORMAT_PCM_S16:
		samples = byte / 2;
		break;

	default:
		NW4RPanicMessage_Line(368, "Invalid format\n");
		break;
	}

	return samples;
}

void StrmPlayer::Stop()
{
	{
		SoundThread::AutoLock lock;

		for (int trackIndex = 0; trackIndex < STRM_TRACK_NUM; trackIndex++)
		{
			if (mTracks[trackIndex].mActiveFlag)
			{
				if (Voice *voice = mTracks[trackIndex].mVoice)
					voice->Stop();
			}
		}

		if (mActiveFlag)
			SoundThread::GetInstance().UnregisterPlayerCallback(this);
	}

	TaskManager::GetInstance().CancelTask(&mStrmHeaderLoadTask);

	{
		ut::AutoInterruptLock lock;

		while (!mStrmDataLoadTaskList.IsEmpty())
		{
			StrmDataLoadTask *task = &mStrmDataLoadTaskList.GetBack();

			TaskManager::GetInstance().CancelTask(task);
		}
	}

	FreeStrmBuffers();
	FreeVoices();

	{
		SoundThread::AutoLock lock;

		if (mFileStream)
		{
			mFileStream->Close();
			mFileStream = nullptr;
		}
	}

	mStartedFlag	= false;
	mPreparedFlag	= false;
	mActiveFlag		= false;

	NW4RAssert_Line(435,
	                mStrmDataLoadTaskPool.Count() == BUFFER_BLOCK_COUNT_MAX);
}

void StrmPlayer::Pause(bool flag)
{
	SoundThread::AutoLock lock;

	mPauseFlag = flag;

	if (flag)
		mLoadWaitFlag = true;

	UpdatePauseStatus();
}

// Some functions in between idk
DECOMP_FORCE(0.0f);
DECOMP_FORCE(SI2D_CONSTANT);
DECOMP_FORCE(UI2D_CONSTANT);

void StrmPlayer::InitParam()
{
	BasicPlayer::InitParam();

	mStartedFlag			= false;
	mPreparedFlag			= false;
	mLoadFinishFlag			= false;
	mPauseFlag				= false;
	mPauseStatus			= false;
	mLoadWaitFlag			= false;
	mNoRealtimeLoadFlag		= false;
	mPlayFinishFlag			= false;
	mSkipUpdateAdpcmLoop	= false;
	mValidAdpcmLoop			= false;
	mAllocStrmBufferFlag	= false;
	mLoopCounter			= 0;
	mVoiceOutCount			= 1;
	mLoadWaitCount			= 0;

	for (int trackIndex = 0; trackIndex < STRM_TRACK_NUM; trackIndex++)
	{
		StrmTrack &track = mTracks[trackIndex];

		track.mActiveFlag	= false;
		track.mVolume		= 1.0f;
		track.mPan			= 0.0f;
		track.mVoice		= nullptr;
	}

	for (int channelIndex = 0; channelIndex < STRM_CHANNEL_NUM; channelIndex++)
	{
		StrmChannel &channel = mChannels[channelIndex];

		channel.bufferAddress	= nullptr;
	}
}

#pragma push

#pragma ppc_iro_level 0 // somehow this got turned off???

bool StrmPlayer::LoadHeader(ut::FileStream *fileStream,
                            StartOffsetType startOffsetType, int startOffset)
{
	NW4RAssertPointerNonnull_Line(619, fileStream);

	ut::detail::AutoLock<OSMutex> lock(sLoadBufferMutex);

	StrmFileLoader loader(*fileStream);
	if (!loader.LoadFileHeader(sLoadBuffer, LOAD_BUFFER_SIZE))
		return false;

	if (!loader.ReadStrmInfo(&mStrmInfo))
		return false;

	if (mChannelCount == 0)
		mChannelCount = ut::Min(loader.GetChannelCount(), STRM_CHANNEL_NUM);

	NW4RAssert_Line(643, mTrackCount == ut::Min( loader.GetTrackCount(),
	                                             STRM_TRACK_NUM ));
	NW4RAssert_Line(644, mChannelCount == ut::Min( loader.GetChannelCount(),
	                                               STRM_CHANNEL_NUM ));

	for (int i = 0; i < mTrackCount; i++)
	{
		if (!loader.ReadStrmTrackInfo(&mTracks[i].mTrackInfo, i))
			return false;
	}

	if (IsAdpcm())
	{
		for (int i = 0; i < mChannelCount; i++)
		{
			if (!loader.ReadAdpcmInfo(&mChannels[i].adpcmParam,
			                          &mChannels[i].adpcmLoopParam, i))
			{
				return false;
			}
		}

		if (startOffset != 0)
		{
			int startOffsetSamples;

			if (startOffsetType == START_OFFSET_TYPE_SAMPLE)
				startOffsetSamples = startOffset;
			else if (startOffsetType == START_OFFSET_TYPE_MILLISEC)
				startOffsetSamples = startOffset * mStrmInfo.sampleRate / 1000;

			/* NOTE: startOffsetSamples is used uninitialized if neither
			 * branch is taken (asserted externally or ERRATUM?)
			 */

			s32 blockIndex =
				startOffsetSamples / static_cast<s32>(mStrmInfo.blockSamples);

			u16 yn1[16];
			u16 yn2[16];
			if (!loader.ReadAdpcBlockData(yn1, yn2, blockIndex,
			                              mStrmInfo.numChannels))
			{
				return false;
			}

			for (int i = 0; i < mStrmInfo.numChannels; i++)
			{
				mChannels[i].adpcmParam.yn1 = yn1[i];
				mChannels[i].adpcmParam.yn2 = yn2[i];
			}
		}
	}

	if (!SetupPlayer())
		return false;

	mPrepareCounter = 0;

	for (int i = 0; i < mBufferBlockCountBase; i++)
	{
		UpdateLoadingBlockIndex();

		mPrepareCounter++;

		if (mLoadFinishFlag)
			break;
	}

	if (mStrmInfo.numBlocks <= 2 && !mStrmInfo.loopFlag)
		SetLoopEndToZeroBuffer(mStrmInfo.numBlocks - 1);

	return true;
}

#pragma pop

#pragma push

#pragma ppc_iro_level 0 // somehow this got turned off???

bool StrmPlayer::LoadStreamData(ut::FileStream *fileStream, int offset,
                                u32 size ATTR_UNUSED, u32 blockSize,
                                int bufferBlockIndex, bool needUpdateAdpcmLoop)
{
	NW4RAssertPointerNonnull_Line(746, fileStream);
	NW4RAssertAligned_Line(747, offset, 32);
	NW4RAssertAligned_Line(748, blockSize, 32);

	if (ut::DvdFileStream *dvdStream =
	        ut::DynamicCast<ut::DvdFileStream *>(fileStream))
	{
		dvdStream->SetPriority(1);
	}

	ut::detail::AutoLock<OSMutex> lock(sLoadBufferMutex);

	DCInvalidateRange(sLoadBuffer, LOAD_BUFFER_SIZE);

	int loadOffset = offset + mStrmInfo.blockHeaderOffset;
	u16 adpcmPredScale[STRM_CHANNEL_NUM];

	int currentChannel = 0;
	while (currentChannel < mChannelCount)
	{
		NW4RAssertAligned_Line(773, loadOffset, 32);

		int loadChannelCount = Channel::CHANNEL_MAX;

		if (currentChannel + loadChannelCount > mChannelCount)
			loadChannelCount = mChannelCount - currentChannel;

		u32 loadSize = blockSize * loadChannelCount;
		NW4RAssert_Line(781, loadSize <= LOAD_BUFFER_SIZE);

		fileStream->Seek(loadOffset, ut::FileStream::SEEK_ORIGIN_SET);

		s32 resultSize = fileStream->Read(sLoadBuffer, loadSize);
		if (resultSize != loadSize)
			return false;

		for (int i = 0; i < loadChannelCount; i++)
		{
			if (needUpdateAdpcmLoop)
				adpcmPredScale[currentChannel] = sLoadBuffer[blockSize * i];

			u32 len = blockSize;
			void *source = ut::AddOffsetToPtr(sLoadBuffer, blockSize * i);
			void *dest =
				ut::AddOffsetToPtr(mChannels[currentChannel].bufferAddress,
			                       mDataBlockSize * bufferBlockIndex);

			std::memcpy(dest, source, len);
			DCFlushRange(dest, len);

			currentChannel++;
		}

		loadOffset += loadSize;
	}

	if (needUpdateAdpcmLoop)
		SetAdpcmLoopContext(mChannelCount, adpcmPredScale);

	if (!mPreparedFlag)
	{
		mPrepareCounter--;

		if (mPrepareCounter == 0)
			mPreparedFlag = true;
	}

	return true;
}

#pragma pop // ?????

bool StrmPlayer::SetupPlayer()
{
	NW4RAssertPointerNonnull_Line(850, mBufferPool);

	u32 strmBufferSize = mBufferPool->GetBlockSize();

	s32 blockIndex = 0;
	u32 blockOffset = 0;
	s32 loopCount = 0;
	if (!CalcStartOffset(&blockIndex, &blockOffset, &loopCount))
		return false;

	mLoopStartBlockIndex = mStrmInfo.loopStart / mStrmInfo.blockSamples;
	mLastBlockIndex = mStrmInfo.numBlocks - 1;

	mDataBlockSize = mStrmInfo.blockSize;
	if (mDataBlockSize > DATA_BLOCK_SIZE_MAX)
	{
		NW4RWarningMessage_Line(870, "Too large stream data block size.");
		return false;
	}

	mBufferBlockCount = strmBufferSize / mDataBlockSize;
	if (mBufferBlockCount < 4)
	{
		NW4RWarningMessage_Line(876, "Too small stream buffer size.");
		return false;
	}

	if (mBufferBlockCount > BUFFER_BLOCK_COUNT_MAX)
		mBufferBlockCount = BUFFER_BLOCK_COUNT_MAX;

	mBufferBlockCountBase		= mBufferBlockCount - 1;
	mChangeNumBlocks			= mBufferBlockCountBase;

	mPlayingDataBlockIndex		= blockIndex;
	mLoadingDataBlockIndex		= blockIndex;

	mLoadingBufferBlockIndex	= 0;
	mPlayingBufferBlockIndex	= 0;

	if (mNoRealtimeLoadFlag)
		mLoadingBufferBlockCount = mStrmInfo.numBlocks;
	else
		mLoadingBufferBlockCount = CalcLoadingBufferBlockCount();

	mPlayingBufferBlockCount = mLoadingBufferBlockCount;

	ut::AutoInterruptLock lock;

	if (!mAllocStrmBufferFlag)
	{
		if (!AllocStrmBuffers())
			return false;

		mAllocStrmBufferFlag = true;
	}

	return true;
}

bool StrmPlayer::AllocStrmBuffers()
{
	for (int index = 0; index < mChannelCount; index++)
	{
		void *strmBuffer = mBufferPool->Alloc();

		if (!strmBuffer)
		{
			for (int i = 0; i < index; i++)
			{
				mBufferPool->Free(mChannels[i].bufferAddress);
				mChannels[i].bufferAddress = nullptr;
			}

			return false;
		}

		mChannels[index].bufferAddress = strmBuffer;
	}

	return true;
}

void StrmPlayer::FreeStrmBuffers()
{
	for (int index = 0; index < mChannelCount; index++)
	{
		if (!mChannels[index].bufferAddress)
			continue;

		mBufferPool->Free(mChannels[index].bufferAddress);
		mChannels[index].bufferAddress = nullptr;
	}
}

bool StrmPlayer::AllocVoices(int voiceOutCount)
{
	ut::AutoInterruptLock lock;

	NW4RAssertPointerNonnull_Line(992, mBufferPool);

	for (int trackIndex = 0; trackIndex < mTrackCount; trackIndex++)
	{
		StrmTrack &track = mTracks[trackIndex];
		if (!track.mActiveFlag)
			continue;

		Voice *voice = VoiceManager::GetInstance().AllocVoice(
			track.mTrackInfo.channelCount, voiceOutCount, Voice::PRIORITY_MAX,
			&VoiceCallbackFunc, &mTracks[trackIndex]);

		if (!voice)
		{
			for (int i = 0; i < trackIndex; i++)
			{
				StrmTrack &t = mTracks[i];

				if (t.mVoice)
				{
					t.mVoice->Free();
					t.mVoice = nullptr;
				}
			}

			return false;
		}

		track.mVoice = voice;
		voice->SetVoiceOutParamPitchDisableFlag(true);
	}

	return true;
}

void StrmPlayer::FreeVoices()
{
	ut::AutoInterruptLock lock;

	for (int trackIndex = 0; trackIndex < mTrackCount; trackIndex++)
	{
		StrmTrack &track = mTracks[trackIndex];
		if (!track.mActiveFlag)
			continue;

		if (track.mVoice)
		{
			track.mVoice->Free();
			track.mVoice = nullptr;
		}
	}
}

void StrmPlayer::Update()
{
	if (!mActiveFlag)
		return;

	if (mTaskErrorFlag && !mTaskCancelFlag)
	{
		NW4RWarningMessage_Line(1076, "Task error is occured.");

		Stop();
		return;
	}

	if (mStartedFlag)
	{
		for (int trackIndex = 0; trackIndex < mTrackCount; trackIndex++)
		{
			StrmTrack &track = mTracks[trackIndex];

			if (track.mActiveFlag && !track.mVoice)
			{
				Stop();
				return;
			}
		}
	}

	if (mLoadWaitFlag && mStrmDataLoadTaskList.IsEmpty()
	    && !CheckDiskDriveError())
	{
		mLoadWaitFlag = false;
		UpdatePauseStatus();
	}

	if (mLoadingDelayFlag)
	{
		NW4RWarningMessage_Line(1109, "Pause stream because of loading delay.");
		mLoadingDelayFlag = false;
	}

	for (int trackIndex = 0; trackIndex < mTrackCount; trackIndex++)
		UpdateVoiceParams(&mTracks[trackIndex]);
}

void StrmPlayer::UpdateVoiceParams(StrmTrack *track)
{
	if (!track->mActiveFlag)
		return;

	f32 volume = 1.0f;
	volume *= GetVolume();
	volume *= track->mTrackInfo.volume / 127.0f;
	volume *= track->mVolume;

	f32 pitchRatio = 1.0f;
	pitchRatio *= GetPitch();

	f32 pan = 0.0f;
	pan += GetPan();
	if (track->mTrackInfo.pan <= 1)
		pan += (track->mTrackInfo.pan - 63) / 63.0f;
	else
		pan += (track->mTrackInfo.pan - 64) / 63.0f;

	pan += track->mPan;

	f32 surroundPan = 0.0f;
	surroundPan += GetSurroundPan();

	f32 lpfFreq = 1.0f;
	lpfFreq += GetLpfFreq();

	int biquadType = GetBiquadType();
	f32 biquadValue = GetBiquadValue();

	int remoteFilter = 0;
	remoteFilter += GetRemoteFilter();

	f32 mainSend = 0.0f;
	mainSend += GetMainSend();

	f32 fxsend[AUX_BUS_NUM];
	for (int i = 0; i < AUX_BUS_NUM; i++)
	{
		fxsend[i] = 0.0f;
		fxsend[i] += GetFxSend(static_cast<AuxBus>(i));
	}

	ut::AutoInterruptLock lock;

	if (Voice *voice = track->mVoice)
	{
		voice->SetVolume(volume);
		voice->SetPitch(pitchRatio);
		voice->SetPan(pan);
		voice->SetSurroundPan(surroundPan);
		voice->SetLpfFreq(lpfFreq);
		voice->SetBiquadFilter(biquadType, biquadValue);
		voice->SetRemoteFilter(remoteFilter);
		voice->SetOutputLine(GetOutputLine());
		voice->SetMainOutVolume(GetMainOutVolume());
		voice->SetMainSend(mainSend);

		for (int i = 0; i < AUX_BUS_NUM; i++)
		{
			AuxBus bus = static_cast<AuxBus>(i);
			voice->SetFxSend(bus, fxsend[i]);
		}

		for (int i = 0; i < mVoiceOutCount; i++)
			voice->SetVoiceOutParam(i, GetVoiceOutParam(i));
	}
}

bool StrmPlayer::CheckDiskDriveError() const
{
	ut::DvdFileStream *dvdFileStream =
		ut::DynamicCast<ut::DvdFileStream *>(mFileStream);
	if (!dvdFileStream)
		return false;

	DVDState driveStatus = DVDGetDriveStatus();
	switch (driveStatus)
	{
	case DVD_STATE_IDLE:
	case DVD_STATE_BUSY:
		return false;

	default:
		return true;
	}
}

void StrmPlayer::UpdateBuffer()
{
	if (!mStartedFlag)
		return;

	if (!mTracks[0].mActiveFlag)
		return;

	Voice *voice = mTracks[0].mVoice;
	if (!voice)
		return;

	if (CheckDiskDriveError())
	{
		mLoadWaitFlag = true;

		UpdatePauseStatus();
	}

	if (!mPlayFinishFlag && !mNoRealtimeLoadFlag && !mLoadWaitFlag)
	{
		u32 playingSample = voice->GetCurrentPlayingSample();
		int axCurrentBlockIndex = playingSample / mStrmInfo.blockSamples;

		while (mPlayingBufferBlockIndex != axCurrentBlockIndex)
		{
			if (!mLoadWaitFlag && !mStrmDataLoadTaskList.IsEmpty()
			    && mLoadWaitCount >= mBufferBlockCountBase - 2)
			{
				mLoadingDelayFlag = true;
				mLoadWaitFlag = true;

				UpdatePauseStatus();

				break;
			}
			else
			{
				UpdatePlayingBlockIndex();
				UpdateLoadingBlockIndex();
			}
		}
	}
}

void StrmPlayer::UpdateLoopAddress(u32 loopStartSamples, u32 loopEndSamples)
{
	ut::AutoInterruptLock lock;

	for (int trackIndex = 0; trackIndex < mTrackCount; trackIndex++)
	{
		StrmTrack &track = mTracks[trackIndex];

		if (!track.mActiveFlag)
			continue;

		Voice *voice = track.mVoice;
		if (!voice)
			continue;

		for (int channelIndex = 0;
		     channelIndex < track.mTrackInfo.channelCount;
		     channelIndex++)
		{
			// mTracks[trackIndex] againinstead of track?
			StrmChannel *channel =
				GetTrackChannel(mTracks[trackIndex], channelIndex);

			voice->SetLoopStart(channelIndex, channel->bufferAddress,
			                    loopStartSamples);
			voice->SetLoopEnd(channelIndex, channel->bufferAddress,
			                  loopEndSamples);
		}

		voice->SetLoopFlag(true);
	}
}

void StrmPlayer::UpdatePlayingBlockIndex()
{
	mPlayingDataBlockIndex++;
	if (mPlayingDataBlockIndex > mLastBlockIndex)
	{
		if (mStrmInfo.loopFlag)
		{
			mPlayingDataBlockIndex = mLoopStartBlockIndex;

			if (mLoopCounter < LONG_MAX)
				mLoopCounter++;

			UpdateLoopAddress(0, mPlayingBufferBlockCount
			                         * mStrmInfo.blockSamples);
		}
		else
		{
			NW4RPanic_Line(1379);
		}
	}

	mPlayingBufferBlockIndex++;
	if (mPlayingBufferBlockIndex >= mPlayingBufferBlockCount)
	{
		mPlayingBufferBlockIndex = 0;
		mPlayingBufferBlockCount = mLoadingBufferBlockCount;

		UpdateLoopAddress(0, mPlayingBufferBlockCount * mStrmInfo.blockSamples);
	}

	if (mPlayingBufferBlockIndex == mPlayingBufferBlockCount - 1)
	{
		if (!mSkipUpdateAdpcmLoop && mValidAdpcmLoop)
		{
			for (int trackIndex = 0; trackIndex < mTrackCount; trackIndex++)
			{
				StrmTrack &track = mTracks[trackIndex];

				if (!track.mActiveFlag)
					continue;

				Voice *voice = track.mVoice;
				if (!voice)
					continue;

				if (voice->GetFormat() == SAMPLE_FORMAT_DSP_ADPCM)
				{
					NW4RCheckMessage_Line(1411, mValidAdpcmLoop,
					                      "AdpcmLoop can not update!");

					ut::AutoInterruptLock lock;

					for (int channelIndex = 0;
					     channelIndex < track.mTrackInfo.channelCount;
					     channelIndex++)
					{
						StrmChannel *channel =
							GetTrackChannel(track, channelIndex);

						AdpcmLoopParam loop;
						loop.loop_pred_scale = channel->adpcmPredScale;
						loop.loop_yn1 = 0;
						loop.loop_yn2 = 0;

						voice->SetAdpcmLoop(channelIndex, &loop);
					}

					voice->SetVoiceType(AxVoice::VOICE_TYPE_STREAM);
				}
			}
		}
		mValidAdpcmLoop = false;
		mSkipUpdateAdpcmLoop = false;
	}

	if (mPlayingDataBlockIndex == mLastBlockIndex - 1)
	{
		s32 endBufferBlockIndex = mPlayingBufferBlockIndex + 1;
		UpdateDataLoopAddress(endBufferBlockIndex);
	}
}

void StrmPlayer::UpdateDataLoopAddress(s32 endBlockBufferIndex)
{
	if (mStrmInfo.loopFlag)
	{
		s32 startBlockNum = endBlockBufferIndex + 1;

		if (startBlockNum >= mPlayingBufferBlockCount)
			startBlockNum -= mPlayingBufferBlockCount;

		ut::AutoInterruptLock lock;

		UpdateLoopAddress(startBlockNum * mStrmInfo.blockSamples,
		                  mStrmInfo.lastBlockSamples
		                      + (endBlockBufferIndex * mStrmInfo.blockSamples));

		if (IsAdpcm())
		{
			for (int trackIndex = 0; trackIndex < mTrackCount; trackIndex++)
			{
				StrmTrack &track = mTracks[trackIndex];

				if (!track.mActiveFlag)
					continue;

				Voice *voice = track.mVoice;
				if (!voice)
					continue;

				if (voice->GetFormat() == SAMPLE_FORMAT_DSP_ADPCM)
				{
					voice->SetVoiceType(AxVoice::VOICE_TYPE_NORMAL);

					for (int channelIndex = 0;
					     channelIndex < track.mTrackInfo.channelCount;
					     channelIndex++)
					{
						StrmChannel *channel =
							GetTrackChannel(track, channelIndex);

						voice->SetAdpcmLoop(channelIndex,
						                    &channel->adpcmLoopParam);
					}
				}
			}

			if (endBlockBufferIndex == mPlayingBufferBlockCount - 1)
				mSkipUpdateAdpcmLoop = true;
		}
	}
	else
	{
		SetLoopEndToZeroBuffer(endBlockBufferIndex);
	}
}

void StrmPlayer::SetLoopEndToZeroBuffer(int endBufferBlockIndex)
{
	{
		ut::AutoInterruptLock lock;

		for (int trackIndex = 0; trackIndex < mTrackCount; trackIndex++)
		{
			StrmTrack &track = mTracks[trackIndex];

			if (!track.mActiveFlag)
				continue;

			Voice *voice = track.mVoice;
			if (!voice)
				continue;

			for (int channelIndex = 0;
			     channelIndex < track.mTrackInfo.channelCount; channelIndex++)
			{
				StrmChannel *channel = GetTrackChannel(track, channelIndex);

				voice->StopAtPoint(channelIndex, channel->bufferAddress,
				                   mStrmInfo.lastBlockSamples
				                       + endBufferBlockIndex
				                             * mStrmInfo.blockSamples);
			}
		}
	}

	mPlayFinishFlag = true;
}

void StrmPlayer::UpdateLoadingBlockIndex()
{
	mLoadWaitCount++;

	if (mLoadFinishFlag)
		return;

	u32 blockSize =
		mLoadingDataBlockIndex < static_cast<s32>(mStrmInfo.numBlocks - 1)
			? mStrmInfo.blockSize
			: mStrmInfo.lastBlockPaddedSize;

	u32 loadSize = mStrmInfo.blockHeaderOffset + blockSize * mChannelCount;

	s32 loadOffset = mStrmInfo.dataOffset
	                + mLoadingDataBlockIndex
	                      * (mStrmInfo.blockHeaderOffset
	                         + mStrmInfo.blockSize * mStrmInfo.numChannels);

	NW4RAssertAligned_Line(1576, blockSize, 32);
	NW4RAssertAligned_Line(1577, loadSize, 32);
	NW4RAssertAligned_Line(1578, loadOffset, 32);

	bool needUpdateAdpcmLoop = mLoadingBufferBlockIndex == 0 && IsAdpcm();

	StrmDataLoadTask *task = mStrmDataLoadTaskPool.Alloc();
	NW4RAssertPointerNonnull_Line(1584, task);

	task->mStrmPlayer			= this;
	task->fileStream			= mFileStream;
	task->mSize					= loadSize;
	task->mOffset				= loadOffset;
	task->mBlockSize			= blockSize;
	task->mBufferBlockIndex		= mLoadingBufferBlockIndex;
	task->mNeedUpdateAdpcmLoop	= needUpdateAdpcmLoop;

	ut::AutoInterruptLock lock;

	mStrmDataLoadTaskList.PushBack(task);

	TaskManager::GetInstance().AppendTask(
		task, mStartedFlag ? TaskManager::PRIORITY_HIGH
						   : TaskManager::PRIORITY_MIDDLE);

	mLoadingDataBlockIndex++;

	if (mLoadingDataBlockIndex > mLastBlockIndex)
	{
		if (mStrmInfo.loopFlag)
		{
			mLoadingDataBlockIndex = mLoopStartBlockIndex;
		}
		else
		{
			mLoadFinishFlag = true;

			return;
		}
	}

	mLoadingBufferBlockIndex++;

	if (mLoadingBufferBlockIndex >= mLoadingBufferBlockCount)
	{
		mLoadingBufferBlockIndex = 0;
		mLoadingBufferBlockCount = CalcLoadingBufferBlockCount();
	}
}

void StrmPlayer::UpdatePauseStatus()
{
	ut::AutoInterruptLock lock;

	bool pauseStatus = false;

	if (mPauseFlag)
		pauseStatus = true;

	if (mLoadWaitFlag)
		pauseStatus = true;

	if (pauseStatus != mPauseStatus)
	{
		for (int trackIndex = 0; trackIndex < mTrackCount; trackIndex++)
		{
			if (!mTracks[trackIndex].mActiveFlag)
				continue;

			if (Voice *voice = mTracks[trackIndex].mVoice)
				voice->Pause(pauseStatus);
		}

		mPauseStatus = pauseStatus;
	}
}

int StrmPlayer::CalcLoadingBufferBlockCount() const
{
	int restBlockCount = mLastBlockIndex - mLoadingDataBlockIndex + 1;
	int loopBlockCount = mLastBlockIndex - mLoopStartBlockIndex + 1;

	if ((mBufferBlockCountBase + 1 - restBlockCount) % loopBlockCount == 0)
		return mBufferBlockCountBase + 1;
	else
		return mBufferBlockCountBase;
}

bool StrmPlayer::CalcStartOffset(s32 *startBlockIndex, u32 *startBlockOffset,
                                 s32 *loopCount)
{
	if (mStrmInfo.blockSamples == 0)
		return false;

	int startOffsetSamples;
	if (mStartOffsetType == START_OFFSET_TYPE_SAMPLE)
	{
		startOffsetSamples = mStartOffset;
	}
	else if (mStartOffsetType == START_OFFSET_TYPE_MILLISEC)
	{
		startOffsetSamples =
			mStartOffset * static_cast<s64>(mStrmInfo.sampleRate) / 1000;
	}

	*loopCount = 0;

	if (startOffsetSamples >= mStrmInfo.loopEnd)
	{
		if (mStrmInfo.loopFlag)
		{
			s32 loopStart = mStrmInfo.loopStart;
			s32 loopEnd = mStrmInfo.loopEnd;
			s32 loopLen = loopEnd - loopStart;
			s32 startOffset2 = startOffsetSamples - loopEnd;

			*loopCount = startOffset2 / loopLen + 1;

			startOffsetSamples = loopStart + startOffset2 % loopLen;
		}
		else
		{
			return false;
		}
	}

	*startBlockIndex =
		startOffsetSamples / static_cast<int>(mStrmInfo.blockSamples);

	*startBlockOffset = startOffsetSamples % mStrmInfo.blockSamples;

	return true;
}

void StrmPlayer::VoiceCallbackFunc(Voice *voice,
                                   Voice::VoiceCallbackStatus status, void *arg)
{
	StrmTrack *track = static_cast<StrmTrack *>(arg);
	NW4RAssertPointerNonnull_Line(1771, track);
	NW4RAssert_Line(1773, track->mVoice == voice);

	ut::AutoInterruptLock lock;

	switch (status)
	{
	case Voice::CALLBACK_STATUS_FINISH_WAVE:
	case Voice::CALLBACK_STATUS_CANCEL:
		voice->Free();
		track->mVoice = nullptr;

		break;

	case Voice::CALLBACK_STATUS_DROP_VOICE:
	case Voice::CALLBACK_STATUS_DROP_DSP:
		track->mVoice = nullptr;

		break;

	default:
		NW4RPanicMessage_Line(1789, "Unknown Voice callback status %d", status);
		return; // NOTE: do not change (invokes scope guard destructor twice)
	}
}

void StrmPlayer::SetAdpcmLoopContext(int channelNum, u16 *predScale)
{
	if (!IsAdpcm())
		return;

	for (int channelIndex = 0;
	     channelIndex < channelNum && channelIndex < STRM_CHANNEL_NUM;
	     channelIndex++)
	{
		mChannels[channelIndex].adpcmPredScale = predScale[channelIndex];
	}

	mValidAdpcmLoop = true;
}

StrmChannel *StrmPlayer::GetTrackChannel(StrmTrack const &track,
                                         int channelIndex)
{
	if (channelIndex >= Channel::CHANNEL_MAX)
		return nullptr;

	int index = track.mTrackInfo.channelIndexTable[channelIndex];
	if (index >= STRM_CHANNEL_NUM)
		return nullptr;

	return &mChannels[index];
}

void StrmPlayer::SetTrackVolume(u32 trackBitFlag, f32 volume)
{
	ut::AutoInterruptLock lock;

	for (int trackNo = 0; trackNo < mTrackCount && trackBitFlag;
	     trackNo++, trackBitFlag >>= 1)
	{
		if (trackBitFlag & 1)
			mTracks[trackNo].mVolume = volume;
	}
}

StrmPlayer::StrmTrack *StrmPlayer::GetPlayerTrack(int trackNo)
{
	if (trackNo > STRM_TRACK_NUM - 1)
		return nullptr;

	return &mTracks[trackNo];
}

StrmPlayer::StrmHeaderLoadTask::StrmHeaderLoadTask() :
	player		(nullptr),
	fileStream	(nullptr),
	startOffset	(0)
{
}

void StrmPlayer::StrmHeaderLoadTask::Execute()
{
	NW4RAssertPointerNonnull_Line(1894, player);

	bool result = player->LoadHeader(fileStream, startOffsetType, startOffset);
	if (!result)
		player->SetTaskErrorFlag();
}

void StrmPlayer::StrmHeaderLoadTask::Cancel()
{
	/* ... */
}

void StrmPlayer::StrmHeaderLoadTask::OnCancel()
{
	player->SetTaskCancelFlag();

	if (fileStream && fileStream->CanCancel())
	{
		if (fileStream->CanAsync())
			fileStream->CancelAsync(nullptr, nullptr);
		else
			fileStream->Cancel();
	}
}

StrmPlayer::StrmDataLoadTask::StrmDataLoadTask() :
	mStrmPlayer				(nullptr),
	fileStream				(nullptr),
	mSize					(0),
	mOffset					(0),
	mBlockSize				(0),
	mBufferBlockIndex		(-1),
	mNeedUpdateAdpcmLoop	(false)
{
}

void StrmPlayer::StrmDataLoadTask::Execute()
{
	bool result =
		mStrmPlayer->LoadStreamData(fileStream, mOffset, mSize, mBlockSize,
	                                mBufferBlockIndex, mNeedUpdateAdpcmLoop);
	if (!result)
		mStrmPlayer->SetTaskErrorFlag();

	ut::AutoInterruptLock lock;
	mStrmPlayer->mStrmDataLoadTaskList.Erase(this);
	mStrmPlayer->mStrmDataLoadTaskPool.Free(this);

	mStrmPlayer->mLoadWaitCount--;
}

void StrmPlayer::StrmDataLoadTask::Cancel()
{
	ut::AutoInterruptLock lock;

	mStrmPlayer->mStrmDataLoadTaskList.Erase(this);
	mStrmPlayer->mStrmDataLoadTaskPool.Free(this);
}

void StrmPlayer::StrmDataLoadTask::OnCancel()
{
	mStrmPlayer->SetTaskCancelFlag();

	if (fileStream && fileStream->CanCancel())
	{
		if (fileStream->CanAsync())
			fileStream->CancelAsync(nullptr, nullptr);
		else
			fileStream->Cancel();
	}
}

}}} // namespace nw4r::snd::detail
