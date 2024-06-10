#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_group.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>

namespace nw4r {
namespace lyt {
namespace {

// RIsSame__27@unnamed@lyt_animation_cpp@Ffff
// RIsSame__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@Ffff
bool RIsSame(f32 a, f32 b, f32 tolerance) {
    f32 c = a - b;
    return (-tolerance < c && c < tolerance);
}

// GetStepCurveValue__27@unnamed@lyt_animation_cpp@FfPCQ44nw4r3lyt3res7StepKeyUl
// GetStepCurveValue__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FfPCQ44nw4r3lyt3res7StepKeyUl
u16 GetStepCurveValue(f32 frame, const res::StepKey *keyArray, u32 keySize) {
    if (keySize == 1 || frame <= keyArray[0].frame) {
        return keyArray[0].value;
    }
    if (frame >= keyArray[keySize - 1].frame) {
        return keyArray[keySize - 1].value;
    }

    int ikeyL = 0;
    int ikeyR = keySize - 1;
    while (ikeyL != ikeyR - 1 && ikeyL != ikeyR) {
        int ikeyCenter = (ikeyL + ikeyR) / 2;
        const res::StepKey &centerKey = keyArray[ikeyCenter];
        if (frame < centerKey.frame) {
            ikeyR = ikeyCenter;
        } else {
            ikeyL = ikeyCenter;
        }
    }

    if (RIsSame(frame, keyArray[ikeyR].frame, 0.001f)) {
        return keyArray[ikeyR].value;
    } else {
        return keyArray[ikeyL].value;
    }
}

// GetHermiteCurveValue__27@unnamed@lyt_animation_cpp@FfPCQ44nw4r3lyt3res10HermiteKeyUl
// GetHermiteCurveValue__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FfPCQ44nw4r3lyt3res10HermiteKeyUl
f32 GetHermiteCurveValue(f32 frame, const res::HermiteKey *keyArray, u32 keySize) {
    if (keySize == 1 || frame <= keyArray[0].frame) {
        return keyArray[0].value;
    }
    if (frame >= keyArray[keySize - 1].frame) {
        return keyArray[keySize - 1].value;
    }

    int ikeyL = 0;
    int ikeyR = keySize - 1;
    while (ikeyL != ikeyR - 1 && ikeyL != ikeyR) {
        int ikeyCenter = (ikeyL + ikeyR) / 2;
        if (frame <= keyArray[ikeyCenter].frame) {
            ikeyR = ikeyCenter;
        } else {
            ikeyL = ikeyCenter;
        }
    }
    const res::HermiteKey &key0 = keyArray[ikeyL];
    const res::HermiteKey &key1 = keyArray[ikeyR];

    if (RIsSame(frame, key1.frame, 0.001f)) {
        if (ikeyR < keySize - 1 && key1.frame == keyArray[ikeyR + 1].frame) {
            return keyArray[ikeyR + 1].value;
        } else {
            return key1.value;
        }
    }

    f32 t1 = frame - key0.frame;
    f32 t2 = 1.0f / (key1.frame - key0.frame);

    f32 v0 = key0.value;
    f32 v1 = key1.value;
    f32 s0 = key0.slope;
    f32 s1 = key1.slope;

    f32 t1t1t2 = t1 * t1 * t2;
    f32 t1t1t2t2 = t1t1t2 * t2;
    f32 t1t1t1t2t2 = t1 * t1t1t2t2;
    f32 t1t1t1t2t2t2 = t1t1t1t2t2 * t2;

    return v0 * (2.0f * t1t1t1t2t2t2 - 3.0f * t1t1t2t2 + 1.0f) + v1 * (-2.0f * t1t1t1t2t2t2 + 3.0f * t1t1t2t2) +
            s0 * (t1 + (t1t1t1t2t2 - 2.0f * t1t1t2)) + s1 * (t1t1t1t2t2 - t1t1t2);
}

// AnimatePainSRT__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimatePaneSRT__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimatePaneSRT(Pane *pPane, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
                detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        const res::HermiteKey *keys = detail::ConvertOffsToPtr<res::HermiteKey>(pAnimTarget, pAnimTarget->keyOffset);
        pPane->SetSRTElement(pAnimTarget->target, GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum));
    }
}

// AnimateVisibility__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimateVisibility__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimateVisibility(Pane *pPane, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
                detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        const res::StepKey *keys = detail::ConvertOffsToPtr<res::StepKey>(pAnimTarget, pAnimTarget->keyOffset);
        pPane->SetVisible(GetStepCurveValue(frame, keys, pAnimTarget->keyNum) != 0);
    }
}

// AnimateVertexColor__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimateVertexColor__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimateVertexColor(Pane *pPane, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets, f32 frame) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
                detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        const res::HermiteKey *keys = detail::ConvertOffsToPtr<res::HermiteKey>(pAnimTarget, pAnimTarget->keyOffset);
        f32 value = GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum);
        value += 0.5f;
        u8 u8Val;
        OSf32tou8(&value, &u8Val);
        pPane->SetColorElement(pAnimTarget->target, u8Val);
    }
}

// AnimateMaterialColor__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimateMaterialColor__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimateMaterialColor(Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets,
        f32 frame) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
                detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        const res::HermiteKey *keys = detail::ConvertOffsToPtr<res::HermiteKey>(pAnimTarget, pAnimTarget->keyOffset);
        f32 value = GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum);
        value += 0.5f;
        s16 s16Val;
        OSf32tos16(&value, &s16Val);
        s16Val = ut::Min<s16>(ut::Max<s16>(s16Val, -0x400), 1023);
        pMaterial->SetColorElement(pAnimTarget->target, s16Val);
    }
}

// AnimateTextureSRT__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimateTextureSRT__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimateTextureSRT(Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets,
        f32 frame) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
                detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        if (pAnimTarget->id < pMaterial->GetTexSRTCap()) {
            const res::HermiteKey *keys =
                    detail::ConvertOffsToPtr<res::HermiteKey>(pAnimTarget, pAnimTarget->keyOffset);

            pMaterial->SetTexSRTElement(pAnimTarget->id, pAnimTarget->target,
                    GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum));
        }
    }
}

// AnimateTexturePattern__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlfPPv
// AnimateTexturePattern__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlfPPv
void AnimateTexturePattern(Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets,
        f32 frame, void **tpls) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
                detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        if (pAnimTarget->id < pMaterial->GetTextureNum()) {
            const res::StepKey *keys = detail::ConvertOffsToPtr<res::StepKey>(pAnimTarget, pAnimTarget->keyOffset);
            u16 fileIdx = GetStepCurveValue(frame, keys, pAnimTarget->keyNum);
            if (tpls[fileIdx] != nullptr) {
                pMaterial->GetTexturePtr(pAnimTarget->id)->ReplaceImage((TPLPalette *)tpls[fileIdx], 0);
            }
        }
    }
}

// AnimateIndTexSRT__27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
// AnimateIndTexSRT__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPCQ44nw4r3lyt3res13AnimationInfoPCUlf
void AnimateIndTexSRT(Material *pMaterial, const res::AnimationInfo *pAnimInfo, const u32 *animTargetOffsets,
        f32 frame) {
    for (int i = 0; i < pAnimInfo->num; i++) {
        const res::AnimationTarget *pAnimTarget =
                detail::ConvertOffsToPtr<res::AnimationTarget>(pAnimInfo, animTargetOffsets[i]);
        if (pAnimTarget->id < pMaterial->GetIndTexSRTCap()) {
            const res::HermiteKey *keys =
                    detail::ConvertOffsToPtr<res::HermiteKey>(pAnimTarget, pAnimTarget->keyOffset);
            pMaterial->SetIndTexSRTElement(pAnimTarget->id, pAnimTarget->target,
                    GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum));
        }
    }
}

// IsBindAnimation__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt8MaterialPQ34nw4r3lyt13AnimTransform
bool IsBindAnimation(Material *pMaterial, AnimTransform *pAnimTrans) {
    return false; // Early return in release builds, no call to FindAnimationLink in SS, exists in tfp2

    if (pMaterial->FindAnimationLink(pAnimTrans)) {
        return true;
    }
    return false;
}

// IsBindAnimation__Q34nw4r3lyt27@unnamed@lyt_animation_cpp@FPQ34nw4r3lyt4PanePQ34nw4r3lyt13AnimTransform
bool IsBindAnimation(Pane *pPane, AnimTransform *pAnimTrans) {
    return false; // Early return in release builds, no call to FindAnimationLink in SS, exists in tfp2

    if (pPane->FindAnimationLinkSelf(pAnimTrans)) {
        return true;
    }
    return false;
}

} // namespace

// __ct__Q34nw4r3lyt13AnimTransformFv
AnimTransform::AnimTransform() : mLink(), mpRes(NULL), mFrame(0.0f) {}

// __dt__Q34nw4r3lyt13AnimTransformFv
AnimTransform::~AnimTransform() {}

// GetFrameSize__Q34nw4r3lyt13AnimTransformCFv
u16 AnimTransform::GetFrameSize() const {
    return mpRes->frameSize;
}

// IsLoopData__Q34nw4r3lyt13AnimTransformCFv
bool AnimTransform::IsLoopData() const {
    return mpRes->loop;
}

// __ct__Q34nw4r3lyt18AnimTransformBasicFv
AnimTransformBasic::AnimTransformBasic() : mpFileResAry(nullptr), mAnimLinkAry(nullptr), mAnimLinkNum(0) {}

// __dt__Q34nw4r3lyt18AnimTransformBasicFv
AnimTransformBasic::~AnimTransformBasic() {
    Layout::DeletePrimArray(mAnimLinkAry);
    Layout::DeletePrimArray(mpFileResAry);
}

// SetResource__Q34nw4r3lyt18AnimTransformBasicFPCQ44nw4r3lyt3res14AnimationBlockPQ34nw4r3lyt16ResourceAccessor
void AnimTransformBasic::SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor) {
    SetResource(pRes, pResAccessor, pRes->animContNum);
}

// SetResource__Q34nw4r3lyt18AnimTransformBasicFPCQ44nw4r3lyt3res14AnimationBlockPQ34nw4r3lyt16ResourceAccessorUs
void AnimTransformBasic::SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor, u16 animNum) {
    mpRes = pRes;
    mpFileResAry = nullptr;
    if (mpRes->fileNum != 0) {
        mpFileResAry = Layout::NewArray<void *>(mpRes->fileNum);
        if (mpFileResAry) {
            const u32 *fileNameOffsets = detail::ConvertOffsToPtr<u32>(mpRes, sizeof(*mpRes));
            for (int i = 0; i < mpRes->fileNum; i++) {
                const char *fileName = detail::GetStrTableStr(fileNameOffsets, i);
                mpFileResAry[i] = pResAccessor->GetResource('timg', fileName, nullptr);
            }
        }
    }

    mAnimLinkAry = Layout::NewArray<AnimationLink>(animNum);
    if (mAnimLinkAry) {
        mAnimLinkNum = animNum;
    }
}

// Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt4Paneb
// Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt4Panebb
void AnimTransformBasic::Bind(Pane *pPane, bool bRecursive, bool bDisable) {
    AnimationLink *pCrAnimLink = nullptr;
    const u32 *animContOffsets = detail::ConvertOffsToPtr<u32>(mpRes, mpRes->animContOffsetsOffset);
    for (u16 i = 0; i < mpRes->animContNum; i++) {
        const res::AnimationContent &animCont =
                *detail::ConvertOffsToPtr<res::AnimationContent>(mpRes, animContOffsets[i]);
        if (animCont.type == 0) {
            Pane *pFindPane = pPane->FindPaneByName(animCont.name, bRecursive);
            if (pFindPane && !IsBindAnimation(pFindPane, this)) {
                pCrAnimLink = Bind(pFindPane, pCrAnimLink, i, bDisable);
                if (pCrAnimLink == nullptr) {
                    break;
                }
            }
        } else {
            Material *pFindMat = pPane->FindMaterialByName(animCont.name, bRecursive);
            if (pFindMat && !IsBindAnimation(pFindMat, this)) {
                pCrAnimLink = Bind(pFindMat, pCrAnimLink, i, bDisable);
                if (pCrAnimLink == nullptr) {
                    break;
                }
            }
        }
    }
}

// Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt8Material
// Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt8Materialb
void AnimTransformBasic::Bind(Material *pMaterial, bool bDisable) {
    AnimationLink *pCrAnimLink = nullptr;
    const u32 *animContOffsets = detail::ConvertOffsToPtr<u32>(mpRes, mpRes->animContOffsetsOffset);
    for (u16 i = 0; i < mpRes->animContNum; i++) {
        const res::AnimationContent &animCont =
                *detail::ConvertOffsToPtr<res::AnimationContent>(mpRes, animContOffsets[i]);
        if (animCont.type == 1) {
            if (detail::EqualsMaterialName(pMaterial->GetName(), animCont.name) && !IsBindAnimation(pMaterial, this)) {
                pCrAnimLink = Bind(pMaterial, pCrAnimLink, i, bDisable);
                if (pCrAnimLink == nullptr) {
                    break;
                }
            }
        }
    }
}

// Animate__Q34nw4r3lyt18AnimTransformBasicFUlPQ34nw4r3lyt4Pane
void AnimTransformBasic::Animate(u32 idx, Pane *pPane) {
    u32 animContOffsets = detail::ConvertOffsToPtr<u32>(mpRes, mpRes->animContOffsetsOffset)[idx];
    const res::AnimationContent *pAnimCont = detail::ConvertOffsToPtr<res::AnimationContent>(mpRes, animContOffsets);
    const u32 *animInfoOffsets = detail::ConvertOffsToPtr<u32>(pAnimCont, sizeof(res::AnimationContent));

    for (int i = 0; i < pAnimCont->num; i++) {
        const res::AnimationInfo *pAnimInfo =
                detail::ConvertOffsToPtr<res::AnimationInfo>(pAnimCont, animInfoOffsets[i]);
        const u32 *animTargetOffsets = detail::ConvertOffsToPtr<u32>(pAnimInfo, sizeof(res::AnimationInfo));
        switch (pAnimInfo->kind) {
        case 'RLPA':
            AnimatePaneSRT(pPane, pAnimInfo, animTargetOffsets, mFrame);
            break;
        case 'RLVI':
            AnimateVisibility(pPane, pAnimInfo, animTargetOffsets, mFrame);
            break;
        case 'RLVC':
            AnimateVertexColor(pPane, pAnimInfo, animTargetOffsets, mFrame);
            break;
        }
    }
}

// Animate__Q34nw4r3lyt18AnimTransformBasicFUlPQ34nw4r3lyt8Material
void AnimTransformBasic::Animate(u32 idx, Material *pMaterial) {
    u32 animContOffsets = detail::ConvertOffsToPtr<u32>(mpRes, mpRes->animContOffsetsOffset)[idx];
    const res::AnimationContent *pAnimCont = detail::ConvertOffsToPtr<res::AnimationContent>(mpRes, animContOffsets);
    const u32 *animInfoOffsets = detail::ConvertOffsToPtr<u32>(pAnimCont, sizeof(res::AnimationContent));

    for (int i = 0; i < pAnimCont->num; i++) {
        const res::AnimationInfo *pAnimInfo =
                detail::ConvertOffsToPtr<res::AnimationInfo>(pAnimCont, animInfoOffsets[i]);
        const u32 *animTargetOffsets = detail::ConvertOffsToPtr<u32>(pAnimInfo, sizeof(res::AnimationInfo));
        switch (pAnimInfo->kind) {
        case 'RLMC':
            AnimateMaterialColor(pMaterial, pAnimInfo, animTargetOffsets, mFrame);
            break;
        case 'RLTS':
            AnimateTextureSRT(pMaterial, pAnimInfo, animTargetOffsets, mFrame);
            break;
        case 'RLTP':
            if (mpFileResAry) {
                AnimateTexturePattern(pMaterial, pAnimInfo, animTargetOffsets, mFrame, mpFileResAry);
            }
            break;
        case 'RLIM':
            AnimateIndTexSRT(pMaterial, pAnimInfo, animTargetOffsets, mFrame);
            break;
        }
    }
}

// FindUnbindLink__Q34nw4r3lyt18AnimTransformBasicCFPQ34nw4r3lyt13AnimationLink
AnimationLink *AnimTransformBasic::FindUnbindLink(AnimationLink *pLink) const {
    if (pLink == nullptr) {
        pLink = mAnimLinkAry;
    }

    while (pLink < &mAnimLinkAry[mAnimLinkNum]) {
        if (pLink->GetAnimTransform() == nullptr) {
            return pLink;
        }
        pLink++;
    }

    return nullptr;
}

// Bind<Q34nw4r3lyt4Pane>__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt4PanePQ34nw4r3lyt13AnimationLinkUsb_PQ34nw4r3lyt13AnimationLink
// Bind<Q34nw4r3lyt8Material>__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt8MaterialPQ34nw4r3lyt13AnimationLinkUsb_PQ34nw4r3lyt13AnimationLink
template <typename T>
AnimationLink *AnimTransformBasic::Bind(T *pTarget, AnimationLink *pAnimLink, u16 idx, bool bDisable) {
    pAnimLink = FindUnbindLink(pAnimLink);
    if (!pAnimLink) {
        return nullptr;
    }
    pAnimLink->Set(this, idx, bDisable);
    pTarget->AddAnimationLink(pAnimLink);
    pAnimLink++;
    return pAnimLink;
}

// __ct__Q34nw4r3lyt12AnimResourceFv
AnimResource::AnimResource() {
    Init();
}

// Set__Q34nw4r3lyt12AnimResourceFPCv
void AnimResource::Set(const void *animResBuf) {
    Init();
    const res::BinaryFileHeader *pFileHeader = (res::BinaryFileHeader *)animResBuf;
    if (detail::TestFileHeader(*pFileHeader, 'RLAN')) {
        if (detail::TestFileVersion(*pFileHeader)) {
            mpFileHeader = pFileHeader;
            const res::DataBlockHeader *pDataBlockHead =
                    detail::ConvertOffsToPtr<res::DataBlockHeader>(mpFileHeader, mpFileHeader->headerSize);
            for (int i = 0; i < mpFileHeader->dataBlocks; i++) {
                switch (detail::GetSignatureInt(pDataBlockHead->kind)) {
                case 'pat1':
                    mpTagBlock = (res::AnimationTagBlock *)pDataBlockHead;
                    break;
                case 'pah1':
                    mpShareBlock = (res::AnimationShareBlock *)pDataBlockHead;
                    break;
                case 'pai1':
                    mpResBlock = (res::AnimationBlock *)pDataBlockHead;
                    break;
                }
                pDataBlockHead = detail::ConvertOffsToPtr<res::DataBlockHeader>(pDataBlockHead, pDataBlockHead->size);
            }
        }
    }
}

// Init__Q34nw4r3lyt12AnimResourceFv
void AnimResource::Init() {
    mpFileHeader = nullptr;
    mpResBlock = nullptr;
    mpTagBlock = nullptr;
    mpShareBlock = nullptr;
}

// GetGroupNum__Q34nw4r3lyt12AnimResourceCFv
u16 AnimResource::GetGroupNum() const {
    if (mpTagBlock) {
        return mpTagBlock->groupNum;
    }
    return 0;
}

// GetGroupArray__Q34nw4r3lyt12AnimResourceCFv
const AnimationGroupRef *AnimResource::GetGroupArray() const {
    if (mpTagBlock) {
        const AnimationGroupRef *groups =
                detail::ConvertOffsToPtr<AnimationGroupRef>(mpTagBlock, mpTagBlock->groupsOffset);
        return groups;
    }
    return nullptr;
}

// IsDescendingBind__Q34nw4r3lyt12AnimResourceCFv
bool AnimResource::IsDescendingBind() const {
    if (mpTagBlock) {
        return detail::TestBit(mpTagBlock->flag, 0);
    }
    return false;
}

// GetAnimationShareInfoNum__Q34nw4r3lyt12AnimResourceCFv
u16 AnimResource::GetAnimationShareInfoNum() const {
    if (mpShareBlock) {
        return mpShareBlock->shareNum;
    }
    return 0;
}

// GetAnimationShareInfoArray__Q34nw4r3lyt12AnimResourceCFv
const AnimationShareInfo *AnimResource::GetAnimationShareInfoArray() const {
    if (mpShareBlock) {
        return detail::ConvertOffsToPtr<AnimationShareInfo>(mpShareBlock, mpShareBlock->animShareInfoOffset);
    }
    return 0;
}

// CalcAnimationNum__Q34nw4r3lyt12AnimResourceCFPQ34nw4r3lyt4Paneb
u16 AnimResource::CalcAnimationNum(Pane *pPane, bool bRecursive) const {
    u16 linkNum = 0;
    const u32 *animContOffsets = detail::ConvertOffsToPtr<u32>(mpResBlock, mpResBlock->animContOffsetsOffset);
    for (u16 i = 0; i < mpResBlock->animContNum; i++) {
        const res::AnimationContent &animCont =
                *detail::ConvertOffsToPtr<res::AnimationContent>(mpResBlock, animContOffsets[i]);
        if (animCont.type == 0) {
            Pane *pFindPane = pPane->FindPaneByName(animCont.name, bRecursive);
            if (pFindPane) {
                linkNum++;
            }
        } else {
            Material *pFindMat = pPane->FindMaterialByName(animCont.name, bRecursive);
            if (pFindMat) {
                linkNum++;
            }
        }
    }
    return linkNum;
}

// CalcAnimationNum__Q34nw4r3lyt12AnimResourceCFPQ34nw4r3lyt5Groupb
u16 AnimResource::CalcAnimationNum(Group *pGroup, bool bRecursive) const {
    u16 linkNum = 0;
    ut::LinkList<detail::PaneLink, 0> *paneList = pGroup->GetPaneList();
    for (ut::LinkList<detail::PaneLink, 0>::Iterator it = paneList->GetBeginIter(); it != paneList->GetEndIter();
            it++) {
        linkNum += CalcAnimationNum(it->mTarget, bRecursive);
    }
    return linkNum;
}

namespace detail {

// FindAnimContent__Q44nw4r3lyt6detail12AnimPaneTreeFPCQ44nw4r3lyt3res14AnimationBlockPCcUc
u16 AnimPaneTree::FindAnimContent(const res::AnimationBlock *pAnimBlock, const char *animContName, u8 animContType) {
    const u32 *animContOffsets = detail::ConvertOffsToPtr<u32>(pAnimBlock, pAnimBlock->animContOffsetsOffset);
    for (u16 i = 0; i < pAnimBlock->animContNum; i++) {
        const res::AnimationContent *pAnimCont =
                detail::ConvertOffsToPtr<res::AnimationContent>(pAnimBlock, animContOffsets[i]);
        if (pAnimCont->type == animContType && EqualsMaterialName(pAnimCont->name, animContName)) {
            return i;
        }
    }
    return -1;
}

// Init__Q44nw4r3lyt6detail12AnimPaneTreeFv
void AnimPaneTree::Init() {
    mLinkNum = 0;
    mAnimPaneIdx = 0;
    mAnimMatCnt = 0;
    for (u8 i = 0; i < 9; i++) {
        mAnimMatIdx[i] = 0;
    }
}

// Set__Q44nw4r3lyt6detail12AnimPaneTreeFPQ34nw4r3lyt4PaneRCQ34nw4r3lyt12AnimResource
void AnimPaneTree::Set(Pane *pTargetPane, const AnimResource &animRes) {
    u16 linkNum = 0;
    const res::AnimationBlock *pAnimBlock = animRes.GetResourceBlock();

    u16 animContIdx = FindAnimContent(pAnimBlock, pTargetPane->GetName(), 0);
    if (animContIdx != 0xFFFF) {
        linkNum++;
    }

    u8 animMatCnt = pTargetPane->GetMaterialNum();
    u16 animMatIdxs[9];
    for (u8 i = 0; i < animMatCnt; i++) {
        animMatIdxs[i] = FindAnimContent(pAnimBlock, pTargetPane->GetMaterial(i)->GetName(), 1);
        if (animMatIdxs[i] != 0xFFFF) {
            linkNum++;
        }
    }
    if (linkNum != 0) {
        mAnimRes = animRes;
        mAnimPaneIdx = animContIdx;
        mAnimMatCnt = animMatCnt;
        for (u8 i = 0; i < animMatCnt; i++) {
            mAnimMatIdx[i] = animMatIdxs[i];
        }
        mLinkNum = linkNum;
    }
}

// Bind__Q44nw4r3lyt6detail12AnimPaneTreeCFPQ34nw4r3lyt6LayoutPQ34nw4r3lyt4PanePQ34nw4r3lyt16ResourceAccessor
AnimTransform *AnimPaneTree::Bind(Layout *pLayout, Pane *pTargetPane, ResourceAccessor *pResAccessor) const {
    AnimTransformBasic *pAnimTrans = (AnimTransformBasic *)pLayout->CreateAnimTransform();
    pAnimTrans->SetResource(mAnimRes.GetResourceBlock(), pResAccessor, mLinkNum);

    AnimationLink *pCrAnimLink = nullptr;
    if (mAnimPaneIdx != 0xFFFF) {
        pCrAnimLink = pAnimTrans->Bind(pTargetPane, nullptr, mAnimPaneIdx, true);
    }

    u8 animMatMax = ut::Min<u8>(mAnimMatCnt, pTargetPane->GetMaterialNum());
    for (u8 i = 0; i < animMatMax; i++) {
        if (mAnimMatIdx[i] != 0xFFFF) {
            Material *pMaterial = pTargetPane->GetMaterial(i);
            pCrAnimLink = pAnimTrans->Bind(pMaterial, pCrAnimLink, mAnimMatIdx[i], true);
        }
    }

    return pAnimTrans;
}

// FindAnimationLink__Q34nw4r3lyt6detailFPQ34nw4r2ut38LinkList<Q34nw4r3lyt13AnimationLink,0>PQ34nw4r3lyt13AnimTransform
AnimationLink *FindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, AnimTransform *pAnimTrans) {
    for (ut::LinkList<AnimationLink, 0>::Iterator it = pAnimList->GetBeginIter(); it != pAnimList->GetEndIter(); it++) {
        if (pAnimTrans == it->GetAnimTransform()) {
            return &*it;
        }
    }
    return nullptr;
}

// FindAnimationLink__Q34nw4r3lyt6detailFPQ34nw4r2ut38LinkList<Q34nw4r3lyt13AnimationLink,0>RCQ34nw4r3lyt12AnimResource
AnimationLink *FindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, const AnimResource &animRes) {
    for (ut::LinkList<AnimationLink, 0>::Iterator it = pAnimList->GetBeginIter(); it != pAnimList->GetEndIter(); it++) {
        if (animRes.GetResourceBlock() == it->GetAnimTransform()->GetAnimResource()) {
            return &*it;
        }
    }
    return nullptr;
}

// UnbindAnimationLink__Q34nw4r3lyt6detailFPQ34nw4r2ut38LinkList<Q34nw4r3lyt13AnimationLink,0>PQ34nw4r3lyt13AnimTransform
void UnbindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, AnimTransform *pAnimTrans) {
    ut::LinkList<AnimationLink, 0>::Iterator it = pAnimList->GetBeginIter();
    while (it != pAnimList->GetEndIter()) {
        ut::LinkList<AnimationLink, 0>::Iterator currIt = it++;
        if (pAnimTrans == nullptr || currIt->GetAnimTransform() == pAnimTrans) {
            pAnimList->Erase(currIt);
            currIt->Reset();
        }
    }
}

// Guess, Unused
// UnbindAnimationLink__Q34nw4r3lyt6detailFPQ34nw4r2ut38LinkList<Q34nw4r3lyt13AnimationLink,0>RCQ34nw4r3lyt12AnimResource
void UnbindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, const AnimResource &animRes) {
    ut::LinkList<AnimationLink, 0>::Iterator it = pAnimList->GetBeginIter();
    while (it != pAnimList->GetEndIter()) {
        ut::LinkList<AnimationLink, 0>::Iterator currIt = it++;
        if (currIt->GetAnimTransform()->GetAnimResource() == animRes.GetResourceBlock()) {
            pAnimList->Erase(currIt);
            currIt->Reset();
        }
    }
}

} // namespace detail

} // namespace lyt

} // namespace nw4r
