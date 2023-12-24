#ifndef NW4R_SND_DISPOSE_CALLBACK_H
#define NW4R_SND_DISPOSE_CALLBACK_H
#include "types_nw4r.h"
#include "ut_LinkList.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct DisposeCallback
			{
				ut::LinkListNode mNode; // at 0x0
				
				inline virtual ~DisposeCallback() {} // at 0x8
				virtual void InvalidateData(const void *, const void *) = 0; // at 0xc
				virtual void InvalidateWaveData(const void *, const void *) = 0; // at 0x10
			};
		}
	}
}

#endif