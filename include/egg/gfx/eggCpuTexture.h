#ifndef EGG_CPU_TEXTURE_H
#define EGG_CPU_TEXTURE_H

#include "rvl/GX/GXTexture.h"
namespace EGG {

class CpuTexture {

public:
    void GetTexObj(GXTexObj *) const;
};

} // namespace EGG

#endif
