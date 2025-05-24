#ifndef NW4R_SND_ADPCM_H
#define NW4R_SND_ADPCM_H

/*******************************************************************************
 * headers
 */

#include <types.h>

#if 0
#include <revolution/AX/AXVPB.h> // AXPBADPCM
#else
#include <context_rvl.h>
#endif

/*******************************************************************************
 * types
 */

namespace nw4r { namespace snd { namespace detail
{
	// Why are these just the AX types

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x291b3
	struct AdpcmParam // AXPBADPCM
	{
		u16	coef[8 * 2];	// size 0x20, offset 0x00
		u16	gain;			// size 0x02, offset 0x20
		u16	pred_scale;		// size 0x02, offset 0x22
		u16	yn1;			// size 0x02, offset 0x24
		u16	yn2;			// size 0x02, offset 0x26
	}; // size 0x28

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x29290
	struct AdpcmLoopParam // AXPBADPCMLOOP
	{
		u16	loop_pred_scale;	// size 0x02, offset 0x00
		u16	loop_yn1;			// size 0x02, offset 0x02
		u16	loop_yn2;			// size 0x02, offset 0x04
	}; // size 0x06
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	s16 DecodeDspAdpcm(AXPBADPCM *adpcm, byte_t bits);
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_ADPCM_H
