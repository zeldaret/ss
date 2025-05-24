#include "nw4r/snd/SoundStartable.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SoundStartable.cpp
 */

/*******************************************************************************
 * headers
 */

#include <types.h> // u32

#include "nw4r/snd/BasicSound.h"
#include "nw4r/snd/SoundHandle.h"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

SoundStartable::StartResult SoundStartable::detail_StartSound(
	SoundHandle *handle, u32 soundId, StartInfo const *startInfo)
{
	StartResult result = detail_SetupSound(handle, soundId, false, startInfo);
	if (result != START_SUCCESS)
		return result;

	handle->StartPrepared();
	return START_SUCCESS;
}

SoundStartable::StartResult SoundStartable::detail_HoldSound(
	SoundHandle *handle, u32 soundId, StartInfo const *startInfo)
{
	if (handle->IsAttachedSound() && soundId == handle->GetId())
	{
		handle->detail_GetAttachedSound()->SetAutoStopCounter(1);
		return START_SUCCESS;
	}

	StartResult result = detail_SetupSound(handle, soundId, true, startInfo);
	if (result != START_SUCCESS)
		return result;

	handle->StartPrepared();
	handle->detail_GetAttachedSound()->SetAutoStopCounter(1);
	return START_SUCCESS;
}

}} // namespace nw4r::snd
