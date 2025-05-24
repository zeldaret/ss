#include "nw4r/snd/MmlSeqTrack.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_MmlSeqTrack.cpp
 */

/*******************************************************************************
 * headers
 */

#include "nw4r/snd/MmlParser.h"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

MmlSeqTrack::MmlSeqTrack() {}

MmlSeqTrack::ParseResult MmlSeqTrack::Parse(bool doNoteOn)
{
	return mParser->Parse(this, doNoteOn);
}

}}} // namespace nw4r::snd::detail
