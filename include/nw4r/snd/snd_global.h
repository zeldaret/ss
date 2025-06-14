#ifndef NW4R_SND_GLOBAL_H
#define NW4R_SND_GLOBAL_H

/*******************************************************************************
 * headers
 */

#include "common.h" // f32

/*******************************************************************************
 * types
 */

#if defined(__cplusplus)

// Macro for for loops using NW4R iterators
# define NW4R_RANGE_FOR(it_, list_)	\
	for (decltype((list_).GetBeginIter()) (it_) = (list_).GetBeginIter(); (it_) != (list_).GetEndIter(); ++(it_))

/* This macro specifically is for the for loops which declare an
 * Iterator currIt = it++; in the body, so that it does not get incremented
 * twice.
 */
# define NW4R_RANGE_FOR_NO_AUTO_INC(it_, list_)	\
	for (decltype((list_).GetBeginIter()) (it_) = (list_).GetBeginIter(); (it_) != (list_).GetEndIter();)

#endif // defined(__cplusplus)

namespace nw4r { namespace snd
{
	enum OutputLineFlag {
		OUTPUT_LINE_MAIN = 0x1,
		OUTPUT_LINE_REMOTE_1 = 0x2,
		OUTPUT_LINE_REMOTE_2 = 0x4,
		OUTPUT_LINE_REMOTE_3 = 0x8,
		OUTPUT_LINE_REMOTE_4 = 0x16,
	};

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x26bd0
	enum AuxBus
	{
		AUX_A,
		AUX_B,
		AUX_C,
	};

	// TODO: would this fit as AX_MAX_AUX in AX?
	static int const AUX_BUS_NUM = 3;

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x20f19
	enum OutputMode
	{
		OUTPUT_MODE_STEREO,
		OUTPUT_MODE_SURROUND,
		OUTPUT_MODE_DPL2,
		OUTPUT_MODE_MONO,
	};

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x25ec5
	enum PanMode
	{
		PAN_MODE_DUAL,
		PAN_MODE_BALANCE,
	};

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x25f0e
	enum PanCurve
	{
		PAN_CURVE_SQRT,
		PAN_CURVE_SQRT_0DB,
		PAN_CURVE_SQRT_0DB_CLAMP,

		PAN_CURVE_SINCOS,
		PAN_CURVE_SINCOS_0DB,
		PAN_CURVE_SINCOS_0DB_CLAMP,

		PAN_CURVE_LINEAR,
		PAN_CURVE_LINEAR_0DB,
		PAN_CURVE_LINEAR_0DB_CLAMP,
	};

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x20e89
	enum SampleFormat
	{
		SAMPLE_FORMAT_PCM_S32,
		SAMPLE_FORMAT_PCM_S16,
		SAMPLE_FORMAT_PCM_S8,
		SAMPLE_FORMAT_DSP_ADPCM,
	};

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x273ea
	struct VoiceOutParam
	{
	// methods
	public:
		// cdtors
		VoiceOutParam() :
			volume		(1.0f),
			pitch		(1.0f),
			pan			(0.0f),
			surroundPan	(0.0f),
			fxSend		(0.0f),
			lpf			(0.0f)
		{
		}

	// members
	public:
		f32	volume;			// size 0x04, offset 0x00
		f32	pitch;			// size 0x04, offset 0x04
		f32	pan;			// size 0x04, offset 0x08
		f32	surroundPan;	// size 0x04, offset 0x0c
		f32	fxSend;			// size 0x04, offset 0x10
		f32	lpf;			// size 0x04, offset 0x14
	}; // size 0x18
}} // namespace nw4r::snd

#endif // NW4R_SND_GLOBAL_H
