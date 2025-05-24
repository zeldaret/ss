#include "nw4r/snd/StrmSoundHandle.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_StrmSoundHandle.cpp
 */

/*******************************************************************************
 * headers
 */

#include <types.h> // nullptr

#include "nw4r/snd/StrmSound.h"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

void StrmSoundHandle::DetachSound()
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
