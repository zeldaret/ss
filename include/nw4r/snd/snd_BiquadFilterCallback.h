#ifndef NW4R_SND_BIQUAD_FILTER_CALLBACK_H
#define NW4R_SND_BIQUAD_FILTER_CALLBACK_H

/*******************************************************************************
 * headers
 */

#include <types.h>

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x4b4e3
	class BiquadFilterCallback
	{
	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x4b416
		struct BiquadCoef
		{
			u16	b0;	// size 0x02, offset 0x00
			u16	b1;	// size 0x02, offset 0x02
			u16	b2;	// size 0x02, offset 0x04
			u16	a1;	// size 0x02, offset 0x06
			u16	a2;	// size 0x02, offset 0x08
		}; // size 0x0a

	// methods
	public:
		// cdtors
		virtual ~BiquadFilterCallback() {}

		// virtual function ordering
		// vtable BiquadFilterCallback
		virtual void GetCoef(int type, f32 value, BiquadCoef *coef) const = 0;

	// members
	public:
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04
}} // namespace nw4r::snd

#endif // NW4R_SND_BIQUAD_FILTER_CALLBACK_H
