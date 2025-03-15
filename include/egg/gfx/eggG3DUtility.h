#ifndef EGG_G3D_UTILITY_H
#define EGG_G3D_UTILITY_H

#include "rvl/MEM/mem_allocator.h"

namespace EGG {

class G3DUtility {
public:
    static MEMAllocator *getAllocator() {
        return sAllocator;
    }

private:
    static MEMAllocator *sAllocator;
};

} // namespace EGG

#endif
