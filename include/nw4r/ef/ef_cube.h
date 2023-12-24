#ifndef NW4R_EF_CUBE_H
#define NW4R_EF_CUBE_H
#include "ef_emitterform.h"

namespace nw4r
{
	namespace ef
	{
		struct EmitterFormCube : EmitterForm
		{
			virtual UNKTYPE Emission(Emitter *, ParticleManager *, int, u32, float *, u16, float, const math::MTX34 *);
		};
	}
}

#endif