#ifndef NW4R_UT_NON_COPYABLE_H
#define NW4R_UT_NON_COPYABLE_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace ut {
namespace {

class NonCopyable {
protected:
    NonCopyable() {}
    NonCopyable(const NonCopyable&) {}
    ~NonCopyable() {}
};

} // namespace
} // namespace ut
} // namespace nw4r

#endif