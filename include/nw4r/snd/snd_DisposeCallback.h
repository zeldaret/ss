#ifndef NW4R_SND_DISPOSE_CALLBACK_H
#define NW4R_SND_DISPOSE_CALLBACK_H
#include <nw4r/types_nw4r.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

class DisposeCallback {
public:
    NW4R_UT_LIST_NODE_DECL(); // at 0x0

public:
    virtual ~DisposeCallback() {} // at 0x8

    virtual void InvalidateData(const void* pStart,
                                const void* pEnd) = 0; // at 0xC
    virtual void InvalidateWaveData(const void* pStart,
                                    const void* pEnd) = 0; // at 0x10
};

NW4R_UT_LIST_TYPEDEF_DECL(DisposeCallback);

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
