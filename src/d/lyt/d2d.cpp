#include <d/d_font_manager.h>
#include <d/lyt/d2d.h>
#include <d/lyt/d_textbox.h>
#include <d/lyt/d_window.h>
#include <egg/gfx/eggScreen.h>
#include <nw4r/lyt/lyt_bounding.h>
#include <nw4r/lyt/lyt_group.h>
#include <nw4r/lyt/lyt_picture.h>
#include <nw4r/lyt/lyt_textBox.h>
#include <nw4r/lyt/lyt_utils.h>
#include <nw4r/lyt/lyt_window.h>

using namespace nw4r::lyt;

namespace d2d {

void ResAccIf_c::detach() {
    for (u8 i = 0; i < 5; i++) {
        mAccessor.UnregistFont(&mFontRefLinks[i]);
    }
    m2d::ResAccIf_c::detach();
}

bool ResAccIf_c::attach(void *data, const char *name) {
    nw4r::lyt::FontRefLink *link;
    bool ok = m2d::ResAccIf_c::attach(data, name);
    if (ok) {
        for (u8 i = 0; i < 5; i++) {
            link = &mFontRefLinks[i];
            link->Set(dFontMng_c::getFontName(i), dFontMng_c::getFont(i));
            mAccessor.RegistFont(link);
        }
    }

    return ok;
}

// Copied from lyt, with some types replaced
nw4r::lyt::Pane *Layout_c::BuildPaneObj(s32 kind, const void *dataPtr, const ResBlockSet &resBlockSet) {
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
        return NewObj<dTextBox_c>(pBlock, resBlockSet);
    } break;
    case 'wnd1': {
        const res::Window *pBlock = (const res::Window *)dataPtr;
        return NewObj<dWindow_c>(pBlock, resBlockSet);
    } break;
    case 'bnd1': {
        const res::Bounding *pResBounding = (const res::Bounding *)dataPtr;
        return NewObj<Bounding>(pResBounding, resBlockSet);
    } break;
    default:
        return nullptr;
    }
}

// This is exactly copied from nw4r::lyt::Layout
bool Layout_c::Build(const void *lytResBuf, ResourceAccessor *pResAcsr) {
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
        default:
            break;
        }
        dataPtr = ((u8 *)dataPtr + pDataBlockHead->size);
    }
    return true;
}

Multi_c::Multi_c() : Base_c(0x80), mLayout(), mDrawInfo(), mpResAcc(nullptr), mFlags(0) {
    mDrawInfo.SetLocationAdjustScale(nw4r::math::VEC2((float)EGG::Screen::GetSizeXMax(EGG::Screen::TV_MODE_1) /
                    (float)EGG::Screen::GetSizeXMax(EGG::Screen::TV_MODE_2),
            1.0f));
    mDrawInfo.SetLocationAdjust(true);
}

void Multi_c::animate() {
    u32 arg = 0;
    if ((mFlags & 1) != 0) {
        arg = 1;
    }
    mLayout.Animate(arg);
}

void Multi_c::calc() {
    calcBefore();
    calcAfter();
}

void Multi_c::calcBefore() {
    u32 arg = 0;
    if ((mFlags & 1) != 0) {
        arg = 1;
    }
    mLayout.Animate(arg);
}

void Multi_c::calcAfter() {
    nw4r::ut::Rect rect = mLayout.GetLayoutRect();
    mDrawInfo.SetViewRect(rect);
    mDrawInfo.SetMultipleViewMtxOnDraw(true);
    mLayout.CalculateMtx(mDrawInfo);
}

void Multi_c::draw() {}

bool Multi_c::build(const char *name, m2d::ResAccIf_c *acc) {
    if (mLayout.GetRootPane() != nullptr) {
        return true;
    }

    if (acc == nullptr) {
        acc = mpResAcc;
        if (acc == nullptr) {
            return false;
        }
    }

    void *res = acc->getAccessor()->GetResource(0, name, nullptr);
    if (res == nullptr) {
        return false;
    }

    bool ok = mLayout.Build(res, acc->getAccessor());
    if (ok) {
        calc();
    }
    return ok;
}

nw4r::lyt::Pane *Multi_c::findPane(const char *name) const {
    return mLayout.GetRootPane()->FindPaneByName(name, true);
}

nw4r::lyt::Picture *Multi_c::findPicture(const char *name) const {
    nw4r::lyt::Pane *p = findPane(name);
    if (p != nullptr) {
        return nw4r::ut::DynamicCast<nw4r::lyt::Picture *>(p);
    }
    return nullptr;
}

nw4r::lyt::Bounding *Multi_c::findBounding(const char *name) const {
    nw4r::lyt::Pane *p = findPane(name);
    if (p != nullptr) {
        return nw4r::ut::DynamicCast<nw4r::lyt::Bounding *>(p);
    }
    return nullptr;
}

void Multi_c::unbindAnims() {
    mLayout.UnbindAllAnimation();
}

LytBase_c::LytBase_c() : mpMsbtInfo(nullptr) {}

extern "C" void libms__LMS_CloseMessage(void *);
LytBase_c::~LytBase_c() {
    if (mpMsbtInfo != nullptr) {
        libms__LMS_CloseMessage(mpMsbtInfo);
    }
}

bool AnmGroup_c::init(const char *fileName, m2d::ResAccIf_c *resAcc, d2d::Layout_c *layout, const char *groupName) {
    if (layout->GetGroupContainer() == nullptr) {
        return false;
    }

    nw4r::lyt::Group *group = layout->GetGroupContainer()->FindGroupByName(groupName);
    if (group == nullptr) {
        return nullptr;
    }
    nw4r::lyt::AnimTransform *transform = layout->CreateAnimTransform();
    return init(transform, fileName, resAcc, group);
}

bool AnmGroup_c::init(nw4r::lyt::AnimTransform *transform, const char *name, m2d::ResAccIf_c *acc,
        nw4r::lyt::Group *group) {
    if (transform == nullptr) {
        return false;
    }

    if (group == nullptr) {
        return false;
    }

    void *res = acc->getAccessor()->GetResource(0, name, nullptr);
    if (res == nullptr) {
        return false;
    }

    mAnmResource.Set(res);
    u16 animNum = mAnmResource.CalcAnimationNum(group, mAnmResource.IsDescendingBind());
    if (animNum != 0) {
        transform->SetResource(mAnmResource.GetResourceBlock(), acc->getAccessor(), animNum);
        mpGroup = group;
        mpAnmTransform = transform;
    }
    return true;
}

bool AnmGroup_c::fn_800AC6D0(bool b) {
    nw4r::lyt::AnimTransform *anmTransform = mpAnmTransform;
    if (anmTransform == nullptr) {
        return false;
    }

    if (mpGroup == nullptr) {
        return false;
    }

    nw4r::lyt::BindAnimation(mpGroup, anmTransform, false, b);
    mFlags |= 1;
    if (b) {
        mFlags = (mFlags & ~2);
    } else {
        mFlags |= 2;
    }

    u32 flags = 1;
    if (mpAnmTransform->IsLoopData()) {
        flags = 0;
    }

    mpFrameCtrl->set(anmTransform->GetFrameSize(), flags, 1.0f, -1.0f);
    syncAnmFrame();
    return true;
}

bool AnmGroup_c::fn_800AC7D0() {
    nw4r::lyt::AnimTransform *anmTransform = mpAnmTransform;
    if (anmTransform == nullptr) {
        return false;
    }

    nw4r::lyt::Group *group = mpGroup;
    if (group == nullptr) {
        return false;
    }

    nw4r::lyt::UnbindAnimation(group, anmTransform, mAnmResource.IsDescendingBind());
    mFlags = (mFlags & ~1);
    return true;
}

bool AnmGroup_c::fn_800AC860() {
    return true;
}

void AnmGroup_c::fn_800AC870(bool b) {
    nw4r::lyt::Group *group = mpGroup;
    nw4r::lyt::AnimTransform *anmTransform = mpAnmTransform;
    nw4r::lyt::SetAnimationEnable(group, anmTransform, b, mAnmResource.IsDescendingBind());
    if (b) {
        mFlags |= 2;
    } else {
        mFlags = mFlags & ~2;
    }
}

void AnmGroup_c::setAnmFrame(f32 arg) {
    mpAnmTransform->SetFrame(arg);
}

void AnmGroup_c::syncAnmFrame() {
    setAnmFrame(mpFrameCtrl->getFrame());
}

void AnmGroup_c::setForward() {
    mpFrameCtrl->setForward();
}

void AnmGroup_c::setBackward() {
    mpFrameCtrl->setBackward();
}

} // namespace d2d
