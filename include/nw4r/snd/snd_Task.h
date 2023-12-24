#ifndef NW4R_SND_TASK_H
#define NW4R_SND_TASK_H
#include "types_nw4r.h"
#include "ut_LinkList.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct Task
			{
				virtual ~Task(); // at 0x8
				virtual void Execute() = 0; // at 0xc
				virtual void Cancel() = 0; // at 0x10
				virtual void OnCancel() = 0; // at 0x14
				
				ut::LinkListNode mNode; // at 0x4
				char BYTE_0xC;
				
				inline Task() : BYTE_0xC() {}
			};
		}
	}
}

#endif