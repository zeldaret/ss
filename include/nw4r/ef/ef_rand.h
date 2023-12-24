#ifndef NW4R_EF_RAND_H
#define NW4R_EF_RAND_H
#include "types_nw4r.h"

namespace nw4r
{
	namespace ef
	{
		struct Random
		{
			u32 mSeed; // at 0x0
			
			inline u32 MixRandomSeed()
			{
				return mSeed = mSeed * 0x343fd + 0x269ec3;
			}
			
			inline void Srand(u32 seed)
			{
				mSeed = seed;
			}
			
			inline u32 Rand()
			{
				//WARNING: THIS INLINE FUNCTION MAY NOT BE ACCURATE
				return MixRandomSeed() >> 0x10;
			}
			
			inline float RandFloat()
			{
				//WARNING: THIS INLINE FUNCTION MAY NOT BE ACCURATE
				return (float)Rand() / 0xFFFF;
			}
		};
	}
}

#endif