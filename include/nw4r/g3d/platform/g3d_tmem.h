#ifndef NW4R_G3D_PLATFORM_TMEM_H
#define NW4R_G3D_PLATFORM_TMEM_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace g3d {
namespace tmem {

// TODO: Naming
enum TMemLayout {
    TMEM_LAYOUT_NONE,
    TMEM_LAYOUT_1,
    TMEM_LAYOUT_2,
    TMEM_LAYOUT_3
};

void SetTMemLayout(TMemLayout layout);

} // namespace tmem
} // namespace g3d
} // namespace nw4r

#endif
