#include "nw4r/snd/snd_StrmSound.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_StrmSound.cpp
 */

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundInstanceManager.h"
#include "nw4r/snd/snd_StrmPlayer.h"
#include "nw4r/snd/snd_StrmSoundHandle.h"
#include "nw4r/snd/snd_MoveValue.h"

#include "nw4r/ut/ut_RuntimeTypeInfo.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class StrmBufferPool; }}}

namespace nw4r { namespace ut { class FileStream; }}

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd { namespace detail
{
	// .sbss
	ut::detail::RuntimeTypeInfo const StrmSound::typeInfo(
		&BasicSound::typeInfo);
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

StrmSound::StrmSound(SoundInstanceManager<StrmSound> *manager, int priority,
                     int ambientPriority) :
	BasicSound			(priority, ambientPriority),
	mTempSpecialHandle	(nullptr),
	mManager			(manager)
{
}

void StrmSound::InitParam()
{
	BasicSound::InitParam();

	for (int i = 0; i < ARRAY_LENGTH(mTrackVolume); i++)
	{
		mTrackVolume[i].InitValue(0.0f);
		mTrackVolume[i].SetTarget(1.0f, 1);
	}
}

StrmPlayer::SetupResult StrmSound::Setup(StrmBufferPool *bufferPool,
                                         int allocChannelCount,
                                         u16 allocTrackFlag)
{
	NW4RAssertPointerNonnull_Line(90, bufferPool);

	InitParam();

	return mStrmPlayer.Setup(bufferPool, allocChannelCount, allocTrackFlag,
	                         GetVoiceOutCount());
}

bool StrmSound::Prepare(StrmPlayer::StartOffsetType startOffsetType, s32 offset,
                        ut::FileStream *fileStream)
{
	bool result = mStrmPlayer.Prepare(fileStream, startOffsetType, offset);
	if (!result)
	{
		mStrmPlayer.Shutdown();
		return false;
	}

	return true;
}

void StrmSound::UpdateMoveValue()
{
	BasicSound::UpdateMoveValue();

	for (int trackNo = 0; trackNo < (int)ARRAY_LENGTH(mTrackVolume); trackNo++)
	{
		if (mStrmPlayer.GetPlayerTrack(trackNo))
			mTrackVolume[trackNo].Update();
	}
}

void StrmSound::UpdateParam()
{
	BasicSound::UpdateParam();

	for (int trackNo = 0; trackNo < (int)ARRAY_LENGTH(mTrackVolume); trackNo++)
	{
		if (mStrmPlayer.GetPlayerTrack(trackNo))
		{
			mStrmPlayer.SetTrackVolume(1 << trackNo,
			                           mTrackVolume[trackNo].GetValue());
		}
	}
}

void StrmSound::Shutdown()
{
	BasicSound::Shutdown();

	mManager->Free(this);
}

void StrmSound::OnUpdatePlayerPriority()
{
	mManager->UpdatePriority(this, CalcCurrentPlayerPriority());
}

bool StrmSound::IsAttachedTempSpecialHandle()
{
	return mTempSpecialHandle != nullptr;
}

void StrmSound::DetachTempSpecialHandle()
{
	mTempSpecialHandle->DetachSound();
}

}}} // namespace nw4r::snd::detail
