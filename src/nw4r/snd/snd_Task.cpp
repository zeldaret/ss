#include "nw4r/snd/Task.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_Task.cpp
 */

/*******************************************************************************
 * headers
 */

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

// Interesting
Task::~Task()
{
	NW4RAssert_Line(24, ! mBusyFlag);
}

}}} // namespace nw4r::snd::detail
