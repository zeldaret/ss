#ifndef NW4R_EF_CYLINDER_H
#define NW4R_EF_CYLINDER_H
#include "ef_emitterform.h"

namespace nw4r
{
	namespace ef
	{
		struct EmitterFormCylinder : EmitterForm
		{
			virtual UNKTYPE Emission(Emitter *, ParticleManager *, int, u32, float *, u16, float, const math::MTX34 *);
		};
	}
}

#endif