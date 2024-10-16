#include "nw4r/lyt/lyt_layout.h"

#include "nw4r/lyt/lyt_animation.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/lyt/lyt_picture.h"
#include "nw4r/lyt/lyt_textBox.h"
#include "nw4r/lyt/lyt_utils.h"
#include "nw4r/lyt/lyt_window.h"
#include "nw4r/ut/ut_RuntimeTypeInfo.h"
#include "nw4r/ut/ut_TagProcessorBase.h"


namespace nw4r {

namespace lyt {

// mspAllocator__Q34nw4r3lyt6Layout
MEMAllocator *Layout::mspAllocator;

namespace {

// SetTagProcessorImpl__Q34nw4r3lyt24@unnamed@lyt_layout_cpp@FPQ34nw4r3lyt4PanePQ34nw4r2ut19TagProcessorBase<w>
void SetTagProcessorImpl(Pane *pPane, ut::TagProcessorBase<wchar_t> *pTagProcessor) {
    TextBox *pTextBox = ut::DynamicCast<TextBox *>(pPane);
    if (pTextBox) {
        pTextBox->SetTagProcessor(pTagProcessor);
    }
    for (ut::LinkList<Pane, 4>::Iterator it = pPane->GetChildList()->GetBeginIter();
         it != pPane->GetChildList()->GetEndIter(); ++it) {
        SetTagProcessorImpl(&*it, pTagProcessor);
    }
}

// IsIncludeAnimationGroupRef__Q34nw4r3lyt24@unnamed@lyt_layout_cpp@FPQ34nw4r3lyt14GroupContainerPCQ34nw4r3lyt17AnimationGroupRefUsbPQ34nw4r3lyt4Pane
// Doesnt match DWARF Vars, but matches
bool IsIncludeAnimationGroupRef(
    GroupContainer *pGroupContainer, const AnimationGroupRef *groupRefs, u16 bindGroupNum, bool bDescendingBind,
    Pane *pTargetPane
) {
    for (u16 grpIdx = 0; grpIdx < bindGroupNum; grpIdx++) {
        Group *pGroup = pGroupContainer->FindGroupByName(groupRefs[grpIdx].GetName());
        for (ut::LinkList<detail::PaneLink, 0>::Iterator paneList = pGroup->GetPaneList()->GetBeginIter();
             paneList != pGroup->GetPaneList()->GetEndIter(); paneList++) {
            const Pane *t = paneList->mTarget;
            if (t == pTargetPane) {
                return true;
            }
            if (bDescendingBind) {
                for (const Pane *pParentPane = pTargetPane->GetParent(); pParentPane != nullptr;
                     pParentPane = pParentPane->GetParent()) {
                    if (t == pParentPane) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

} // namespace

// __ct__Q34nw4r3lyt6LayoutFv
Layout::Layout() : mAnimTransList(), mpRootPane(nullptr), mpGroupContainer(nullptr) {}

// __dt__Q34nw4r3lyt6LayoutFv
Layout::~Layout() {
    DeleteObj(mpGroupContainer);
    if (mpRootPane && !mpRootPane->IsUserAllocated()) {
        DeleteObj(mpRootPane);
    }
    ut::LinkList<AnimTransform, 4>::Iterator it = mAnimTransList.GetBeginIter();
    while (it != mAnimTransList.GetEndIter()) {
        ut::LinkList<AnimTransform, 4>::Iterator currIt = it++;
        mAnimTransList.Erase(currIt);
        DeleteObj(&*currIt);
    }
}

// Build__Q34nw4r3lyt6LayoutFPCvPQ34nw4r3lyt16ResourceAccessor
bool Layout::Build(const void *lytResBuf, ResourceAccessor *pResAcsr) {
    const res::BinaryFileHeader *pFileHead = (const res::BinaryFileHeader *)(lytResBuf);
    if (!detail::TestFileHeader(*pFileHead, 'RLYT')) {
        return false;
    }
    if (!detail::TestFileVersion(*pFileHead)) {
        return false;
    }

    ResBlockSet resBlockSet = {nullptr, nullptr, nullptr, pResAcsr};
    Pane *pParentPane = nullptr;
    Pane *pLastPane = nullptr;
    bool bReadRootGroup = false;
    int groupNestLevel = 0;

    const void *dataPtr = ((u8 *)lytResBuf + pFileHead->headerSize);
    for (int i = 0; i < pFileHead->dataBlocks; i++) {
        const res::DataBlockHeader *pDataBlockHead = (const res::DataBlockHeader *)dataPtr;
        switch (detail::GetSignatureInt(pDataBlockHead->kind)) {
            case 'lyt1': // Main Layout
            {
                const res::Layout *pResLyt = ((const res::Layout *)dataPtr);
                mLayoutSize = pResLyt->layoutSize;
            } break;
            case 'txl1': // Texture List
                resBlockSet.pTextureList = (const res::TextureList *)dataPtr;
                break;
            case 'fnl1': // Font List
                resBlockSet.pFontList = (const res::FontList *)dataPtr;
                break;
            case 'mat1': // Material
                resBlockSet.pMaterialList = (const res::MaterialList *)dataPtr;
                break;
            case 'wnd1': // Window
            case 'pan1': // Pane
            case 'pic1': // Picture
            case 'txt1': // Text Box
            case 'bnd1': // Boundary Pane
            {
                Pane *pPane = BuildPaneObj(detail::GetSignatureInt(pDataBlockHead->kind), dataPtr, resBlockSet);
                if (pPane) {
                    if (mpRootPane == nullptr) {
                        mpRootPane = pPane;
                    }
                    if (pParentPane) {
                        pParentPane->AppendChild(pPane);
                    }
                    pLastPane = pPane;
                }
            } break;
            case 'usd1': // User Data
                pLastPane->SetExtUserDataList((const res::ExtUserDataList *)dataPtr);
                break;
            case 'pas1': // PaneChildren Start
                pParentPane = pLastPane;
                break;
            case 'pae1': // PaneChildren End
                pLastPane = pParentPane;
                pParentPane = pLastPane->GetParent();
                break;
            case 'grp1': // Group
                if (!bReadRootGroup) {
                    bReadRootGroup = true;
                    mpGroupContainer = NewObj<GroupContainer>();
                } else {
                    if (mpGroupContainer && groupNestLevel == 1) {
                        Group *pGroup = NewObj<Group>((const res::Group *)dataPtr, mpRootPane);
                        if (pGroup) {
                            mpGroupContainer->AppendGroup(pGroup);
                        }
                    }
                }
                break;
            case 'grs1': // Group Children Start
                groupNestLevel++;
                break;
            case 'gre1': // Group Children End
                groupNestLevel--;
                break;
            default: break;
        }
        dataPtr = ((u8 *)dataPtr + pDataBlockHead->size);
    }
    return true;
}

// CreateAnimTransform__Q34nw4r3lyt6LayoutFv
AnimTransform *Layout::CreateAnimTransform() {
    AnimTransformBasic *pAnimTrans = NewObj<AnimTransformBasic>();
    if (pAnimTrans) {
        mAnimTransList.PushBack(pAnimTrans);
    }
    return pAnimTrans;
}

// CreateAnimTransform__Q34nw4r3lyt6LayoutFPCvPQ34nw4r3lyt16ResourceAccessor
AnimTransform *Layout::CreateAnimTransform(const void *animResBuf, ResourceAccessor *pResAcsr) {
    return CreateAnimTransform(AnimResource(animResBuf), pResAcsr);
}

// CreateAnimTransform__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt12AnimResourcePQ34nw4r3lyt16ResourceAccessor
AnimTransform *Layout::CreateAnimTransform(const AnimResource &animRes, ResourceAccessor *pResAcsr) {
    const res::AnimationBlock *pAnimBlock = animRes.GetResourceBlock();
    if (!pAnimBlock) {
        return nullptr;
    }

    AnimTransform *pAnimTrans = CreateAnimTransform();
    if (pAnimTrans) {
        pAnimTrans->SetResource(pAnimBlock, pResAcsr);
    }

    return pAnimTrans;
}

// BindAnimation__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransform
void Layout::BindAnimation(AnimTransform *pAnimTrans) {
    if (!mpRootPane) {
        return;
    }
    mpRootPane->BindAnimation(pAnimTrans, true, false);
}

// UnbindAnimation__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransform
void Layout::UnbindAnimation(AnimTransform *pAnimTrans) {
    if (!mpRootPane) {
        return;
    }
    mpRootPane->UnbindAnimation(pAnimTrans, true);
}

// UnbindAllAnimation__Q34nw4r3lyt6LayoutFv
void Layout::UnbindAllAnimation() {
    UnbindAnimation(nullptr);
}

// BindAnimationAuto__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt12AnimResourcePQ34nw4r3lyt16ResourceAccessor
bool Layout::BindAnimationAuto(const AnimResource &animRes, ResourceAccessor *pResAcsr) {
    // Ensure Root pane and Resource Block Exists
    if (!mpRootPane) {
        return false;
    }
    if (!animRes.GetResourceBlock()) {
        return false;
    }

    AnimTransform *pAnimTrans = CreateAnimTransform();
    u16 bindGroupNum = animRes.GetGroupNum();
    u16 animNum = 0;
    if (bindGroupNum == 0) {
        // No Groups to bind with, only bind to root pane
        pAnimTrans->SetResource(animRes.GetResourceBlock(), pResAcsr, animRes.GetResourceBlock()->animContNum);
        mpRootPane->BindAnimation(pAnimTrans, true, true);
    } else {
        // Bind to all Groups
        const AnimationGroupRef *groupRefs = animRes.GetGroupArray();
        for (int grpIdx = 0; grpIdx < bindGroupNum; grpIdx++) {
            Group *pGroup = mpGroupContainer->FindGroupByName(groupRefs[grpIdx].GetName());
            animNum += animRes.CalcAnimationNum(pGroup, animRes.IsDescendingBind());
        }
        pAnimTrans->SetResource(animRes.GetResourceBlock(), pResAcsr, animNum);
        for (int grpIdx = 0; grpIdx < bindGroupNum; grpIdx++) {
            Group *pGroup = mpGroupContainer->FindGroupByName(groupRefs[grpIdx].GetName());
            lyt::BindAnimation(pGroup, pAnimTrans, animRes.IsDescendingBind(), true);
        }
    }
    u16 animSharInfoNum = animRes.GetAnimationShareInfoNum();
    if (animSharInfoNum != 0) {
        const AnimationShareInfo *animSharInfoAry = animRes.GetAnimationShareInfoArray();
        for (int i = 0; i < animSharInfoNum; i++) {
            Pane *pSrcPane = mpRootPane->FindPaneByName(animSharInfoAry[i].GetSrcPaneName(), true);
            detail::AnimPaneTree animPaneTree = detail::AnimPaneTree(pSrcPane, animRes);
            if (animPaneTree.GetLinkNum()) {
                Group *pGroup = mpGroupContainer->FindGroupByName(animSharInfoAry[i].GetTargetGroupName());
                ut::LinkList<detail::PaneLink, 0> *paneList = pGroup->GetPaneList();
                for (ut::LinkList<detail::PaneLink, 0>::Iterator it = paneList->GetBeginIter();
                     it != paneList->GetEndIter(); it++) {
                    if (it->mTarget != pSrcPane) {
                        if (bindGroupNum != 0) {
                            bool bInclude = IsIncludeAnimationGroupRef(
                                mpGroupContainer, animRes.GetGroupArray(), bindGroupNum, animRes.IsDescendingBind(),
                                it->mTarget
                            );
                            if (!bInclude) {
                                continue;
                            }
                        }
                        animPaneTree.Bind(this, it->mTarget, pResAcsr);
                    }
                }
            }
        }
    }
    return true;
}

// SetAnimationEnable__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransformb
void Layout::SetAnimationEnable(AnimTransform *pAnimTrans, bool bEnable) {
    if (!mpRootPane) {
        return;
    }
    mpRootPane->SetAnimationEnable(pAnimTrans, bEnable, true);
}

// CalculateMtx__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt8DrawInfo
void Layout::CalculateMtx(const DrawInfo &drawInfo) {
    if (!mpRootPane) {
        return;
    }
    mpRootPane->CalculateMtx(drawInfo);
}

// Draw__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt8DrawInfo
void Layout::Draw(const DrawInfo &drawInfo) {
    if (!mpRootPane) {
        return;
    }
    mpRootPane->Draw(drawInfo);
}

// Animate__Q34nw4r3lyt6LayoutFUl
void Layout::Animate(u32 option) {
    if (!mpRootPane) {
        return;
    }
    mpRootPane->Animate(option);
}

// GetLayoutRect__Q34nw4r3lyt6LayoutCFv
ut::Rect Layout::GetLayoutRect() const {
    return ut::Rect(-mLayoutSize.width / 2, mLayoutSize.height / 2, mLayoutSize.width / 2, -mLayoutSize.height / 2);
}

// SetTagProcessor__Q34nw4r3lyt6LayoutFPQ34nw4r2ut19TagProcessorBase<w>
void Layout::SetTagProcessor(ut::TagProcessorBase<wchar_t> *pTagProcessor) {
    SetTagProcessorImpl(this->mpRootPane, pTagProcessor);
}

// BuildPaneObj__Q34nw4r3lyt6LayoutFlPCvRCQ34nw4r3lyt11ResBlockSet
Pane *Layout::BuildPaneObj(s32 kind, const void *dataPtr, const ResBlockSet &resBlockSet) {
    // Oddly enough the breaks are required here to not inline the function???
    // Had them as left over from editing and somehow when removing them it just broke Build.
    // Probably some analysis depth as 3 break statements do it
    switch (kind) {
        case 'pan1': {
            const res::Pane *pResPane = (const res::Pane *)dataPtr;
            return NewObj<Pane>(pResPane);
        } break;
        case 'pic1': {
            const res::Picture *pResPic = (const res::Picture *)dataPtr;
            return NewObj<Picture>(pResPic, resBlockSet);
        } break;
        case 'txt1': {
            const res::TextBox *pBlock = (const res::TextBox *)dataPtr;
            return NewObj<TextBox>(pBlock, resBlockSet);
        } break;
        case 'wnd1': {
            const res::Window *pBlock = (const res::Window *)dataPtr;
            return NewObj<Window>(pBlock, resBlockSet);
        } break;
        case 'bnd1': {
            const res::Bounding *pResBounding = (const res::Bounding *)dataPtr;
            return NewObj<Bounding>(pResBounding, resBlockSet);
        } break;
        default: return nullptr;
    }
}

} // namespace lyt

} // namespace nw4r
