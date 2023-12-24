#ifndef NW4R_EF_RESOURCE
#define NW4R_EF_RESOURCE
#include "types_nw4r.h"

namespace nw4r
{
	namespace ef
	{
		struct Resource
		{
			EmitterResource * _FindEmitter(const char *, EffectProject *) const;
			
			static Resource * GetInstance();
		};
	}
}

#endif