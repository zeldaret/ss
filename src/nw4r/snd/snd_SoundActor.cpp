/* Only implemented to the extent necessary to match early instantiations of
 * inline functions and data sections.
 */

#include "nw4r/snd/SoundActor.h"

#include <types.h>

#include "nw4r/snd/SoundArchivePlayer.h"

nw4r::snd::SoundActor::SoundActor() :
	mSoundArchivePlayer	(*new SoundArchivePlayer)
{
	// DECOMP_FORCE_CLASS_METHOD
	(void)mSoundArchivePlayer.detail_ConvertLabelStringToSoundId(nullptr);
}
