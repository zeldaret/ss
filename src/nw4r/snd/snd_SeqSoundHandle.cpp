#include "nw4r/snd/snd_SeqSoundHandle.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SeqSoundHandle.cpp
 */

/*******************************************************************************
 * headers
 */

#include "common.h" // nullptr

#include "nw4r/snd/snd_SeqSound.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/ut/ut_RuntimeTypeInfo.h"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

SeqSoundHandle::SeqSoundHandle(SoundHandle *handle) :
	mSound	(nullptr)
{
	if (!handle)
		return;

	detail::BasicSound *basicSound = handle->detail_GetAttachedSound();
	if (!basicSound)
		return;

	if (detail::SeqSound *sound =
	        ut::DynamicCast<detail::SeqSound *>(basicSound))
	{

		mSound = sound;

		if (mSound->IsAttachedTempSpecialHandle())
			mSound->DetachTempSpecialHandle();

		mSound->mTempSpecialHandle = this;
	}
}

void SeqSoundHandle::DetachSound()
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
