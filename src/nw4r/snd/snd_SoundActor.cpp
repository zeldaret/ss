/* Only implemented to the extent necessary to match early instantiations of
 * inline functions and data sections.
 */

#include "nw4r/snd/snd_SoundActor.h"

#include "common.h"

#include "nw4r/snd/snd_SoundArchivePlayer.h"

namespace nw4r { namespace snd {

SoundActor::SoundActor(SoundArchivePlayer &player) :
	mSoundArchivePlayer	(player)
{
	
	for (int i = 0; i < ACTOR_PLAYER_COUNT; i++)
	{
		mActorPlayer[i].SetPlayableSoundCount(i == 0 ? INT_MAX : 1);
	}
}

SoundActor::~SoundActor()
{
	for (int i = 0; i < ACTOR_PLAYER_COUNT; i++)
	{
		mActorPlayer[i].DetachSoundActorAll(this);
	}
}

void SoundActor::StopAllSound(int fadeFrames)
{
	for (int i = 0; i < ACTOR_PLAYER_COUNT; i++)
	{
		mActorPlayer[i].StopAllSound(fadeFrames);
	}
}

void SoundActor::PauseAllSound(bool flag, int fadeFrames)
{
	for (int i = 0; i < ACTOR_PLAYER_COUNT; i++)
	{
		mActorPlayer[i].PauseAllSound(flag, fadeFrames);
	}
}

int SoundActor::GetPlayingSoundCount(int playerId) const
{
	return mActorPlayer[playerId].GetPlayingSoundCount();
}

SoundStartable::StartResult SoundActor::SetupSound(SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void* arg)
{
	return mSoundArchivePlayer.detail_SetupSoundImpl(pHandle, soundId, nullptr, this, *(bool*)arg, pStartInfo);
}

SoundStartable::StartResult SoundActor::detail_SetupSoundWithAmbientInfo(SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, detail::BasicSound::AmbientInfo *pAmbientInfo, void* arg)
{
	return mSoundArchivePlayer.detail_SetupSoundImpl(pHandle, soundId, pAmbientInfo, this, *(bool*)arg, pStartInfo);
}

SoundStartable::StartResult SoundActor::detail_SetupSound(SoundHandle *pHandle, u32 soundId, bool holdFlag, const StartInfo *pStartInfo)
{
	return SetupSound(pHandle, soundId, pStartInfo, &holdFlag);
}

u32 SoundActor::detail_ConvertLabelStringToSoundId(const char* label)
{
	return mSoundArchivePlayer.detail_ConvertLabelStringToSoundId(label);
}

}}
