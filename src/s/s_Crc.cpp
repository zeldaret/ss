#include "s/s_Crc.h"

#include "rvl/OS.h" // IWYU pragma: export

namespace sCrc {

u32 calcCRC32(const void *ptr, u32 size) {
    return OSCalcCRC32(ptr, size);
}

} // namespace sCrc
