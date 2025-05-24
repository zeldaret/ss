#ifndef NW4R_SND_AX_VOICE_MANAGER_H
#define NW4R_SND_AX_VOICE_MANAGER_H

/*******************************************************************************
 * headers
 */

#include <types.h> // u32

#include "AxVoice.h"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2adb3b
	class AxVoiceManager
	{
	// methods
	public:
		// instance accessors
		static AxVoiceManager &GetInstance();

		// methods
		void Setup(void *mem, u32 memSize);
		void Shutdown();

		u32 GetRequiredMemSize(int axVoiceCount);

		AxVoice *AcquireAxVoice(u32 priority, AxVoice::Callback *callback,
		                        void *callbackData);
		void ReserveForFreeAxVoice(AxVoice *voice);
		void FreeAxVoice(AxVoice *voice);
		void FreeAllReservedAxVoice();

	private:
		// cdtors
		AxVoiceManager();

		// methods
		AxVoice *Alloc();
		void ReserveForFree(AxVoice *voice);
		void Free(AxVoice *voice);

	// static members
	public:
		static int const VOICE_COUNT_MARGIN = 16;

	// members
	private:
		AxVoice::LinkList	mActiveVoiceList;		// size 0x0c, offset 0x00
		AxVoice::LinkList	mFreeVoiceList;			// size 0x0c, offset 0x0c
		AxVoice::LinkList	mFreeReservedVoiceList;	// size 0x0c, offset 0x18
		bool				mInitialized;			// size 0x01, offset 0x24
		/* 3 bytes padding */
		int					mVoiceCount;			// size 0x04, offset 0x28
	}; // size 0x2c
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_AX_VOICE_MANAGER_H
