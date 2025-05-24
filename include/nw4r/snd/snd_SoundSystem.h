#ifndef NW4R_SND_SOUND_SYSTEM_H
#define NW4R_SND_SOUND_SYSTEM_H

/*******************************************************************************
 * headers
 */

#include <types.h>

/*******************************************************************************
 * classes and functions
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class TaskThread; }}}

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x4bd66
	class SoundSystem
	{
	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x4bc73
		struct SoundSystemParam
		{
		// methods
		public:
			// cdtors
			SoundSystemParam() :
				soundThreadPriority		(DEFAULT_SOUND_THREAD_PRIORITY),
				soundThreadStackSize	(DEFAULT_SOUND_THREAD_STACK_SIZE),
				dvdThreadPriority		(DEFAULT_DVD_THREAD_PRIORITY),
				dvdThreadStackSize		(DEFAULT_DVD_THREAD_STACK_SIZE)
			{
			}

		// members
		public:
			s32	soundThreadPriority;	// size 0x04, offset 0x00
			u32	soundThreadStackSize;	// size 0x04, offset 0x04
			s32	dvdThreadPriority;		// size 0x04, offset 0x08
			u32	dvdThreadStackSize;		// size 0x04, offset 0x0c
		}; // size 0x10

	// static members
	private:
		static int const DEFAULT_DVD_THREAD_STACK_SIZE = 0x4000;
		static int const DEFAULT_SOUND_THREAD_STACK_SIZE = 0x4000;
		static int const DEFAULT_DVD_THREAD_PRIORITY = 3;
		static int const DEFAULT_SOUND_THREAD_PRIORITY = 4;

		static int sMaxVoices;
		static detail::TaskThread sTaskThread;

	// methods
	public:
		static void InitSoundSystem(SoundSystemParam const &param,
		                            void *workMem, u32 workMemSize);
		static void ShutdownSoundSystem();

		static bool IsInitializedSoundSystem();

		static u32 GetRequiredMemSize(SoundSystemParam const &param);
	}; // size 0x01 (0x00 for inheritance)
}} // namespace nw4r::snd

#endif // NW4R_SND_SOUND_SYSTEM_H
