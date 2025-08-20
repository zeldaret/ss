#ifndef S_CRC_H
#define S_CRC_H

#include "common.h"

namespace sCrc {

u32 calcCRC32(const void *ptr, u32 size);

} // namespace sCrc

#endif
