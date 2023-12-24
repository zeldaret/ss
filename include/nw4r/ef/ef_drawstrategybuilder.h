#ifndef NW4R_EF_DRAWSTRATEGYBUILDER_H
#define NW4R_EF_DRAWSTRATEGYBUILDER_H
#include "types_nw4r.h"

namespace nw4r
{
	namespace ef
	{
		enum DrawStrategyType
		{
			DrawStratType_Point,
			DrawStratType_Line,
			DrawStratType_Free,
			DrawStratType_Billboard,
			DrawStratType_Directional,
			DrawStratType_Stripe,
			DrawStratType_SmoothStripe
		};
		
		struct DrawStrategyBuilder
		{
			virtual DrawStrategy * Create(u32);
		};
	}
}

#endif