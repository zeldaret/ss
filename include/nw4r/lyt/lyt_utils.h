#ifndef NW4R_LYT_UTIL_H
#define NW4R_LYT_UTIL_H

#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_group.h>

namespace nw4r {
namespace lyt {

void BindAnimation(Group *pGroup, AnimTransform *pAnimTrans, bool bRecursive, bool bDisable);

} // namespace lyt

} // namespace nw4r

#endif
