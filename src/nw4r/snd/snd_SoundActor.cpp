/* Only implemented to the extent necessary to match early instantiations of
 * inline functions and data sections.
 */

#include "nw4r/snd/snd_SoundActor.h"

#include "common.h"

#include "nw4r/snd/snd_SoundArchivePlayer.h"

nw4r::snd::SoundActor::SoundActor() :
	mSoundArchivePlayer	(*new SoundArchivePlayer)
{
	// DECOMP_FORCE_CLASS_METHOD
	(void)mSoundArchivePlayer.detail_ConvertLabelStringToSoundId(nullptr);
}
