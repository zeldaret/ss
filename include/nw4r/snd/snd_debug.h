#ifndef NW4R_SND_DEBUG_H
#define NW4R_SND_DEBUG_H

/*******************************************************************************
 * types
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2bd7b3
	enum DebugWarningFlag
	{
		DEBUG_WARNING_NOT_ENOUGH_INSTANCE,
		DEBUG_WARNING_NOT_ENOUGH_SEQSOUND,
		DEBUG_WARNING_NOT_ENOUGH_STRMSOUND,
		DEBUG_WARNING_NOT_ENOUGH_WAVESOUND,
		DEBUG_WARNING_NOT_ENOUGH_SEQTRACK,
		DEBUG_WARNING_NOT_ENOUGH_STRMCHANNEL,
	};

	namespace detail
	{
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2ec85
		enum DebugSoundType
		{
			DEBUG_SOUND_TYPE_SEQSOUND,
			DEBUG_SOUND_TYPE_STRMSOUND,
			DEBUG_SOUND_TYPE_WAVESOUND,
		};
	} // namespace detail
}} // namespace nw4r::snd

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	bool Debug_GetWarningFlag(DebugWarningFlag warning);
	DebugWarningFlag Debug_GetDebugWarningFlagFromSoundType(
		DebugSoundType type);
	char const *Debug_GetSoundTypeString(DebugSoundType type);
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_DEBUG_H
