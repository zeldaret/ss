#ifndef NW4R_EF_REF
#define NW4R_EF_REF
#include "types_nw4r.h"
#include "math_types.h"

namespace nw4r
{
	namespace ef
	{
		struct ReferencedObject
		{
			char UNK_0x0[0xC];
			UNKWORD WORD_0xC;
			u32 mRefCount; // at 0x10
			char UNK_0x14[0x8];
			
			virtual bool SendClosing();
			virtual UNKTYPE DestroyFunc();
			
			inline void Initialize()
			{
				mRefCount = 0;
				WORD_0xC = 1;
			}
			
			inline void Ref()
			{
				mRefCount++;
			}
			
			inline void UnRef()
			{
				if (--mRefCount == 0 && WORD_0xC == 2) SendClosing();
			}
			
			inline void Destroy()
			{
				DestroyFunc();
				WORD_0xC = 2;
				if (mRefCount == 0) SendClosing();
			}
		};
		
		typedef ReferencedObject UNKREF;
	}
}

#endif