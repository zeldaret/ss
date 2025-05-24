#ifndef NW4R_SND_LFO_H
#define NW4R_SND_LFO_H

/*******************************************************************************
 * headers
 */

#include <types.h>

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x28dc2
	struct LfoParam
	{
	// methods
	public:
		// cdtors
		LfoParam() { Init(); }

		// methods
		void Init();

	// members
	public:
		f32		depth;		// size 0x04, offset 0x00
		f32		speed;		// size 0x04, offset 0x04
		u32		delay;		// size 0x04, offset 0x08
		u8		range;		// size 0x01, offset 0x0c
		byte1_t	padding[3];
	}; // size 0x10

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x28ea0
	class Lfo
	{
	// methods
	public:
		// cdtors
		Lfo() :
			mDelayCounter	(0),
			mCounter		(0.0f)
		{
		}

		// methods
		void Update(int msec);
		void Reset();

		LfoParam &GetParam() { return mParam; }

		void SetParam(LfoParam const &param) { mParam = param; }

		f32 GetValue() const;

	private:
		static s8 GetSinIdx(int i);

	// static members
	private:
		static int const PERIOD;
		static int const TABLE_SIZE = 32;

	// members
	private:
		LfoParam	mParam;			// size 0x10, offset 0x00
		u32			mDelayCounter;	// size 0x04, offset 0x10
		f32			mCounter;		// size 0x04, offset 0x14
	}; // size 0x18
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_LFO_H
