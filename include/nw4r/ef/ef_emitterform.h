#ifndef NW4R_EF_EMITTERFORM_H
#define NW4R_EF_EMITTERFORM_H
#include "types_nw4r.h"

namespace nw4r
{
	namespace ef
	{
		struct EmitterForm
		{
			virtual UNKTYPE Emission(Emitter *, ParticleManager *, int, u32, float *, u16, float, const math::MTX34 *) = 0;
			
			UNKTYPE CalcVelocity(math::VEC3 *, Emitter *, const math::VEC3 &, const math::VEC3 &, const math::VEC3 &, const math::VEC3 &) const;
			u16 CalcLife(u16, float, Emitter *);
		};
	}
}

#endif