#ifndef NW4R_SND_MIDI_SEQ_TRACK_H
#define NW4R_SND_MIDI_SEQ_TRACK_H

/*******************************************************************************
 * headers
 */

#include "SeqTrack.h"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	class MidiSeqTrack : public SeqTrack
	{
		MidiSeqTrack();
	};
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_MIDI_SEQ_TRACK_H
