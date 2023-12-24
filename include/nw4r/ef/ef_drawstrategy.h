#ifndef NW4R_EF_DRAWSTRATEGY_H
#define NW4R_EF_DRAWSTRATEGY_H
#include "types_nw4r.h"

namespace nw4r
{
	namespace ef
	{
		struct DrawStrategy
		{
			inline virtual ~DrawStrategy() {}
			virtual UNKTYPE Draw(const DrawInfo &, ParticleManager *) = 0;
		};
	}
}

#endif