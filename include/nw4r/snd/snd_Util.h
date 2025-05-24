#ifndef NW4R_SND_UTIL_H
#define NW4R_SND_UTIL_H

/*******************************************************************************
 * headers
 */

#include <macros.h> // static_assert
#include <types.h>

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2a4bc2
	class Util
	{
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2a4932
		enum RefType
		{
			REFTYPE_ADDRESS,
			REFTYPE_OFFSET,
		};

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2a497b
		enum PanCurve
		{
			PAN_CURVE_SQRT,
			PAN_CURVE_SINCOS,
			PAN_CURVE_LINEAR,
		};

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x241fd, 0x242b9...
		template <typename, typename = void, typename = void, typename = void>
		struct DataRef
		{
			u8		refType;	// size 0x01, offset 0x00
			u8		dataType;	// size 0x01, offset 0x01
			byte2_t	reserved;
			u32		value;		// size 0x04, offset 0x04
		}; // size 0x08

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x247c7, 0x249c5...
		template <typename T>
		struct Table
		{
			u32	count;		// size 0x04, offset 0x00
			T	item[1];	// size T,    offset 0x04
		}; // size 0x04 + T

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2a49db
		struct PanInfo
		{
			PanCurve	curve;			// size 0x04, offset 0x00
			bool		centerZeroFlag;	// size 0x01, offset 0x04
			bool		zeroClampFlag;	// size 0x01, offset 0x05
			/* 2 bytes padding */
		}; // size 0x08

		static f32 CalcPitchRatio(int pitch);
		static f32 CalcVolumeRatio(f32 dB);
		static f32 CalcPanRatio(f32 pan, PanInfo const &info);
		static f32 CalcSurroundPanRatio(f32 pan, PanInfo const &info);

		static u16 CalcLpfFreq(f32 scale);

		static void GetRemoteFilterCoefs(int filter, u16 *b0, u16 *b1, u16 *b2,
		                                 u16 *a1, u16 *a2);

		static u16 CalcRandom();

		static void const *GetDataRefAddressImpl(RefType refType, byte4_t value,
		                                         void const *baseAddress);

// The only way these asserts happen one line after the other is with macros
#define DEF_GET_DATA_REF_ADDRESS_(line_, index_)							\
	template <typename T0, typename T1, typename T2, typename T3>			\
	static inline T ## index_ const *GetDataRefAddress ## index_(			\
		Util::DataRef<T0, T1, T2, T3> const &ref, void const *baseAddress)	\
	{																		\
		/* specifically not the header variant */							\
		NW4RAssert_Line(line_, ref.dataType == index_);						\
																			\
		return static_cast<T ## index_ const *>(GetDataRefAddressImpl(		\
			static_cast<RefType>(ref.refType), ref.value, baseAddress));	\
	}																		\
	static_assert(true, "") // swallow semicolon

		DEF_GET_DATA_REF_ADDRESS_(141, 0);
		DEF_GET_DATA_REF_ADDRESS_(142, 1);
		DEF_GET_DATA_REF_ADDRESS_(143, 2);
		DEF_GET_DATA_REF_ADDRESS_(144, 3);

#undef DEF_GET_DATA_REF_ADDRESS_

		static inline byte2_t ReadBigEndian(byte2_t x)
		{
			return x;
		}

		static inline byte4_t ReadBigEndian(byte4_t x)
		{
			return x;
		}

		static f32 const CALC_LPF_FREQ_INTERCEPT;
		static int const CALC_LPF_FREQ_TABLE_SIZE = 3 * 8;
		static int const BIQUAD_COEF_COUNT = 5;
		static int const IIR_COEF_COUNT;
		static int const COEF_TABLE_SIZE = 128;
		static int const COEF_TABLE_MAX = 127;
		static int const COEF_TABLE_MIN = 0;
		static int const PAN_TABLE_SIZE = 256 + 1;
		static int const PAN_TABLE_CENTER = 128;
		static int const PAN_TABLE_MIN = 0;
		static int const PAN_TABLE_MAX = 256;
		static int const DECIBEL_SQUARE_TABLE_SIZE;
		static int const DECIBEL_TABLE_SIZE = 964 + 1;
		static int const VOLUME_TABLE_SIZE;
		static int const OCTAVE_DIVISION = 12; // ? makes sense to me
		static int const PAN_CURVE_NUM = 3;
		static int const CALC_DECIBEL_SCALE_MAX;
		static int const PITCH_DIVISION_RANGE = 256; // ? maybe makes sense?
		static int const PITCH_DIVISION_BIT;
		static int const PAN_CENTER;
		static int const PAN_MAX;
		static int const PAN_MIN;
		static int const VOLUME_DB_MAX = 60;
		static int const VOLUME_DB_MIN = -904;
		static u16 const VOLUME_MAX;
		static u16 const VOLUME_MIN;

		static u16 const CalcLpfFreqTable[CALC_LPF_FREQ_TABLE_SIZE];
		static u16 const RemoteFilterCoefTable[COEF_TABLE_SIZE][BIQUAD_COEF_COUNT];
		static f32 const Pan2RatioTableLinear[PAN_TABLE_SIZE];
		static f32 const Pan2RatioTableSinCos[PAN_TABLE_SIZE];
		static f32 const Pan2RatioTableSqrt[PAN_TABLE_SIZE];
		static f32 const *PanTableTable[PAN_CURVE_NUM];
		static f32 const Decibel2RatioTable[DECIBEL_TABLE_SIZE];
		static f32 const PitchTable[PITCH_DIVISION_RANGE];
		static f32 const NoteTable[OCTAVE_DIVISION];
	}; // "namespace" Util
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_UTIL_H
