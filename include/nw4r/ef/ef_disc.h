#ifndef NW4R_EF_DISC_H
#define NW4R_EF_DISC_H
#include "ef_emitterform.h"

namespace nw4r
{
	namespace ef
	{
		struct EmitterFormDisc : EmitterForm
		{
			virtual UNKTYPE Emission(Emitter *, ParticleManager *, int, u32, float *, u16, float, const math::MTX34 *);
		};
	}
}

#endif