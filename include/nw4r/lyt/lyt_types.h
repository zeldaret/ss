#ifndef NW4R_LYT_TYPES_H
#define NW4R_LYT_TYPES_H

#include <nw4r/ut/ut_algorithm.h>

namespace nw4r {
namespace lyt {
namespace detail {
template <typename T>
inline bool TestBit(T bits, int index) {
    T mask = 1 << index;
    return bits & mask;
}
} // namespace detail

} // namespace lyt

} // namespace nw4r

#endif
