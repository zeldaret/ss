#include "nw4r/snd/SoundPlayer.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SoundPlayer.cpp
 */

/*******************************************************************************
 * headers
 */

#include <climits> // INT_MAX

#include <decomp.h>
#include <macros.h>
#include <types.h> // nullptr

#include "nw4r/snd/BasicSound.h"
#include "nw4r/snd/global.h" // AUX_BUS_NUM
#include "nw4r/snd/PlayerHeap.h"
#include "nw4r/snd/SoundThread.h"

#include "nw4r/ut/inlines.h" // ut::Clamp

#include <nw4r/NW4RAssert.hpp>

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

SoundPlayer::SoundPlayer() :
	mPlayableCount		(1),
	mPlayableLimit		(INT_MAX),
	mVolume				(1.0f),
	mLpfFreq			(0.0f),
	mOutputLineFlag		(1),
	mMainOutVolume		(1.0f),
	mBiquadType			(0),
	mBiquadValue		(0.0f),
	mMainSend			(0.0f)
{
	for (int i = 0; i < AUX_BUS_NUM; i++)
		mFxSend[i] = 0.0f;
}

SoundPlayer::~SoundPlayer()
{
	StopAllSound(0);
}

void SoundPlayer::Update()
{
	detail::SoundThread::AutoLock lock;

	NW4R_RANGE_FOR_NO_AUTO_INC(itr, mSoundList)
	{
		decltype(itr) curItr = itr++;

		curItr->Update();
	}

	detail_SortPriorityList();
}

void SoundPlayer::StopAllSound(int fadeFrames)
{
	detail::SoundThread::AutoLock lock;

	NW4R_RANGE_FOR_NO_AUTO_INC(itr, mSoundList)
	{
		decltype(itr) curItr = itr++;

		curItr->Stop(fadeFrames);
	}
}

void SoundPlayer::PauseAllSound(bool flag, int fadeFrames)
{
	detail::SoundThread::AutoLock lock;

	NW4R_RANGE_FOR_NO_AUTO_INC(itr, mSoundList)
	{
		decltype(itr) curItr = itr++;

		curItr->Pause(flag, fadeFrames);
	}
}

// SoundPlayer::SetVolume ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14397)
/* also __FILE__ because this is the first assert in the file so it needs to
 * reference that first as well
 */
DECOMP_FORCE(__FILE__);
DECOMP_FORCE(NW4RAssert_String(volume >= 0.0f));

// SoundPlayer::SetFxSend ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14405)
DECOMP_FORCE(NW4RAssertHeaderClampedLValue_String(bus));

void SoundPlayer::RemoveSoundList(detail::BasicSound *sound)
{
	detail::SoundThread::AutoLock lock;

	mSoundList.Erase(sound);
	sound->DetachSoundPlayer(this);
}

void SoundPlayer::InsertPriorityList(detail::BasicSound *sound)
{
	detail::SoundThread::AutoLock lock;

	decltype(mPriorityList.GetBeginIter()) itr = mPriorityList.GetBeginIter();
	for (; itr != mPriorityList.GetEndIter(); ++itr)
	{
		if (sound->CalcCurrentPlayerPriority()
		    < itr->CalcCurrentPlayerPriority())
		{
			break;
		}
	}

	mPriorityList.Insert(itr, sound);
}

void SoundPlayer::RemovePriorityList(detail::BasicSound *sound)
{
	detail::SoundThread::AutoLock lock;

	mPriorityList.Erase(sound);
}

void SoundPlayer::detail_SortPriorityList(detail::BasicSound *sound)
{
	RemovePriorityList(sound);
	InsertPriorityList(sound);
}

void SoundPlayer::detail_SortPriorityList()
{
	detail::SoundThread::AutoLock lock;

	if (mPriorityList.GetSize() < 2)
		return;

	static detail::BasicSound::SoundPlayerPriorityLinkList
		tmplist[detail::BasicSound::PRIORITY_MAX + 1];

	while (!mPriorityList.IsEmpty())
	{
		detail::BasicSound &front = mPriorityList.GetFront();
		mPriorityList.PopFront();
		tmplist[front.CalcCurrentPlayerPriority()].PushBack(&front);
	}

	for (int i = 0; i < (int)ARRAY_LENGTH(tmplist); i++)
	{
		while (!tmplist[i].IsEmpty())
		{
			detail::BasicSound &front = tmplist[i].GetFront();

			tmplist[i].PopFront();
			mPriorityList.PushBack(&front);
		}
	}
}

bool SoundPlayer::detail_AppendSound(detail::BasicSound *sound)
{
	NW4RAssertPointerNonnull_Line(402, sound);

	detail::SoundThread::AutoLock lock;

	int allocPriority = sound->CalcCurrentPlayerPriority();

	if (GetPlayableSoundCount() == 0)
		return false;

	while (GetPlayingSoundCount() >= GetPlayableSoundCount())
	{
		detail::BasicSound *dropSound = GetLowestPrioritySound();
		if (!dropSound)
			return false;

		if (allocPriority < dropSound->CalcCurrentPlayerPriority())
			return false;

		dropSound->Shutdown();
	}

	mSoundList.PushBack(sound);
	InsertPriorityList(sound);
	sound->AttachSoundPlayer(this);

	return true;
}

void SoundPlayer::detail_RemoveSound(detail::BasicSound *sound)
{
	RemovePriorityList(sound);
	RemoveSoundList(sound);
}

void SoundPlayer::SetPlayableSoundCount(int count)
{
	NW4RAssert_Line(453, count >= 0);

	detail::SoundThread::AutoLock lock;

	NW4RCheckMessage_Line(458, count <= mPlayableLimit,
	                      "playable sound count is over limit.");

	mPlayableCount = ut::Clamp(count, 0, mPlayableLimit);

	while (GetPlayingSoundCount() > GetPlayableSoundCount())
	{
		detail::BasicSound *dropSound = GetLowestPrioritySound();
		NW4RAssertPointerNonnull_Line(467, dropSound);

		dropSound->Shutdown();
	}
}

void SoundPlayer::detail_SetPlayableSoundLimit(int limit)
{
	NW4RAssert_Line(483, limit >= 0);

	mPlayableLimit = limit;
}

bool SoundPlayer::detail_CanPlaySound(int startPriority)
{
	detail::SoundThread::AutoLock lock;

	if (GetPlayableSoundCount() == 0)
		return false;

	if (GetPlayingSoundCount() >= GetPlayableSoundCount())
	{
		detail::BasicSound *dropSound = GetLowestPrioritySound();
		if (!dropSound)
			return false;

		if (startPriority < dropSound->CalcCurrentPlayerPriority())
			return false;
	}

	return true;
}

void SoundPlayer::detail_AppendPlayerHeap(detail::PlayerHeap *heap)
{
	NW4RAssertPointerNonnull_Line(524, heap);

	detail::SoundThread::AutoLock lock;

	heap->AttachSoundPlayer(this);
	mHeapList.PushBack(heap);
}

detail::PlayerHeap *SoundPlayer::detail_AllocPlayerHeap(detail::BasicSound *sound)
{
	NW4RAssertPointerNonnull_Line(557, sound);

	detail::SoundThread::AutoLock lock;

	if (mHeapList.IsEmpty())
		return nullptr;

	detail::PlayerHeap &playerHeap = mHeapList.GetFront();
	mHeapList.PopFront();

	playerHeap.AttachSound(sound);
	sound->AttachPlayerHeap(&playerHeap);
	playerHeap.Clear();

	return &playerHeap;
}

void SoundPlayer::detail_FreePlayerHeap(detail::BasicSound *sound)
{
	NW4RAssertPointerNonnull_Line(587, sound);

	detail::SoundThread::AutoLock lock;

	detail::PlayerHeap *heap = sound->GetPlayerHeap();
	if (!heap)
		return;

	heap->DetachSound(sound);
	sound->DetachPlayerHeap(heap);
	mHeapList.PushBack(heap);
}

}} // namespace nw4r::snd
