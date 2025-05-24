/* Only implemented to the extent necessary to match data sections. */

#include <types.h> // nullptr
#include <decomp.h>

#include "nw4r/snd/Util.h"

DECOMP_FORCE(nw4r::snd::detail::Util::GetDataRefAddress0(
	*static_cast<nw4r::snd::detail::Util::DataRef<char> const *>(nullptr),
	nullptr));
