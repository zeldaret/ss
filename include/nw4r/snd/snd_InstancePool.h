#ifndef NW4R_SND_INSTANCE_POOL_H
#define NW4R_SND_INSTANCE_POOL_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace snd {
namespace detail {

class PoolImpl {
public:
    PoolImpl() : mNext(NULL) {}

protected:
    u32 CreateImpl(void* pBuffer, u32 size, u32 stride);
    void DestroyImpl(void* pBuffer, u32 size);
    int CountImpl() const;

    void* AllocImpl();
    void FreeImpl(void* pElem);

private:
    PoolImpl* mNext; // at 0x0
};

template <typename T> class InstancePool : private PoolImpl {
public:
    u32 Create(void* pBuffer, u32 size) {
        return CreateImpl(pBuffer, size, sizeof(T));
    }

    void Destroy(void* pPtr, u32 size) {
        DestroyImpl(pPtr, size);
    }

    int Count() const {
        return CountImpl();
    }

    T* Alloc() {
        void* pPtr = AllocImpl();
        if (pPtr == NULL) {
            return NULL;
        }

        return new (pPtr) T;
    }

    void Free(T* pElem) {
        if (pElem != NULL) {
            pElem->~T();
            FreeImpl(pElem);
        }
    }
};

template <typename T> class MemoryPool : private PoolImpl {
public:
    u32 Create(void* pBuffer, u32 size) {
        return CreateImpl(pBuffer, size, sizeof(T));
    }

    void Destroy(void* pPtr, u32 size) {
        DestroyImpl(pPtr, size);
    }

    int Count() const {
        return CountImpl();
    }

    T* Alloc() {
        return static_cast<T*>(AllocImpl());
    }

    void Free(T* pElem) {
        FreeImpl(pElem);
    }
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
