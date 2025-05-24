#ifndef NW4R_SND_MIDI_SEQ_TRACK_ALLOCATOR_H
#define NW4R_SND_MIDI_SEQ_TRACK_ALLOCATOR_H

/*******************************************************************************
 * headers
 */

#include "SeqTrackAllocator.h"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	class MidiSeqTrackAllocator : public SeqTrackAllocator
	{
		MidiSeqTrackAllocator();
	};
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_MIDI_SEQ_TRACK_ALLOCATOR_H
