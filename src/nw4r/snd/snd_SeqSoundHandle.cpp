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

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

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
