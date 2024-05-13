#ifndef NW4R_EF_DRAW_BILLBOARD_STRATEGY_H
#define NW4R_EF_DRAW_BILLBOARD_STRATEGY_H
#include "common.h"
#include "ef_drawstrategyimpl.h"

namespace nw4r {
namespace ef {
class DrawBillboardStrategy : public DrawStrategyImpl {
public:
    DrawBillboardStrategy();
    virtual ~DrawBillboardStrategy() {}                  // at 0x8
    virtual UNKTYPE GetCalcAheadFunc(ParticleManager *); // at 0x18

    void Draw(const DrawInfo &, ParticleManager *);
};
} // namespace ef
} // namespace nw4r

#endif
