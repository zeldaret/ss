#include "nw4r/snd/SeqSound.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SeqSound.cpp
 */

/*******************************************************************************
 * headers
 */

#include <decomp.h>
#include <types.h>

#include "nw4r/snd/BasicSound.h"
#include "nw4r/snd/PlayerHeap.h"
#include "nw4r/snd/SeqFile.h"
#include "nw4r/snd/SeqPlayer.h"
#include "nw4r/snd/SeqSoundHandle.h"
#include "nw4r/snd/SoundInstanceManager.h"
#include "nw4r/snd/TaskManager.h"

#include "nw4r/ut/FileStream.h"
#include "nw4r/ut/RuntimeTypeInfo.h"

#if 0
#include <revolution/DVD/dvd.h> // DVD_ECANCELED
#else
#include <context_rvl.h>
#endif

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class NoteOnCallback; }}}
namespace nw4r { namespace snd { namespace detail { class SeqTrackAllocator; }}}

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd { namespace detail
{
	ut::detail::RuntimeTypeInfo const SeqSound::typeInfo(&BasicSound::typeInfo);
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

#pragma push

// TODO: fake

SeqSound::SeqSound(SoundInstanceManager<SeqSound> *manager, int priority,
                   int ambientPriority) :
#if 1
	BasicSound			(priority, ambientPriority),
	mTempSpecialHandle	(
		reinterpret_cast<SeqSoundHandle *>(mPreparedFlag = mLoadingFlag = 0)),
	mManager			(manager),
	mStartOffset		(0),
	mFileStream			(nullptr)
#else
	BasicSound			(priority, ambientPriority),
	mTempSpecialHandle	(nullptr),
	mManager			(manager),
	mStartOffset		(0),
	mLoadingFlag		(false),
	mPreparedFlag		(false),
	mFileStream			(nullptr)
#endif
{
}

#pragma pop

void SeqSound::InitParam()
{
	BasicSound::InitParam();

	mStartOffset	= 0;
}

SeqPlayer::SetupResult SeqSound::Setup(SeqTrackAllocator *trackAllocator,
                                       u32 allocTracks,
                                       NoteOnCallback *callback)
{
	NW4RAssertPointerNonnull_Line(95, callback);

	InitParam();

	return mSeqPlayer.Setup(trackAllocator, allocTracks, GetVoiceOutCount(),
	                        callback);
}

void SeqSound::Prepare(void const *seqBase, s32 seqOffset,
                       SeqPlayer::OffsetType startOffsetType, int startOffset)
{
	NW4RAssertPointerNonnull_Line(124, seqBase);

	mSeqPlayer.SetSeqData(seqBase, seqOffset);

	if (startOffset > 0)
		Skip(startOffsetType, startOffset);

	mPreparedFlag = true;
}

void SeqSound::Prepare(ut::FileStream *fileStream, s32 seqOffset,
                       SeqPlayer::OffsetType startOffsetType, int startOffset)
{
	mFileStream			= fileStream;
	mSeqOffset			= seqOffset;
	mStartOffsetType	= startOffsetType;
	mStartOffset		= startOffset;

	mLoadingFlag = true;

	if (!LoadData(&NotifyLoadAsyncEndSeqData, this))
		Shutdown();
}

void SeqSound::NotifyLoadAsyncEndSeqData(bool result, void const *seqBase,
                                         void *userData)
{
	SeqSound *sound = static_cast<SeqSound *>(userData);
	NW4RAssertPointerNonnull_Line(178, sound);

	sound->mLoadingFlag = false;

	if (result == false)
	{
		sound->Stop(0);
		return;
	}

	NW4RAssertPointerNonnull_Line(189, seqBase);
	sound->mSeqPlayer.SetSeqData(seqBase, sound->mSeqOffset);

	if (sound->mStartOffset > 0)
		sound->mSeqPlayer.Skip(sound->mStartOffsetType, sound->mStartOffset);

	sound->mPreparedFlag = true;
}

void SeqSound::Skip(SeqPlayer::OffsetType offsetType, int offset)
{
	mSeqPlayer.Skip(offsetType, offset);
}

void SeqSound::Shutdown()
{
	if (mLoadingFlag)
		TaskManager::GetInstance().CancelTask(&mSeqLoadTask);

	if (mFileStream)
	{
		mFileStream->Close();
		mFileStream = nullptr;
	}

	BasicSound::Shutdown();
	mManager->Free(this);
}

// SeqSound::SetTempoRatio ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13849)
DECOMP_FORCE(NW4RAssert_String(tempoRatio >= 0.0f));

void SeqSound::SetChannelPriority(int priority)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(266, priority, 0, 127);

	mSeqPlayer.SetChannelPriority(priority);
}

void SeqSound::SetReleasePriorityFix(bool fix)
{
	mSeqPlayer.SetReleasePriorityFix(fix);
}

void SeqSound::SetSeqUserprocCallback(SeqPlayer::SeqUserprocCallback *callback,
                                      void *arg)
{
	mSeqPlayer.SetSeqUserprocCallback(callback, arg);
}

void SeqSound::OnUpdatePlayerPriority()
{
	mManager->UpdatePriority(this, CalcCurrentPlayerPriority());
}

// SeqSound::SetTrackVolume ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13857)
DECOMP_FORCE(NW4RAssert_String(volume >= 0.0f));

// SeqSound::SetTrackPitch ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13858)
DECOMP_FORCE(NW4RAssert_String(pitch >= 0.0f));

// SeqSound::ReadVariable? maybe all of them?
DECOMP_FORCE(NW4RAssertPointerNonnull_String(var));

// SeqSound::ReadGlobalVariable? maybe both of them?
DECOMP_FORCE(NW4RAssertHeaderClampedLValue_String(varNo));

// SeqSound::ReadTrackVariable? maybe both of them?
DECOMP_FORCE(NW4RAssertHeaderClampedLValue_String(trackNo));

bool SeqSound::IsAttachedTempSpecialHandle()
{
	return mTempSpecialHandle != nullptr;
}

void SeqSound::DetachTempSpecialHandle()
{
	mTempSpecialHandle->DetachSound();
}

bool SeqSound::LoadData(SeqLoadTask::Callback *callback, void *callbackArg)
{
	PlayerHeap *heap = static_cast<BasicSound *>(callbackArg)->GetPlayerHeap();
	if (!heap)
		return false;

	int fileSize = mFileStream->GetSize();
	void *buffer = heap->Alloc(fileSize);
	if (!buffer)
		return false;

	mSeqLoadTask.mFileStream	= mFileStream;
	mSeqLoadTask.mBuffer		= buffer;
	mSeqLoadTask.mBufferSize	= fileSize;
	mSeqLoadTask.mCallback		= callback;
	mSeqLoadTask.mCallbackData	= this;

	TaskManager::GetInstance().AppendTask(&mSeqLoadTask,
	                                      TaskManager::PRIORITY_MIDDLE);

	return true;
}

SeqSound::SeqLoadTask::SeqLoadTask() :
	mFileStream		(nullptr),
	mBuffer			(nullptr),
	mCallback		(nullptr),
	mCallbackData	(nullptr)
{
}

void SeqSound::SeqLoadTask::Execute()
{
	mFileStream->Seek(0, ut::FileStream::SEEK_ORIGIN_SET);

	s32 readSize = mFileStream->Read(mBuffer, mBufferSize);

	mFileStream = nullptr;

	if (readSize == DVD_ECANCELED)
	{
		if (mCallback)
			(*mCallback)(false, nullptr, mCallbackData);

		return;
	}

	if (readSize != mBufferSize)
	{
		NW4RCheckMessage_Line(716, readSize != 0,
		                      "failed to load sequence data\n");

		if (mCallback)
			(*mCallback)(false, nullptr, mCallbackData);

		return;
	}

	SeqFile const *seq = static_cast<SeqFile const *>(mBuffer); // What
	SeqFileReader reader(seq);
	void const *seqBase = reader.GetBaseAddress();

	if (mCallback)
		(*mCallback)(true, seqBase, mCallbackData);
}

void SeqSound::SeqLoadTask::Cancel()
{
	if (mCallback)
		(*mCallback)(false, nullptr, mCallbackData);
}

void SeqSound::SeqLoadTask::OnCancel()
{
	mCallback = nullptr;

	ut::FileStream *fileStream = mFileStream;
	if (fileStream)
		fileStream->Cancel();
}

}}} // namespace nw4r::snd::detail
