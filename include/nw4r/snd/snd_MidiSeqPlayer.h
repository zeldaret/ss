#ifndef NW4R_SND_MIDI_SEQ_PLAYER_H
#define NW4R_SND_MIDI_SEQ_PLAYER_H

/*******************************************************************************
 * headers
 */

#include "nw4r/snd/snd_SeqPlayer.h"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	class MidiSeqPlayer : public SeqPlayer
	{
		MidiSeqPlayer();
		virtual void ChannelCallback(Channel *) {}
	};
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_MIDI_SEQ_PLAYER_H
