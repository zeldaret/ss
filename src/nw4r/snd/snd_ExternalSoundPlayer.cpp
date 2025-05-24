#include "nw4r/snd/snd_ExternalSoundPlayer.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_ExternalSoundPlayer.cpp
 */

/*******************************************************************************
 * headers
 */

#include <decomp.h>
#include "common.h" // nullptr

#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundThread.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace ut { struct LinkListNode; }}

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

// not sure which one uses this exactly, maybe StopAllSound?
DECOMP_FORCE_CLASS_METHOD(
	BasicSound::ExtSoundPlayerPlayLinkList,
	GetPointerFromNode(static_cast<ut::LinkListNode *>(nullptr)));

bool ExternalSoundPlayer::AppendSound(BasicSound *sound)
{
	NW4RAssertPointerNonnull_Line(129, sound);

	SoundThread::AutoLock lock;

	int allocPriority = sound->CalcCurrentPlayerPriority();

	if (GetPlayableSoundCount() == 0)
		return false;

	while (GetPlayingSoundCount() >= GetPlayableSoundCount())
	{
		BasicSound *dropSound = GetLowestPrioritySound();
		if (!dropSound)
			return false;

		if (allocPriority < dropSound->CalcCurrentPlayerPriority())
			return false;

		dropSound->Shutdown();
	}

	mSoundList.PushBack(sound);
	sound->AttachExternalSoundPlayer(this);

	return true;
}

void ExternalSoundPlayer::RemoveSound(BasicSound *sound)
{
	mSoundList.Erase(sound);
	sound->DetachExternalSoundPlayer(this);
}

bool ExternalSoundPlayer::detail_CanPlaySound(int startPriority)
{
	if (GetPlayableSoundCount() == 0)
		return false;

	if (GetPlayingSoundCount() >= GetPlayableSoundCount())
	{
		BasicSound *dropSound = GetLowestPrioritySound();
		if (!dropSound)
			return false;

		if (startPriority < dropSound->CalcCurrentPlayerPriority())
			return false;
	}

	return true;
}

BasicSound *ExternalSoundPlayer::GetLowestPrioritySound()
{
	int priority = 128;
	BasicSound *sound = nullptr;

	NW4R_RANGE_FOR(itr, mSoundList)
	{
		int itrPriority = itr->CalcCurrentPlayerPriority();

		if (priority > itrPriority)
		{
			sound = &(*itr);
			priority = itrPriority;
		}
	}

	return sound;
}

}}} // namespace nw4r::snd::detail
