#include "nw4r/snd/snd_debug.h"

/*******************************************************************************
 * headers
 */

#include "common.h" // u32

#include <nw4r/NW4RAssert.hpp>

/*******************************************************************************
 * types
 */

namespace nw4r { namespace snd { namespace
{
	/* I just made all of this up, but this is a convenient place to store these
	 * constants
	 */
	enum DebugWarningBitFlag
	{
		DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_SEQSOUND		= 1 << 0,
		DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_STRMSOUND		= 1 << 1,
		DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_WAVESOUND		= 1 << 2,
		DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_SEQTRACK		= 1 << 3,
		DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_STRMCHANNEL	= 1 << 4,

		DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_INSTANCE =
			DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_SEQSOUND
			| DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_STRMSOUND
			| DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_WAVESOUND
			| DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_SEQTRACK
			| DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_STRMCHANNEL,
	};
}}} // namespace nw4r::snd::(unnamed)

/*******************************************************************************
 * local function declarations
 */

namespace nw4r { namespace snd { namespace
{
	u32 GetWarningBitFlag(DebugWarningFlag warning);
}}} // namespace nw4r::snd::(unnamed)

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd { namespace
{
	u32 gWarningFlag = DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_INSTANCE;
}}} // namespace nw4r::snd::(unnamed)

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

namespace detail {

bool Debug_GetWarningFlag(DebugWarningFlag warning)
{
	u32 bitFlag = GetWarningBitFlag(warning);

	return (gWarningFlag & bitFlag) == bitFlag;
}

DebugWarningFlag Debug_GetDebugWarningFlagFromSoundType(DebugSoundType type)
{
	switch (type)
	{
	case DEBUG_SOUND_TYPE_SEQSOUND:
		return DEBUG_WARNING_NOT_ENOUGH_SEQSOUND;

	case DEBUG_SOUND_TYPE_STRMSOUND:
		return DEBUG_WARNING_NOT_ENOUGH_STRMSOUND;

	case DEBUG_SOUND_TYPE_WAVESOUND:
		return DEBUG_WARNING_NOT_ENOUGH_WAVESOUND;

	default:
		NW4RPanic_Line(91);
		return DEBUG_WARNING_NOT_ENOUGH_SEQSOUND;
	}
}

char const *Debug_GetSoundTypeString(DebugSoundType type)
{
	switch (type)
	{
	case DEBUG_SOUND_TYPE_SEQSOUND:
		return "seq";

	case DEBUG_SOUND_TYPE_STRMSOUND:
		return "strm";

	case DEBUG_SOUND_TYPE_WAVESOUND:
		return "wave";

	default:
		NW4RPanic_Line(107);
		return "";
	}
}

} // namespace detail

namespace {

u32 GetWarningBitFlag(DebugWarningFlag warning)
{
	u32 bitFlag = 0;

	switch (warning)
	{
	case DEBUG_WARNING_NOT_ENOUGH_INSTANCE:
		bitFlag = DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_INSTANCE;
		break;

	case DEBUG_WARNING_NOT_ENOUGH_SEQSOUND:
		bitFlag = DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_SEQSOUND;
		break;

	case DEBUG_WARNING_NOT_ENOUGH_STRMSOUND:
		bitFlag = DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_STRMSOUND;
		break;

	case DEBUG_WARNING_NOT_ENOUGH_WAVESOUND:
		bitFlag = DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_WAVESOUND;
		break;

	case DEBUG_WARNING_NOT_ENOUGH_SEQTRACK:
		bitFlag = DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_SEQTRACK;
		break;

	case DEBUG_WARNING_NOT_ENOUGH_STRMCHANNEL:
		bitFlag = DEBUG_WARNING_BIT_FLAG_NOT_ENOUGH_STRMCHANNEL;
		break;
	}

	return bitFlag;
}

} // unnamed namespace

}} // namespace nw4r::snd
