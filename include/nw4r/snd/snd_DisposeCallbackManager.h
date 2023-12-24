#ifndef NW4R_SND_DISPOSE_CALLBACK_MANAGER_H
#define NW4R_SND_DISPOSE_CALLBACK_MANAGER_H
#include "types_nw4r.h"
#include "snd_DisposeCallback.h"
#include "ut_LinkList.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct DisposeCallbackManager
			{
				static DisposeCallbackManager * GetInstance();
				
				ut::LinkList<DisposeCallback, 0x0> mList; // at 0x0
				
				UNKTYPE RegisterDisposeCallback(DisposeCallback *);
				UNKTYPE UnregisterDisposeCallback(DisposeCallback *);
				
				UNKTYPE Dispose(void *, u32, void *);
				UNKTYPE DisposeWave(void *, u32, void *);
			};
		}
	}
}

#endif