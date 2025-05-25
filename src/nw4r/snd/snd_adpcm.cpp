#include "nw4r/snd/snd_adpcm.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_adpcm.cpp
 */

/*******************************************************************************
 * headers
 */

#include <climits>

#include "common.h"

#include <rvl/AX/AXVPB.h> // AXPBADPCM

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

s16 DecodeDspAdpcm(AXPBADPCM *adpcm, byte_t bits)
{
	s16 yn1 = static_cast<s16>(adpcm->yn1);
	s16 yn2 = static_cast<s16>(adpcm->yn2);

	s16 scale = 1 << (adpcm->pred_scale & 0x0f);
	s16 bits2 = bits << 12;
	u16 index = adpcm->pred_scale >> 4;

	s16 coef0 = adpcm->a[index][0];
	s16 coef1 = adpcm->a[index][1];

	s32 sample = coef0 * yn1;
	sample += coef1 * yn2;
	sample += scale * (bits2 >> 1);
	sample >>= 10;
	sample += 1;
	sample >>= 1;

	if (sample > SHRT_MAX)
		sample = SHRT_MAX;
	else if (sample < SHRT_MIN)
		sample = SHRT_MIN;

	adpcm->yn2 = adpcm->yn1;
	adpcm->yn1 = sample;

	return sample;
}

}}} // namespace nw4r::snd::detail
