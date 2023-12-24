#ifndef NW4R_EF_DRAW_SMOOTH_STRIPE_STRATEGY_H
#define NW4R_EF_DRAW_SMOOTH_STRIPE_STRATEGY_H
#include "types_nw4r.h"
#include "ef_drawstrategyimpl.h"

namespace nw4r
{
    namespace ef
    {
        class DrawSmoothStripeStrategy : public DrawStrategyImpl
        {
        public:
            DrawSmoothStripeStrategy();
            virtual ~DrawSmoothStripeStrategy() {} // at 0x8
            virtual UNKTYPE GetCalcAheadFunc(ParticleManager *); // at 0x18
            
            UNKTYPE Draw(const DrawInfo &, ParticleManager *);
        };
    }
}

#endif