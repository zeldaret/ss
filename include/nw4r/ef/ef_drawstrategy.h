#ifndef NW4R_EF_DRAWSTRATEGY_H
#define NW4R_EF_DRAWSTRATEGY_H
#include "common.h"

namespace nw4r {
namespace ef {
struct DrawStrategy {
    inline virtual ~DrawStrategy() {}
    virtual UNKTYPE Draw(const DrawInfo &, ParticleManager *) = 0;
};
} // namespace ef
} // namespace nw4r

#endif
