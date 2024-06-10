#ifndef NW4R_LYT_UTIL_H
#define NW4R_LYT_UTIL_H

#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_group.h>

namespace nw4r {
namespace lyt {

void BindAnimation(Group *pGroup, AnimTransform *pAnimTrans, bool bRecursive, bool bDisable);
void UnbindAnimation(Group *pGroup, AnimTransform *pAnimTrans, bool bRecusive);
void SetAnimationEnable(Group *pGroup, AnimTransform *pAnimTrans, bool bEnable, bool bRecursive);
bool IsContain(Pane *pPane, const math::VEC2 &pos);
Pane *FindHitPane(Pane *pPane, const math::VEC2 &pos);
Pane *FindHitPane(Layout *pLayout, const math::VEC2 &pos);
Pane *GetNextPane(Pane *);

} // namespace lyt

} // namespace nw4r

#endif
