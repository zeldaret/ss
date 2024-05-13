#ifndef NW4R_LYT_LAYOUT_H
#define NW4R_LYT_LAYOUT_H
#include "common.h"
#include <new.h>
#include <rvl/MEM/mem_allocator.h>

namespace nw4r {
namespace lyt {
struct Layout {
    // TO-DO: Class members

    static void FreeMemory(void *p) {
        MEMFreeToAllocator(mspAllocator, p);
    }

    static void *AllocMemory(size_t n) {
        return MEMAllocFromAllocator(mspAllocator, n);
    }

    template <typename T>
    static void DeleteArray(T *p, size_t n) {
        if (p) {
            for (size_t i = 0; i < n; i++) {
                p[i].~T();
            }

            FreeMemory(p);
        }
    }

    template <typename T>
    static T *NewArray(size_t n) {
        T *array = (T *)AllocMemory(n * sizeof(T));

        for (size_t i = 0; i < n; i++) {
            new (&array[i]) T();
        }

        return array;
    }

    template <typename T>
    static void DeleteObj(T *t) {
        t->~T();
        FreeMemory(t);
    }

    template <typename T>
    static T *NewObj() {
        T *obj = (T *)AllocMemory(sizeof(T));
        return new (obj) T();
    }

    static MEMAllocator *mspAllocator;
};
} // namespace lyt
} // namespace nw4r

#endif
