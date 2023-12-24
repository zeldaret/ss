#ifndef NW4R_LYT_MATERIAL_H
#define NW4R_LYT_MATERIAL_H
#include "lyt_animation.h"
#include "ut_LinkList.h"
#include <rvl/GX.h>


namespace nw4r {
namespace lyt {
// TO-DO
struct Material {
    ut::LinkList<AnimationLink, 0> mAnimLinks; // at 0x4
    // . . .
    GXColorS10 COLOR_0x10;
    GXColorS10 COLOR_0x18;
    GXColorS10 COLOR_0x20;
    // . . .
    UNKWORD WORD_0x40;
    // . . .
    u8 BYTE_0x59;
    // . . .
};
} // namespace lyt
} // namespace nw4r

#endif
