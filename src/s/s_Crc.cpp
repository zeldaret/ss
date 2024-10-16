#include "rvl/OS.h" // IWYU pragma: export

#include <s/s_Crc.h>


namespace sCrc {

u32 calcCRC(const void *ptr, u32 size) {
    return OSCalcCRC32(ptr, size);
}

} // namespace sCrc
