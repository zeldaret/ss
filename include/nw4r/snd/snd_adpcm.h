#ifndef NW4R_SND_ADPCM_H
#define NW4R_SND_ADPCM_H

#include "rvl/AX.h" // IWYU pragma: export

namespace nw4r {
namespace snd {
namespace detail {

s16 DecodeDspAdpcm(AXPBADPCM *pAdpcm, u8 bits);

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
