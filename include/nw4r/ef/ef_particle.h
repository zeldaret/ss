#ifndef NW4R_EF_PARTICLE_H
#define NW4R_EF_PARTICLE_H
#include "types_nw4r.h"
#include "ef_ref.h"

namespace nw4r
{
	namespace ef
	{
		struct Particle : ReferencedObject
		{
			char UNK_0x20[0x8C];
			
			math::VEC3 VEC_0xAC;
			math::VEC3 VEC_0xB8;
			
			char UNK_0xC4[0x4];
			
			ParticleManager * mManager; // at 0xc8
		};
	}
}

#endif