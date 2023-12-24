#ifndef NW4R_SND_FRAME_HEAP_H
#define NW4R_SND_FRAME_HEAP_H
#include "types_nw4r.h"
#include "ut_LinkList.h"
#include "ut_algorithm.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct FrameHeap
			{
				typedef void (* AllocCallback)(void *, u32, void *);
				
				struct Block
				{
					ut::LinkListNode mNode; // at 0x0
					u32 mSize; // at 0x8
					AllocCallback mCallback; // at 0xc
					void * PTR_0x10;
					
					inline Block(u32 size, AllocCallback callback, void * ptr)
						:
						mSize(size),
						mCallback(callback),
						PTR_0x10(ptr)
					{}
					
					inline void * GetBufferAddr()
					{
						return reinterpret_cast<u8 *>(this) + ut::RoundUp<u32>(sizeof(Block), 0x20);
					}
					
					inline ~Block()
					{
						if (mCallback) mCallback(PTR_0x10, mSize, GetBufferAddr());
					}
				};
				
				struct Section
				{
					ut::LinkListNode mNode; // at 0x0
					ut::LinkList<Block, 0x0> mList; // at 0x8
					
					inline void AppendBlock(Block * pBlock)
					{
						mList.PushBack(pBlock);
					}
					
					inline ~Section()
					{
						ut::LinkList<Block, 0x0>::Iterator iter = mList.GetEndIter();
						
						while (iter != mList.GetBeginIter())
						{
							(--iter)->~Block();
						}
					}
				};
				
				bool NewSection(); //inlined
				UNKTYPE ClearSection(); //inlined
				
				FrameHeap();
				~FrameHeap();
				
				bool Create(void *, u32);
				UNKTYPE Destroy();
				void Clear();
				void * Alloc(u32, AllocCallback, void *);
				int SaveState();
				void LoadState(int);
				int GetCurrentLevel() const;
				u32 GetFreeSize() const;
				
				UNKTYPE * PTR_0x0;
				ut::LinkList<Section, 0x0> mList; // at 0x4
				
				inline bool IsValid() const
				{
					return mList.GetSize();
				}
			};
		}
	}
}

#endif