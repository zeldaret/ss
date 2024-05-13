#ifndef NW4R_EF_DRAW_DIRECTIONAL_STRATEGY_H
#define NW4R_EF_DRAW_DIRECTIONAL_STRATEGY_H
#include "common.h"
#include "ef_drawstrategyimpl.h"

namespace nw4r {
namespace ef {
class DrawDirectionalStrategy : public DrawStrategyImpl {
public:
    DrawDirectionalStrategy();
    virtual ~DrawDirectionalStrategy() {}                // at 0x8
    virtual UNKTYPE GetCalcAheadFunc(ParticleManager *); // at 0x18

    void Draw(const DrawInfo &, ParticleManager *);
};
} // namespace ef
} // namespace nw4r

#endif
