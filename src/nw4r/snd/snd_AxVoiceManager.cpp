#include "nw4r/snd/AxVoiceManager.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_AxVoiceManager.cpp
 */

/*******************************************************************************
 * headers
 */

#include <new>

#include <types.h>

#include "nw4r/snd/AxVoice.h"

#include "nw4r/ut/Lock.h" // ut::AutoInterruptLock

#if 0
#include <revolution/AX/AXAlloc.h>
#include <revolution/AX/AXVPB.h>
#else
#include <context_rvl.h>
#endif

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

AxVoiceManager &AxVoiceManager::GetInstance()
{
	static AxVoiceManager instance;

	return instance;
}

AxVoiceManager::AxVoiceManager() :
	mInitialized	(false)
{
}

u32 AxVoiceManager::GetRequiredMemSize(int axVoiceCount)
{
	return sizeof(AxVoice) * (VOICE_COUNT_MARGIN + axVoiceCount);
}

void AxVoiceManager::Setup(void *mem, u32 memSize)
{
	if (mInitialized)
		return;

	mVoiceCount = memSize / sizeof(AxVoice);
	byte_t *ptr = static_cast<byte_t *>(mem);

	for (u32 i = 0; i < mVoiceCount; i++)
	{
		mFreeVoiceList.PushBack(new (ptr) AxVoice);

		ptr += sizeof(AxVoice);
	}

	NW4RAssert_Line(65, ptr <= reinterpret_cast<u8*>( mem ) + memSize);

	mInitialized = true;
}

void AxVoiceManager::Shutdown()
{
	if (!mInitialized)
		return;

	while (!mActiveVoiceList.IsEmpty())
	{
		AxVoice &voice = mActiveVoiceList.GetFront();
		mActiveVoiceList.PopFront();

		if (voice.mVpb.IsAvailable())
		{
			voice.Stop();

			if (voice.mCallback)
			{
				(*voice.mCallback)(&voice, AxVoice::CALLBACK_STATUS_CANCEL,
				                   voice.mCallbackData);
			}

			FreeAxVoice(&voice);
		}
	}

	while (!mFreeReservedVoiceList.IsEmpty())
	{
		AxVoice &voice = mFreeReservedVoiceList.GetFront();
		// ERRATUM: Pop from wrong list
		mActiveVoiceList.PopFront();

		if (voice.mVpb.IsAvailable())
		{
			voice.Stop();

			if (voice.mCallback)
			{
				(*voice.mCallback)(&voice, AxVoice::CALLBACK_STATUS_CANCEL,
				                   voice.mCallbackData);
			}

			FreeAxVoice(&voice);
		}
	}

	while (!mFreeVoiceList.IsEmpty())
	{
		AxVoice &voice = mFreeVoiceList.GetFront();
		mFreeVoiceList.PopFront();

		voice.~AxVoice();
	}

	mInitialized = false;
}

AxVoice *AxVoiceManager::Alloc()
{
	ut::AutoInterruptLock lock;

	FreeAllReservedAxVoice();

	if (mFreeVoiceList.IsEmpty())
		return nullptr;

	AxVoice *ptr = &mFreeVoiceList.GetFront();
	mFreeVoiceList.PopFront();

	AxVoice *voice = new (ptr) AxVoice();
	mActiveVoiceList.PushBack(ptr);

	return voice;
}

void AxVoiceManager::Free(AxVoice *voice)
{
	NW4RAssert_Line(154, voice);

	voice->~AxVoice();

	ut::AutoInterruptLock lock;

	if (voice->mReserveForFreeFlag)
		mFreeReservedVoiceList.Erase(voice);
	else
		mActiveVoiceList.Erase(voice);

	mFreeVoiceList.PushBack(voice);
}

void AxVoiceManager::ReserveForFree(AxVoice *voice)
{
	NW4RAssert_Line(169, voice);

	ut::AutoInterruptLock lock;

	mActiveVoiceList.Erase(voice);
	mFreeReservedVoiceList.PushBack(voice);
}

AxVoice *AxVoiceManager::AcquireAxVoice(u32 priority,
                                        AxVoice::Callback *callback,
                                        void *callbackData)
{
	ut::AutoInterruptLock lock;

	AxVoice *voice = Alloc();
	if (!voice)
		return nullptr;

	AXVPB *vpb = AXAcquireVoice(priority, &AxVoice::VoiceCallback,
	                            reinterpret_cast<register_t>(voice));
	if (!vpb)
	{
		Free(voice);
		return nullptr;
	}

	voice->mVpb.Set(vpb);
	voice->mCallback = callback;
	voice->mCallbackData = callbackData;

	return voice;
}

void AxVoiceManager::FreeAxVoice(AxVoice *voice)
{
	ut::AutoInterruptLock lock;

	NW4RAssertPointerNonnull_Line(212, voice);

	if (voice->mVpb.IsAvailable())
		AXFreeVoice(voice->mVpb);

	Free(voice);
}

void AxVoiceManager::ReserveForFreeAxVoice(AxVoice *voice)
{
	ut::AutoInterruptLock lock;

	NW4RAssert_Line(226, voice);

	voice->mReserveForFreeFlag = true;
	ReserveForFree(voice);
}

void AxVoiceManager::FreeAllReservedAxVoice()
{
	while (!mFreeReservedVoiceList.IsEmpty())
	{
		AxVoice &voice = mFreeReservedVoiceList.GetFront();

		if (voice.mCallback)
		{
			(*voice.mCallback)(&voice, AxVoice::CALLBACK_STATUS_DROP_DSP,
			                voice.mCallbackData);
		}

		// NOTE: unnecessary call to GetInstance from instance-method
		GetInstance().FreeAxVoice(&voice);
	}
}

}}} // namespace nw4r::snd::detail
