#include "nw4r/snd/WaveSound.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_WaveSound.cpp
 */

/*******************************************************************************
 * headers
 */

#include <types.h>

#include "nw4r/snd/BasicSound.h"
#include "nw4r/snd/SoundInstanceManager.h"
#include "nw4r/snd/WaveSoundHandle.h"
#include "nw4r/snd/WsdPlayer.h"

#include "nw4r/ut/RuntimeTypeInfo.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd { namespace detail
{
	// .sbss
	ut::detail::RuntimeTypeInfo const WaveSound::typeInfo(
		&BasicSound::typeInfo);
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

WaveSound::WaveSound(SoundInstanceManager<WaveSound> *manager, int priority,
                     int ambientPriority) :
	BasicSound			(priority, ambientPriority),
	mTempSpecialHandle	(nullptr),
	mManager			(manager),
	mPreparedFlag		(false)
{
}

bool WaveSound::Prepare(void const *waveSoundBase, s32 waveSoundOffset,
                        WsdPlayer::StartOffsetType startOffsetType, s32 offset,
                        WsdPlayer::WsdCallback const *callback,
                        register_t callbackData)
{
	NW4RAssertPointerNonnull_Line(74, waveSoundBase);
	NW4RAssertPointerNonnull_Line(75, callback);

	InitParam();

	bool result =
		mWsdPlayer.Prepare(waveSoundBase, waveSoundOffset, startOffsetType,
	                       offset, GetVoiceOutCount(), callback, callbackData);
	if (!result)
		return false;

	mPreparedFlag = true;
	return true;
}

void WaveSound::Shutdown()
{
	BasicSound::Shutdown();

	mManager->Free(this);
}

void WaveSound::SetChannelPriority(int priority)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(124, priority, BasicSound::PRIORITY_MIN,
	                                    BasicSound::PRIORITY_MAX);

	mWsdPlayer.SetChannelPriority(priority);
}

void WaveSound::SetReleasePriorityFix(bool flag)
{
	mWsdPlayer.SetReleasePriorityFix(flag);
}

void WaveSound::OnUpdatePlayerPriority()
{
	mManager->UpdatePriority(this, CalcCurrentPlayerPriority());
}

bool WaveSound::IsAttachedTempSpecialHandle()
{
	return mTempSpecialHandle != nullptr;
}

void WaveSound::DetachTempSpecialHandle()
{
	mTempSpecialHandle->DetachSound();
}

}}} // namespace nw4r::snd::detail
