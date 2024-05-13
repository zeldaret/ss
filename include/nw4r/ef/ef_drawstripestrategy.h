#ifndef NW4R_EF_DRAW_STRIPE_STRATEGY_H
#define NW4R_EF_DRAW_STRIPE_STRATEGY_H
#include "common.h"
#include "ef_drawstrategyimpl.h"

namespace nw4r {
namespace ef {
class DrawStripeStrategy : public DrawStrategyImpl {
public:
    DrawStripeStrategy();
    virtual ~DrawStripeStrategy() {}                     // at 0x8
    virtual UNKTYPE GetCalcAheadFunc(ParticleManager *); // at 0x18

    UNKTYPE Draw(const DrawInfo &, ParticleManager *);
};
} // namespace ef
} // namespace nw4r

#endif
