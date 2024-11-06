#include "nw4r/snd/snd_adpcm.h"

namespace nw4r {
namespace snd {
namespace detail {

s16 DecodeDspAdpcm(AXPBADPCM* pAdpcm, u8 bits) {
    s16 yn1 = pAdpcm->yn1;
    s16 yn2 = pAdpcm->yn2;

    s16 scale = 1 << (pAdpcm->pred_scale & 0x0F);
    s16 bits2 = bits << 12;
    s16 index = pAdpcm->pred_scale >> 4;

    s16 coef0 = pAdpcm->a[index][0];
    s16 coef1 = pAdpcm->a[index][1];

    s32 sample = coef0 * yn1;
    sample += coef1 * yn2;
    sample += scale * (bits2 >> 1);
    sample >>= 10;
    sample += 1;
    sample >>= 1;

    if (sample > 32767) {
        sample = 32767;
    } else if (sample < -32768) {
        sample = -32768;
    }

    pAdpcm->yn2 = pAdpcm->yn1;
    pAdpcm->yn1 = sample;
    return sample;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
