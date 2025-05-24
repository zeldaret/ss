#include "nw4r/snd/VoiceManager.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_VoiceManager.cpp
 */

/*******************************************************************************
 * headers
 */

#include <new>

#include <macros.h> // NW4R_RANGE_FOR_NO_AUTO_INC
#include <types.h>

#include "nw4r/snd/DisposeCallbackManager.h"
#include "nw4r/snd/Voice.h"

#include "nw4r/ut/Lock.h" // ut::AutoInterruptLock

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

VoiceManager &VoiceManager::GetInstance()
{
	static VoiceManager instance;

	return instance;
}

VoiceManager::VoiceManager() :
	mInitialized(false)
{
}

u32 VoiceManager::GetRequiredMemSize(int voiceCount)
{
	return sizeof(Voice) * voiceCount;
}

void VoiceManager::Setup(void *mem, u32 memSize)
{
	if (mInitialized)
		return;

	u32 voiceCount = memSize / sizeof(Voice);
	byte_t *ptr = static_cast<byte_t *>(mem);

	for (int i = 0; i < voiceCount; i++)
	{
		mFreeVoiceList.PushBack(new (ptr) Voice);

		ptr += sizeof(Voice);
	}

	NW4RAssert_Line(64, ptr <= reinterpret_cast<u8*>( mem ) + memSize);

	mInitialized = true;
}

void VoiceManager::Shutdown()
{
	if (!mInitialized)
		return;

	StopAllVoices();

	while (!mFreeVoiceList.IsEmpty())
	{
		Voice &voice = mFreeVoiceList.GetFront();
		mFreeVoiceList.PopFront();

		voice.~Voice();
	}

	mInitialized = false;
}

void VoiceManager::StopAllVoices()
{
	ut::AutoInterruptLock lock;

	while (!mPrioVoiceList.IsEmpty())
	{
		Voice &voice = mPrioVoiceList.GetFront();

		voice.Stop();

		if (voice.mCallback)
		{
			(*voice.mCallback)(&voice, Voice::CALLBACK_STATUS_CANCEL,
			                   voice.mCallbackData);
		}

		voice.Free();
	}
}

Voice *VoiceManager::AllocVoice(int voiceChannelCount, int voiceOutCount,
                                int priority, Voice::Callback *callback,
                                void *callbackData)
{
	ut::AutoInterruptLock lock;

	if (mFreeVoiceList.IsEmpty() && !DropLowestPriorityVoice(priority))
		return nullptr;

	Voice &voice = mFreeVoiceList.GetFront();
	if (!voice.Acquire(voiceChannelCount, voiceOutCount, priority, callback,
	                   callbackData))
	{
		NW4RWarningMessage_Line(128, "Voice Acquisition failed!\n");
		return nullptr;
	}

	voice.mPriority = priority & Voice::PRIORITY_MAX;
	AppendVoiceList(&voice);
	UpdateEachVoicePriority(mPrioVoiceList.GetIteratorFromPointer(&voice),
	                        mPrioVoiceList.GetEndIter());
	DisposeCallbackManager::GetInstance().RegisterDisposeCallback(&voice);

	return &voice;
}

void VoiceManager::FreeVoice(Voice *voice)
{
	NW4RAssertPointerNonnull_Line(148, voice);

	ut::AutoInterruptLock lock;

	DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(voice);
	RemoveVoiceList(voice);
}

void VoiceManager::UpdateAllVoices()
{
	NW4R_RANGE_FOR_NO_AUTO_INC(itr, mPrioVoiceList)
	{
		decltype(itr) curItr = itr++;

		curItr->StopFinished();
	}

	NW4R_RANGE_FOR_NO_AUTO_INC(itr, mPrioVoiceList)
	{
		decltype(itr) curItr = itr++;

		curItr->Calc();
	}

	ut::AutoInterruptLock lock;

	NW4R_RANGE_FOR_NO_AUTO_INC(itr, mPrioVoiceList)
	{
		decltype(itr) curItr = itr++;

		curItr->Update();
	}
}

void VoiceManager::NotifyVoiceUpdate()
{
	ut::AutoInterruptLock lock;

	NW4R_RANGE_FOR_NO_AUTO_INC(itr, mPrioVoiceList)
	{
		decltype(itr) curItr = itr++;

		curItr->ResetDelta();
	}
}

void VoiceManager::AppendVoiceList(Voice *voice)
{
	ut::AutoInterruptLock lock;

	mFreeVoiceList.Erase(voice);

	Voice::LinkList::ReverseIterator it = mPrioVoiceList.GetEndReverseIter();
	for (; it != mPrioVoiceList.GetBeginReverseIter(); ++it)
	{
		if (it->GetPriority() <= voice->GetPriority())
			break;
	}

	mPrioVoiceList.Insert(it.GetBase(), voice);
}

void VoiceManager::RemoveVoiceList(Voice *voice)
{
	ut::AutoInterruptLock lock;

	mPrioVoiceList.Erase(voice);
	mFreeVoiceList.PushBack(voice);
}

void VoiceManager::ChangeVoicePriority(Voice *voice)
{
	ut::AutoInterruptLock lock;

	RemoveVoiceList(voice);
	AppendVoiceList(voice);

	UpdateEachVoicePriority(mPrioVoiceList.GetIteratorFromPointer(voice),
	                        mPrioVoiceList.GetEndIter());
}

void VoiceManager::UpdateEachVoicePriority(
	Voice::LinkList::Iterator const &beginItr,
	Voice::LinkList::Iterator const &endItr)
{
	for (Voice::LinkList::Iterator it = beginItr; it != endItr; ++it)
	{
		if (it->GetPriority() <= 1)
			return;

		if (it->GetPriority() != Voice::PRIORITY_MAX)
			it->UpdateVoicesPriority();
	}
}

void VoiceManager::UpdateAllVoicesSync(byte4_t syncFlag)
{
	ut::AutoInterruptLock lock;

	NW4R_RANGE_FOR_NO_AUTO_INC(itr, mPrioVoiceList)
	{
		decltype(itr) curItr = itr++;

		if (curItr->mActiveFlag)
			curItr->mSyncFlag |= syncFlag;
	}
}

int VoiceManager::DropLowestPriorityVoice(int priority)
{
	int dropCount = 0;

	if (mFreeVoiceList.IsEmpty())
	{
		Voice &dropVoice = mPrioVoiceList.GetFront();

		if (dropVoice.GetPriority() > priority)
			return 0;

		dropCount = dropVoice.GetPhysicalVoiceCount();

		dropVoice.Stop();
		dropVoice.Free();

		if (dropVoice.mCallback)
		{
			(*dropVoice.mCallback)(&dropVoice,
			                       Voice::CALLBACK_STATUS_DROP_VOICE,
			                       dropVoice.mCallbackData);
		}
	}

	return dropCount;
}

}}} // namespace nw4r::snd::detail
