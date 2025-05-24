#ifndef NW4R_SND_BIQUAD_FILTER_PRESET_H
#define NW4R_SND_BIQUAD_FILTER_PRESET_H

/*******************************************************************************
 * headers
 */

#include "common.h" // f32

#include "nw4r/snd/snd_BiquadFilterCallback.h"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2a5425
	class BiquadFilterLpf : public BiquadFilterCallback
	{
	// methods
	public:
		// virtual function ordering
		// vtable BiquadFilterCallback
		virtual void GetCoef(int type, f32 value, BiquadCoef *coef) const;

	// static members
	private:
		/* technically it seems that COEF_TABLE_SIZE should be defined in terms
		 * of ARRAY_LENGTH(coefTable) (see the order of the typedef tags), but
		 * that seems needlessly more complicated, and I like putting static
		 * constexpr members first
		 */
		static int const COEF_TABLE_SIZE = 112;

		static BiquadCoef const coefTable[COEF_TABLE_SIZE];

	// members
	private:
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2a5544
	class BiquadFilterHpf : public BiquadFilterCallback
	{
	// methods
	public:
		// virtual function ordering
		// vtable BiquadFilterCallback
		virtual void GetCoef(int type, f32 value, BiquadCoef *coef) const;

	// static members
	private:
		static int const COEF_TABLE_SIZE = 97;

		static BiquadCoef const coefTable[COEF_TABLE_SIZE];

	// members
	private:
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2a5663
	class BiquadFilterBpf512 : public BiquadFilterCallback
	{
	// methods
	public:
		// virtual function ordering
		// vtable BiquadFilterCallback
		virtual void GetCoef(int type, f32 value, BiquadCoef *coef) const;

	// static members
	private:
		static int const COEF_TABLE_SIZE = 122;

		static BiquadCoef const coefTable[COEF_TABLE_SIZE];

	// members
	private:
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2a578b
	class BiquadFilterBpf1024 : public BiquadFilterCallback
	{
	// methods
	public:
		// virtual function ordering
		// vtable BiquadFilterCallback
		virtual void GetCoef(int type, f32 value, BiquadCoef *coef) const;

	// static members
	private:
		static int const COEF_TABLE_SIZE = 93;

		static BiquadCoef const coefTable[COEF_TABLE_SIZE];

	// members
	private:
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2a58b6
	class BiquadFilterBpf2048 : public BiquadFilterCallback
	{
	// methods
	public:
		// virtual function ordering
		// vtable BiquadFilterCallback
		virtual void GetCoef(int type, f32 value, BiquadCoef *coef) const;

	// static members
	private:
		static int const COEF_TABLE_SIZE = 93;

		static BiquadCoef const coefTable[COEF_TABLE_SIZE];

	// members
	private:
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_BIQUAD_FILTER_PRESET_H
