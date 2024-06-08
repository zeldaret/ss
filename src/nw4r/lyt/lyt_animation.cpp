#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_group.h>
#include <nw4r/lyt/lyt_pane.h>

namespace nw4r {
namespace lyt {
namespace {

// GetStepCurveValue__27@unnamed@lyt_animation_cpp@FfPCQ44nw4r3lyt3res7StepKeyUl
// GetStepCurveValue__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FfPCQ44nw4r3lyt3res7StepKeyUl
u16 GetStepCurveValue(f32 frame, const res::StepKey *keyArray, u32 keySize) {
    int ikeyL, ikeyR, ikeyCenter;

    const res::StepKey &centerKey = keyArray[0];
}

// RIsSame__27@unnamed@lyt_animation_cpp@Ffff
// RIsSame__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@Ffff
bool RIsSame(f32 a, f32 b, f32 tolerance) {
    f32 c;
}

// GetHermiteCurveValue__27@unnamed@lyt_animation_cpp@FfPCQ44nw4r3lyt3res10HermiteKeyUl
// GetHermiteCurveValue__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FfPCQ44nw4r3lyt3res10HermiteKeyUl
f32 GetHermiteCurveValue(f32 frame, const res::HermiteKey *keyArray, u32 keySize) {
    int ikeyL, ikeyR, ikeyCenter;
    const res::HermiteKey &key0 = keyArray[0];
    const res::HermiteKey &key1 = keyArray[0];
    f32 t1, t2;
    f32 v0, v1;
    f32 s0, s1;
    f32 t1t1t2;
    f32 t1t1t2t2;
    f32 t1t1t1t2t2;
    f32 t1t1t1t2t2t2;
}

// AnimatePainSRT__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimatePaneSRT__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimatePaneSRT(Pane *pPane, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame) {
    int i;
    const res::AnimationTarget *pAnimTarget;
    const res::HermiteKey *keys;
}

// AnimateVisibility__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimateVisibility__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimateVisibility(Pane *pPane, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame) {
    int i;
    const res::AnimationTarget *pAnimTarget;
    const res::StepKey *keys;
}

// AnimateVertexColor__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimateVertexColor__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimateVertexColor(Pane *pPane, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame) {
    int i;
    const res::AnimationTarget *pAnimTarget;
    const res::HermiteKey *keys;
    f32 value;
    u8 u8Val;
}

// AnimateMaterialColor__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimateMaterialColor__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimateMaterialColor(Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets,
        f32 frame) {
    int i;
    const res::AnimationTarget *pAnimTarget;
    const res::HermiteKey *keys;
    f32 value;
    s16 s16Val;
}

// AnimateTextureSRT__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimateTextureSRT__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimateTextureSRT(Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets,
        f32 frame) {
    int i;
    const res::AnimationTarget *pAnimTarget;
    const res::HermiteKey *keys;
}

// AnimateTexturePattern__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlfPPv
// AnimateTexturePattern__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlfPPv
void AnimateTexturePattern(Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets,
        f32 frame, void **tpls) {
    int i;
    const res::AnimationTarget *pAnimTarget;
    const res::StepKey *keys;
    u16 fileIdx;
}

// AnimateIndTexSRT__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimateIndTexSRT__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimateIndTexSRT(Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets,
        f32 frame) {
    int i;
    const res::AnimationTarget *pAnimTarget;
    const res::HermiteKey *keys;
}

// IsBindAnimation__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPQ34nw4r3lyt13AnimTransform
bool IsBindAnimation(Material *pMaterial, AnimTransform *pAnimTrans) {}

// IsBindAnimation__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePQ34nw4r3lyt13AnimTransform
bool IsBindAnimation(Pane *pPane, AnimTransform *pAnimTrans) {}

} // namespace

// __ct__Q34nw4r3lyt13AnimTransformFv
AnimTransform::AnimTransform() : mLink(), mpRes(NULL), mFrame(0.0f) {}

// __dt__Q34nw4r3lyt13AnimTransformFv
AnimTransform::~AnimTransform() {}

// GetFrameSize__Q34nw4r3lyt13AnimTransformCFv
u16 AnimTransform::GetFrameSize() const {}

// IsLoopData__Q34nw4r3lyt13AnimTransformCFv
bool AnimTransform::IsLoopData() const {}

// __ct__Q34nw4r3lyt18AnimTransformBasicFv
AnimTransformBasic::AnimTransformBasic() : AnimTransform() {}

// __dt__Q34nw4r3lyt18AnimTransformBasicFv
AnimTransformBasic::~AnimTransformBasic() {}

// SetResource__Q34nw4r3lyt18AnimTransformBasicFPCQ44nw4r3lyt3res14AnimationBlockPQ34nw4r3lyt16ResourceAccessor
void AnimTransformBasic::SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor) {}

// SetResource__Q34nw4r3lyt18AnimTransformBasicFPCQ44nw4r3lyt3res14AnimationBlockPQ34nw4r3lyt16ResourceAccessorUs
void AnimTransformBasic::SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor, u16 animNum) {
    const u32 *fineNameOffsets;
    int i;
    const char *fileName;
}

// Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt4Paneb
// Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt4Panebb
void AnimTransformBasic::Bind(Pane *pPane, bool bRecursive, bool bDisable) {
    AnimationLink *pCrAnimLink;
    const u32 *animContOffsets;
    u16 i;
    // const res::AnimationContent &animCont;
    Pane *pFindPane;
    Material *pFindMat;
}

// Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt8Material
// Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt8Materialb
void AnimTransformBasic::Bind(Material *pMaterial, bool bDisable) {
    AnimationLink *pCrAnimLink;
    const u32 *animContOffsets;
    u16 i;
    // const res::AnimationContent &animCont;
}

// Animate__Q34nw4r3lyt18AnimTransformBasicFUlPQ34nw4r3lyt4Pane
void AnimTransformBasic::Animate(u32 idx, Pane *pPane) {
    u32 animContOffsets;
    res::AnimationContent *pAnimCont;
    const u32 *animInfoOffsets;
    int i;
    res::AnimationInfo *pAnimInfo;
    const u32 *animTargetOffsets;
}

// Animate__Q34nw4r3lyt18AnimTransformBasicFUlPQ34nw4r3lyt8Material
void AnimTransformBasic::Animate(u32 idx, Material *pMaterial) {
    u32 animContOffsets;
    res::AnimationContent *pAnimCont;
    const u32 *animInfoOffsets;
    int i;
    res::AnimationInfo *pAnimInfo;
    const u32 *animTargetOffsets;
}

// FindUnbindLink__Q34nw4r3lyt18AnimTransformBasicCFPQ34nw4r3lyt13AnimationLink
AnimationLink *AnimTransformBasic::FindUnbindLink(AnimationLink *pLink) {}

// Bind<Q34nw4r3lyt4Pane>__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt4PanePQ34nw4r3lyt13AnimationLinkUsb_PQ34nw4r3lyt13AnimationLink
// Bind<Q34nw4r3lyt8Material>__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt8MaterialPQ34nw4r3lyt13AnimationLinkUsb_PQ34nw4r3lyt13AnimationLink
template <typename T>
AnimationLink *AnimTransformBasic::Bind(T *pTarget, AnimationLink *pAnimLink, u16 idx, bool bDisable) {}

// __ct__Q34nw4r3lyt12AnimResourceFv
AnimResource::AnimResource() {}

// Set__Q34nw4r3lyt12AnimResourceFPCv
void AnimResource::Set(const void *animResBuf) {
    const res::BinaryFileHeader *pFileHeader;
    const res::DataBlockHeader *pDataBlockHead;
    int i;
}

// Init__Q34nw4r3lyt12AnimResourceFv
void AnimResource::Init() {}

// GetGroupNum__Q34nw4r3lyt12AnimResourceCFv
u16 AnimResource::GetGroupNum() const {}

// GetGroupArray__Q34nw4r3lyt12AnimResourceCFv
AnimationGroupRef *AnimResource::GetGroupArray() const {
    const AnimationGroupRef *groups;
}

// IsDescendingBind__Q34nw4r3lyt12AnimResourceCFv
bool AnimResource::IsDescendingBind() const {}

// GetAnimationShareInfoNum__Q34nw4r3lyt12AnimResourceCFv
u16 AnimResource::GetAnimationShareInfoNum() const {}

// GetAnimationShareInfoArray__Q34nw4r3lyt12AnimResourceCFv
AnimationShareInfo *AnimResource::GetAnimationShareInfoArray() const {}

// CalcAnimationNum__Q34nw4r3lyt12AnimResourceCFPQ34nw4r3lyt4Paneb
u16 AnimResource::CalcAnimationNum(Pane *pPane, bool bRecursive) const {
    u16 linkNum;
    const u32 *animContOffsets;
    u16 i;
    // const res::AnimationContent &animCont;
    Pane *pFindPane;
    Material *pFindMat;
}

// CalcAnimationNum__Q34nw4r3lyt12AnimResourceCFPQ34nw4r3lyt5Groupb
u16 AnimResource::CalcAnimationNum(Group *pGroup, bool bRecursive) const {
    u16 linkNum;
    ut::LinkList<detail::PaneLink, 0> paneList;
    ut::LinkList<detail::PaneLink, 0>::Iterator it;
}

namespace detail {

// FindAnimContent__Q44nw4r3lyt6detail12AnimPaneTreeFPCQ44nw4r3lyt3res14AnimationBlockPCcUc
u16 AnimPaneTree::FindAnimContent(const res::AnimationBlock *pAnimBlock, const char *animContName, u8 animContType) {
    const u32 *animContOffsets;
    u16 i;
    const res::AnimationContent *pAnimCont;
}

// Init__Q44nw4r3lyt6detail12AnimPaneTreeFv
void AnimPaneTree::Init() {
    u8 i;
}

// Set__Q44nw4r3lyt6detail12AnimPaneTreeFPQ34nw4r3lyt4PaneRCQ34nw4r3lyt12AnimResource
void AnimPaneTree::Set(Pane *pTargetPane, const AnimResource &animRes) {
    u16 linkNum;
    const res::AnimationBlock *pAnimBlock;
    u16 animContIdx;
    u8 animMatCnt;
    u16 animMatIdxs[9];
    for (u8 i;;) {}
    for (u8 i;;) {}
}

// Bind__Q44nw4r3lyt6detail12AnimPaneTreeCFPQ34nw4r3lyt6LayoutPQ34nw4r3lyt4PanePQ34nw4r3lyt16ResourceAccessor
AnimTransform *AnimPaneTree::Bind(Layout *pLayout, Pane *pTargetPane, ResourceAccessor *pResAccessor) const {
    AnimTransform *pAnimTrans;
    AnimationLink *pCrAnimLink;
    u8 animMatMax;
    u8 i;
    Material *pMaterial;
}

// FindAnimationLink__Q34nw4r3lyt6detailFPQ34nw4r2ut38LinkList<Q34nw4r3lyt13AnimationLink,0>PQ34nw4r3lyt13AnimTransform
AnimationLink *FindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, AnimTransform *pAnimTrans) {
    // it
}

// FindAnimationLink__Q34nw4r3lyt6detailFPQ34nw4r2ut38LinkList<Q34nw4r3lyt13AnimationLink,0>RCQ34nw4r3lyt12AnimResource
AnimationLink *FindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, const AnimResource &animRes) {
    // it
}

// UnbindAnimationLink__Q34nw4r3lyt6detailFPQ34nw4r2ut38LinkList<Q34nw4r3lyt13AnimationLink,0>PQ34nw4r3lyt13AnimTransform
void UnbindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, AnimTransform *pAnimTrans) {
    // it, currIt
}

// UnbindAnimationLink__Q34nw4r3lyt6detailFPQ34nw4r2ut38LinkList<Q34nw4r3lyt13AnimationLink,0>RCQ34nw4r3lyt12AnimResource
void UnbindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, const AnimResource &animRes) {
    // it, currIt
}

} // namespace detail

} // namespace lyt

} // namespace nw4r
