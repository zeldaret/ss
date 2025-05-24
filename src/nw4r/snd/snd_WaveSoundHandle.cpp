#include "nw4r/snd/WaveSoundHandle.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_WaveSoundHandle.cpp
 */

/*******************************************************************************
 * headers
 */

#include <types.h> // nullptr

#include "nw4r/snd/BasicSound.h"
#include "nw4r/snd/SoundHandle.h"
#include "nw4r/snd/WaveSound.h"

#include "nw4r/ut/RuntimeTypeInfo.h" // ut::DynamicCast

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

WaveSoundHandle::WaveSoundHandle(SoundHandle *handle) :
	mSound	(nullptr)
{
	if (!handle)
		return;

	detail::BasicSound *basicSound = handle->detail_GetAttachedSound();
	if (!basicSound)
		return;

	if (detail::WaveSound *sound =
	        ut::DynamicCast<detail::WaveSound *>(basicSound))
	{
		NW4RAssertPointerNonnull_Line(50, sound); // ?

		mSound = sound;

		if (mSound->IsAttachedTempSpecialHandle())
			mSound->DetachTempSpecialHandle();

		mSound->mTempSpecialHandle = this;
	}
}

void WaveSoundHandle::DetachSound()
{
	if (IsAttachedSound())
	{
		if (mSound->mTempSpecialHandle == this)
			mSound->mTempSpecialHandle = nullptr;
	}

	if (mSound)
		mSound = nullptr;
}

}} // namespace nw4r::snd
