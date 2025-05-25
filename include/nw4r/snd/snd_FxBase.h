#ifndef NW4R_SND_FX_BASE_H
#define NW4R_SND_FX_BASE_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_global.h"

#include "nw4r/ut/ut_NonCopyable.h" // NonCopyable
#include "nw4r/ut/ut_LinkList.h"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x20f98
	class FxBase : private ut::NonCopyable
	{
	// typedefs
	public:
		typedef ut::LinkList<FxBase, 0x04> LinkList;

	// methods
	public:
		// cdtors
		virtual ~FxBase() {}

		// virtual function ordering
		// vtable FxBase
		virtual bool StartUp();
		virtual void Shutdown();
		virtual void UpdateBuffer(int, void *, u32, SampleFormat, f32,
		                          OutputMode);
		virtual void OnChangeOutputMode();

	private:
		/* base ut::NonCopyable */		// size 0x00, offset 0x00
		/* vtable */					// size 0x04, offset 0x00
	public:
		ut::LinkListNode	mFxLink;	// size 0x08, offset 0x04
	}; // size 0x0c
}} // namespace nw4r::snd

#endif // NW4R_SND_FX_BASE_H
