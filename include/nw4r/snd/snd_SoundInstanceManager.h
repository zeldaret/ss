#ifndef NW4R_SND_SOUND_INSTANCE_MANAGER_H
#define NW4R_SND_SOUND_INSTANCE_MANAGER_H
#include <OSMutex.h>
#include "types_nw4r.h"
#include "ut_lock.h"
#include "ut_LinkList.h"
#include "snd_InstancePool.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			template <typename T>
			struct SoundInstanceManager
			{
				MemoryPool<T> mPool; // at 0x0
				ut::LinkList<T, 0xB8> mPriorityList; // at 0x4
				OSMutex mMutex; // at 0x10
				
				inline void Free(T * pInstance)
				{
					ut::detail::AutoLock<OSMutex> lock(mMutex);
					
					if (mPriorityList.IsEmpty()) return;
					
					mPriorityList.Erase(pInstance);
					mPool.Free(pInstance);
				}
				
				inline void InsertPriorityList(T * pInstance, int priority)
				{
					ut::LinkList<T, 0xB8>::Iterator iter = mPriorityList.GetBeginIter();
					int curPriority;
					
					while (iter != mPriorityList.GetEndIter())
					{
						curPriority = iter->CalcCurrentPlayerPriority();
						
						if (priority < curPriority) break;
						
						iter++;
					}
					
					mPriorityList.Insert(iter, pInstance);
				}
				
				inline void UpdatePriority(T * pInstance, int priority)
				{
					ut::detail::AutoLock<OSMutex> lock(mMutex);
					
					mPriorityList.Erase(pInstance);
					InsertPriorityList(pInstance, priority);
				}
			};
		}
	}
}

#endif