#include "nw4r/snd/snd_SoundStartable.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SoundStartable.cpp
 */

/*******************************************************************************
 * headers
 */

#include "common.h" // u32

#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundHandle.h"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

SoundStartable::StartResult SoundStartable::detail_StartSound(
	SoundHandle *handle, u32 soundId, const StartInfo *startInfo)
{
	StartResult result = detail_SetupSound(handle, soundId, false, startInfo);
	if (result != START_SUCCESS)
		return result;

	handle->StartPrepared();
	return START_SUCCESS;
}

SoundStartable::StartResult SoundStartable::detail_StartSound(
	SoundHandle *pHandle, const char *label, const StartInfo *pStartInfo)
{
	u32 soundId = detail_ConvertLabelStringToSoundId(label);
	if (soundId == -1)
		return START_ERR_INVALID_LABEL_STRING;

	return detail_StartSound(pHandle, soundId, pStartInfo);
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

SoundStartable::StartResult SoundStartable::detail_HoldSound(
	SoundHandle *pHandle, const char *label, const StartInfo *pStartInfo)
{
	u32 soundId = detail_ConvertLabelStringToSoundId(label);
	if (soundId == -1)
		return START_ERR_INVALID_LABEL_STRING;

	return detail_HoldSound(pHandle, soundId, pStartInfo);
}

SoundStartable::StartResult SoundStartable::detail_PrepareSound(
	SoundHandle *pHandle, u32 id,
	const StartInfo *pStartInfo)
{
	return detail_SetupSound(pHandle, id, false, pStartInfo);
}

SoundStartable::StartResult SoundStartable::detail_PrepareSound(
	SoundHandle *pHandle, const char *label,
	const StartInfo *pStartInfo)
{
	u32 soundId = detail_ConvertLabelStringToSoundId(label);
	if (soundId == -1)
		return START_ERR_INVALID_LABEL_STRING;

	return detail_PrepareSound(pHandle, soundId, pStartInfo);
}

}} // namespace nw4r::snd
