#ifndef NW4R_EF_DRAWORDER
#define NW4R_EF_DRAWORDER
#include "types_nw4r.h"
#include "ef_particlemanager.h"
#include "ef_draworderbase.h"

namespace nw4r
{
	namespace ef
	{
		struct DrawOrder : DrawOrderBase
		{
			void Draw(Effect *, const DrawInfo &);
			void Add(Effect *, ParticleManager *);
			void Remove(Effect *, ParticleManager *);
		};
	}
}

#endif