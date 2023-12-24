#ifndef NW4R_SND_INSTANCE_POOL_H
#define NW4R_SND_INSTANCE_POOL_H
#include "types_nw4r.h"
#include <MSL_C/new.h>


namespace nw4r {
namespace snd {
namespace detail {
struct PoolImpl {
    struct Member {
        Member *mNext; // at 0x0
    };

    u32 CreateImpl(void *, u32, u32);
    void DestroyImpl(void *, u32);
    u32 CountImpl() const;
    void *AllocImpl();
    void FreeImpl(void *);

    Member mHead; // at 0x0

    inline PoolImpl() {
        mHead.mNext = NULL;
    }
};

template <typename T>
struct InstancePool : PoolImpl {
    inline u32 Create(void *ptr, u32 num) {
        return CreateImpl(ptr, num, sizeof(T));
    }

    inline T *Alloc() {
        void *ptr = AllocImpl();

        if (!ptr) {
            return NULL;
        }

        return new (ptr) T;
    }

    inline void Free(T *ptr) {
        if (ptr) {
            ptr->~T();
            FreeImpl(ptr);
        }
    }

    inline void Destroy(void *ptr, u32 num) {
        DestroyImpl(ptr, num);
    }

    inline u32 Count() const {
        return CountImpl();
    }
};

template <typename T>
struct MemoryPool : PoolImpl {
    inline void Free(T *ptr) {
        ptr->~T();
        FreeImpl(ptr);
    }
};
} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
