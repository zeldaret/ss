#include "nw4r/snd/SeqSoundHandle.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SeqSoundHandle.cpp
 */

/*******************************************************************************
 * headers
 */

#include <types.h> // nullptr

#include "nw4r/snd/SeqSound.h"

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
