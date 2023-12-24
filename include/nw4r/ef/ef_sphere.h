#ifndef NW4R_EF_SPHERE_H
#define NW4R_EF_SPHERE_H
#include "ef_emitterform.h"

namespace nw4r
{
	namespace ef
	{
		struct EmitterFormSphere : EmitterForm
		{
			virtual UNKTYPE Emission(Emitter *, ParticleManager *, int, u32, float *, u16, float, const math::MTX34 *);
		};
	}
}

#endif