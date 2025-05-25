#include "nw4r/snd/snd_PlayerHeap.h"

/*******************************************************************************
 * headers
 */

#include <cstddef> // NULL

#include "common.h"

#include "nw4r/snd/snd_DisposeCallbackManager.h"
#include "nw4r/snd/snd_SoundThread.h" // SoundThread::AutoLock

#include "nw4r/ut/ut_algorithm.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class BasicSound; }}}

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

PlayerHeap::PlayerHeap() :
	mSound			(nullptr),
	mPlayer			(nullptr),
	mStartAddress	(nullptr),
	mEndAddress		(nullptr),
	mAllocAddress	(nullptr)
{
}

PlayerHeap::~PlayerHeap()
{
	Destroy();
}

bool PlayerHeap::Create(void *startAddress, u32 size)
{
	void *endAddress = ut::AddOffsetToPtr(startAddress, size);
	startAddress = ut::RoundUp(startAddress, 32);

	if (startAddress > endAddress)
		return false;

	mStartAddress	= startAddress;
	mEndAddress		= endAddress;
	mAllocAddress	= mStartAddress;

	return true;
}

void PlayerHeap::Destroy()
{
	Clear();
	mAllocAddress = nullptr;
}

void *PlayerHeap::Alloc(u32 size)
{
	NW4RAssertAligned_Line(108, mAllocAddress, 32);

	void *endp = ut::AddOffsetToPtr(mAllocAddress, size);
	if (endp > mEndAddress)
		return nullptr;

	void *allocAddress = mAllocAddress;
	mAllocAddress = ut::RoundUp(endp, 32);
	return allocAddress;
}

void PlayerHeap::Clear()
{
	SoundThread::AutoLock lockForDispose;

	DisposeCallbackManager::GetInstance().Dispose(
		mStartAddress, ut::GetOffsetFromPtr(mStartAddress, mAllocAddress),
		nullptr);

	DisposeCallbackManager::GetInstance().DisposeWave(
		mStartAddress, ut::GetOffsetFromPtr(mStartAddress, mAllocAddress),
		nullptr);

	mAllocAddress = mStartAddress;
}

u32 PlayerHeap::GetFreeSize() const
{
	s32 offset = ut::GetOffsetFromPtr(mAllocAddress, mEndAddress);
	NW4RAssert_Line(157, offset >= 0);

	return offset;
}

void PlayerHeap::AttachSound(BasicSound *sound)
{
	NW4RAssertPointerNonnull_Line(172, sound);
	NW4RAssert_Line(173, mSound == NULL);

	mSound = sound;
}

void PlayerHeap::DetachSound(BasicSound *sound)
{
	NW4RAssertPointerNonnull_Line(189, sound);
	NW4RAssert_Line(190, sound == mSound);

	mSound = nullptr;
}

}}} // namespace nw4r::snd::detail
