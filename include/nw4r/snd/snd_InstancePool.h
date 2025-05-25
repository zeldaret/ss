#ifndef NW4R_SND_INSTANCE_POOL_H
#define NW4R_SND_INSTANCE_POOL_H

/*******************************************************************************
 * headers
 */

#include <new>

#include "common.h"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e96b
	class PoolImpl
	{
	// methods
	protected:
		// cdtors
		PoolImpl() : mNext(nullptr) {}

		// methods
		u32 CreateImpl(void *buffer, u32 size, u32 objSize);
		void DestroyImpl(void *buffer, u32 size);

		int CountImpl() const;

		void *AllocImpl();
		void FreeImpl(void *ptr);

	// members
	private:
		PoolImpl	*mNext;	// size 0x04, offset 0x00
	}; // size 0x04

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f735, 0x31491...
	template <class T>
	class InstancePool : private PoolImpl
	{
	// methods
	public:
		// methods
		u32 Create(void *buffer, u32 size)
		{
			u32 objSize = sizeof(T);

			return CreateImpl(buffer, size, objSize);
		}
		void Destroy(void *buffer, u32 size) { DestroyImpl(buffer, size); }

		int Count() const { return CountImpl(); }

		T *Alloc()
		{
			void *ptr = AllocImpl();
			if (!ptr)
				return nullptr;

			return new (ptr) T;
		}
		void Free(T *obj)
		{
			if (obj)
			{
				obj->~T();
				FreeImpl(obj);
			}
		}

	// members
	private:
		/* base PoolImpl */	// size 0x04, offset 0x00
	}; // size 0x04

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2efa0, 0x309c7...
	template <class T>
	class MemoryPool : private PoolImpl
	{
	// methods
	public:
		// methods
		u32 Create(void *buffer, u32 size)
		{
			u32 objSize = sizeof(T);

			return CreateImpl(buffer, size, objSize);
		}
		void Destroy(void *buffer, u32 size) { DestroyImpl(buffer, size); }

		int Count() const { return CountImpl(); } // Presumably

		T *Alloc() { return static_cast<T *>(AllocImpl()); }
		void Free(T *obj) { FreeImpl(obj); }

	// members
	private:
		/* base PoolImpl */	// size 0x04, offset 0x00
	}; // size 0x04
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_INSTANCE_POOL_H
