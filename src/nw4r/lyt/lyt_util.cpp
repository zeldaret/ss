
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_utils.h>
#include <nw4r/math/math_geometry.h>
#include <nw4r/math/math_types.h>
#include <nw4r/ut/ut_Rect.h>

namespace nw4r {
namespace lyt {

namespace {
// Contains__Q34nw4r3lyt22@unnamed@lyt_util_cpp@FRCQ34nw4r2ut4RectRCQ34nw4r4math4VEC2
bool Contains(const ut::Rect &rect, const math::VEC2 &point) {
    return (rect.left <= point.x && point.x <= rect.right) && (rect.bottom <= point.y && point.y <= rect.top);
}

} // namespace

// BindAnimation__Q24nw4r3lytFPQ34nw4r3lyt5GroupPQ34nw4r3lyt13AnimTransformbb
void BindAnimation(Group *pGroup, AnimTransform *pAnimTrans, bool bRecursive, bool bDisable) {
    ut::LinkList<detail::PaneLink, 0> &paneList = *pGroup->GetPaneList();
    for (ut::LinkList<detail::PaneLink, 0>::Iterator it = paneList.GetBeginIter(); it != paneList.GetEndIter(); it++) {
        it->mTarget->BindAnimation(pAnimTrans, bRecursive, bDisable);
    }
}

// UnbindAnimation__Q24nw4r3lytFPQ34nw4r3lyt5GroupPQ34nw4r3lyt13AnimTransformb
void UnbindAnimation(Group *pGroup, AnimTransform *pAnimTrans, bool bRecursive) {
    ut::LinkList<detail::PaneLink, 0> &paneList = *pGroup->GetPaneList();
    for (ut::LinkList<detail::PaneLink, 0>::Iterator it = paneList.GetBeginIter(); it != paneList.GetEndIter(); it++) {
        it->mTarget->UnbindAnimation(pAnimTrans, bRecursive);
    }
}

// SetAnimationEnable__Q24nw4r3lytFPQ34nw4r3lyt5GroupPQ34nw4r3lyt13AnimTransformbb
void SetAnimationEnable(Group *pGroup, nw4r::lyt::AnimTransform *pAnimTrans, bool bEnable, bool bRecursive) {
    ut::LinkList<detail::PaneLink, 0> &paneList = *pGroup->GetPaneList();
    for (ut::LinkList<detail::PaneLink, 0>::Iterator it = paneList.GetBeginIter(); it != paneList.GetEndIter(); it++) {
        it->mTarget->SetAnimationEnable(pAnimTrans, bEnable, bRecursive);
    }
}

// IsContain__Q24nw4r3lytFPQ34nw4r3lyt4PaneRCQ34nw4r4math4VEC2
bool IsContain(Pane *pPane, const math::VEC2 &pos) {
    math::MTX34 invGlbMtx;
    PSMTXInverse(pPane->GetGlobalMtx(), invGlbMtx);
    math::VEC3 lclPos;
    PSMTXMultVec(invGlbMtx, math::VEC3(pos.x, pos.y, 0.0f), lclPos);
    return Contains(pPane->GetPaneRect(), math::VEC2(lclPos.x, lclPos.y));
}

// FindHitPane__Q24nw4r3lytFPQ34nw4r3lyt4PaneRCQ34nw4r4math4VEC2
Pane *FindHitPane(Pane *pPane, const math::VEC2 &pos) {
    // TODO
    return nullptr;
}

// FindHitPane__Q24nw4r3lytFPQ34nw4r3lyt6LayoutRCQ34nw4r4math4VEC2
Pane *FindHitPane(Layout *pLayout, const math::VEC2 &pos) {
    // TODO
    return nullptr;
}

// GetNextPane__Q24nw4r3lytFPQ34nw4r3lyt4Pane
Pane *GetNextPane(Pane *) {
    // TODO
    return nullptr;
}

} // namespace lyt

} // namespace nw4r
