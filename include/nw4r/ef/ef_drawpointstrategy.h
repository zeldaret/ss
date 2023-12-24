#ifndef NW4R_EF_DRAW_POINT_STRATEGY_H
#define NW4R_EF_DRAW_POINT_STRATEGY_H
#include "types_nw4r.h"
#include "ef_drawstrategyimpl.h"

namespace nw4r
{
    namespace ef
    {
        class DrawPointStrategy : public DrawStrategyImpl
        {
        public:
            DrawPointStrategy();
            virtual ~DrawPointStrategy() {} // at 0x8
            
            void Draw(const DrawInfo&, ParticleManager *);
			void InitGraphics(const DrawInfo&, ParticleManager *);
        };
    }
}

#endif