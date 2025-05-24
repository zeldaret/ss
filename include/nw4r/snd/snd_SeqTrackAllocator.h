#ifndef NW4R_SND_SEQ_TRACK_ALLOCATOR_H
#define NW4R_SND_SEQ_TRACK_ALLOCATOR_H

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class SeqPlayer; }}}
namespace nw4r { namespace snd { namespace detail { class SeqTrack; }}}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2db61

	class SeqTrackAllocator
	{
	// methods
	public:
		// cdtors
		virtual ~SeqTrackAllocator() {}

		// virtual function ordering
		// vtable SeqTrackAllocator
		virtual SeqTrack *AllocTrack(SeqPlayer *player) = 0;
		virtual void FreeTrack(SeqTrack *track) = 0;
		virtual int GetAllocatableTrackCount() const = 0;

	// members
	private:
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_SEQ_TRACK_ALLOCATOR_H
