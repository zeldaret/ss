#include "nw4r/lyt/lyt_pane.h"

#include "nw4r/lyt/lyt_drawInfo.h"
#include "nw4r/lyt/lyt_layout.h"
#include "nw4r/lyt/lyt_material.h"
#include "nw4r/ut/ut_Color.h"
#include "nw4r/ut/ut_Rect.h"
#include "nw4r/ut/ut_RuntimeTypeInfo.h"
#include "nw4r/ut/ut_list.h"


// ReverseYAxis__22@unnamed@lyt_pane_cpp@FPQ34nw4r4math5MTX34
namespace nw4r {

namespace lyt {
namespace detail {
// __ct__Q44nw4r3lyt6detail8PaneBaseFv
PaneBase::PaneBase() : mLink() {}

// __dt__Q44nw4r3lyt6detail8PaneBaseFv
PaneBase::~PaneBase() {}

} // namespace detail

NW4R_UT_RTTI_DEF_BASE(Pane);

// __ct__Q34nw4r3lyt4PaneFv
// Guess pulled from BBA/slight modified
Pane::Pane() : mChildList(), mAnimList(), mSize() {
    this->mpParent = nullptr;
    this->mpMaterial = nullptr;
    this->mbUserAllocated = false;
    this->mpExtUserDataList = nullptr;
    this->mBasePosition = 4;
    memset(this->mName, 0, PANE_NAME_SIZE + 1);
    memset(this->mUserData, 0, PANE_USERDATA_SIZE + 1);
    this->mTranslate = math::VEC3(0.0f, 0.0f, 0.0f);
    this->mRotate = math::VEC3(0.0f, 0.0f, 0.0f);
    this->mScale = math::VEC2(1.0f, 1.0f);
    this->mSize = Size();
    this->mAlpha = 0xFF;
    this->mGlbAlpha = 0xFF;
    SetVisible(true);
}

// __dt__Q34nw4r2ut38LinkList<Q34nw4r3lyt13AnimationLink,0>Fv

// __dt__Q34nw4r2ut28LinkList<Q34nw4r3lyt4Pane,4>Fv

// 80486a70
// __ct__Q34nw4r3lyt4PaneFPCQ44nw4r3lyt3res4Pane
Pane::Pane(const res::Pane *pBlock) : mChildList(), mAnimList(), mSize() {
    this->mpParent = nullptr;
    this->mpMaterial = nullptr;
    this->mbUserAllocated = false;
    this->mpExtUserDataList = nullptr;
    this->mBasePosition = pBlock->basePosition;
    strncpy(this->mName, pBlock->name, PANE_NAME_SIZE);
    this->mName[PANE_NAME_SIZE] = '\0';
    strncpy(this->mUserData, pBlock->mUserData, PANE_USERDATA_SIZE);
    this->mUserData[PANE_USERDATA_SIZE] = '\0';
    this->mTranslate = pBlock->translate;
    this->mRotate = pBlock->rotate;
    this->mScale = pBlock->scale;
    this->mSize = pBlock->size;
    this->mGlbAlpha = this->mAlpha = pBlock->alpha;
    this->mFlag = pBlock->flag;
}

// Init__Q34nw4r3lyt4PaneFv
void Pane::Init() {
    this->mpParent = NULL;
    this->mpMaterial = NULL;
    this->mbUserAllocated = false;
    this->mpExtUserDataList = NULL;
}

// __dt__Q34nw4r3lyt4PaneFv
Pane::~Pane() {
    ut::LinkList<Pane, 4>::Iterator it = this->mChildList.GetBeginIter();
    while (it != this->mChildList.GetEndIter()) {
        ut::LinkList<Pane, 4>::Iterator currIt = it++;
        mChildList.Erase(currIt);
        if (!currIt->mbUserAllocated) {
            Layout::DeleteObj(&*currIt);
        }
    }
    this->UnbindAnimationSelf(nullptr);
    if (this->mpMaterial && !this->mpMaterial->IsUserAllocated()) {
        Layout::DeleteObj(this->mpMaterial);
    }
}

// SetName__Q34nw4r3lyt4PaneFPCc
void Pane::SetName(const char *pName) {
    strncpy(this->mName, pName, PANE_NAME_SIZE);
    this->mName[PANE_NAME_SIZE - 1] = '\0';
}

// SetUserData__Q34nw4r3lyt4PaneFPCc
void Pane::SetUserData(const char *pData) {
    strncpy(this->mUserData, pData, PANE_USERDATA_SIZE);
    this->mUserData[PANE_USERDATA_SIZE - 1] = '\0';
}

// AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane
void Pane::AppendChild(Pane *pChild) {
    InsertChild(this->mChildList.GetEndIter(), pChild);
}

// PrependChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane
void Pane::PrependChild(Pane *pChild) {
    InsertChild(this->mChildList.GetBeginIter(), pChild);
}

// InsertChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4PanePQ34nw4r3lyt4Pane
void Pane::InsertChild(Pane *at, Pane *pChild) {
    this->mChildList.Insert(mChildList.GetIteratorFromPointer(&at->mLink), pChild);
    pChild->mpParent = this;
}

// InsertChild__Q34nw4r3lyt4PaneFQ44nw4r2ut28LinkList<Q34nw4r3lyt4Pane,4>8IteratorPQ34nw4r3lyt4Pane
// Guess for now. Not in SS
void Pane::InsertChild(ut::LinkList<Pane, 4>::Iterator next, Pane *pChild) {
    this->mChildList.Insert(next, pChild);
    pChild->mpParent = this;
}

// RemoveChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane
void Pane::RemoveChild(Pane *pChild) {
    this->mChildList.Erase(pChild);
    pChild->mpParent = nullptr;
}
// GetPaneRect__Q34nw4r3lyt4PaneCFv
ut::Rect Pane::GetPaneRect() const {
    ut::Rect ret;
    math::VEC2 basePt = GetVtxPos();

    ret.left = basePt.x;
    ret.top = basePt.y;
    ret.right = basePt.x + mSize.width;
    ret.bottom = basePt.y - mSize.height;

    return ret;
}

// GetPaneRect__Q34nw4r3lyt4PaneCFRCQ34nw4r3lyt8DrawInfo
ut::Rect Pane::GetPaneRect(const DrawInfo &) const {
    return GetPaneRect();
}

// GetVtxColor__Q34nw4r3lyt4PaneCFUl
ut::Color Pane::GetVtxColor(u32) const {
    return ut::Color();
}

// SetVtxColor__Q34nw4r3lyt4PaneFUlQ34nw4r2ut5Color
void Pane::SetVtxColor(u32, ut::Color) {}

// GetColorElement__Q34nw4r3lyt4PaneCFUl
u8 Pane::GetColorElement(u32 idx) const {
    if (idx == 0x10) {
        return this->mAlpha;
    } else {
        return GetVtxColorElement(idx);
    }
}

// SetColorElement__Q34nw4r3lyt4PaneFUlUc
void Pane::SetColorElement(u32 idx, u8 value) {
    if (idx == 0x10) {
        this->mAlpha = value;
    } else {
        return SetVtxColorElement(idx, value);
    }
}

// GetVtxColorElement__Q34nw4r3lyt4PaneCFUl
u8 Pane::GetVtxColorElement(u32 idx) const {
    return 0xFF;
}

// SetVtxColorElement__Q34nw4r3lyt4PaneFUlUc
void Pane::SetVtxColorElement(u32 idx, u8 value) {}

// FindPaneByName__Q34nw4r3lyt4PaneFPCcb
Pane *Pane::FindPaneByName(const char *findName, bool bRecursive) {
    if (detail::EqualsResName(this->mName, findName)) {
        return this;
    }
    if (bRecursive) {
        for (ut::LinkList<Pane, 4>::Iterator it = this->mChildList.GetBeginIter(); it != this->mChildList.GetEndIter();
             it++) {
            Pane *pPane = it->FindPaneByName(findName, bRecursive);
            if (pPane != nullptr) {
                return pPane;
            }
        }
    }
    return nullptr;
}

// FindMaterialByName__Q34nw4r3lyt4PaneFPCcb
Material *Pane::FindMaterialByName(const char *findName, bool bRecursive) {
    if (this->mpMaterial && detail::EqualsMaterialName(this->mpMaterial->GetName(), findName)) {
        return this->mpMaterial;
    }
    if (bRecursive) {
        for (ut::LinkList<Pane, 4>::Iterator it = this->mChildList.GetBeginIter(); it != this->mChildList.GetEndIter();
             it++) {
            Material *pPane = it->FindMaterialByName(findName, bRecursive);
            if (pPane != nullptr) {
                return pPane;
            }
        }
    }
    return nullptr;
}

// CalculateMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo
// Matches for SS, applying the rotation and scale seems to be different accross versions.
// Also look out for the Bottom CalculateMtxChild section. In other version this is actually seperated differently
void Pane::CalculateMtx(const DrawInfo &drawInfo) {
    if (!IsVisible() && !drawInfo.IsInvisiblePaneCalculateMtx()) {
        return;
    }

    math::VEC2 scale = this->mScale;

    if (drawInfo.IsLocationAdjust() && IsLocationAdjust()) {
        scale.x *= drawInfo.GetLocationAdjustScale().x;
        scale.y *= drawInfo.GetLocationAdjustScale().y;
    }
    if (mRotate.x != 0.0f || mRotate.y != 0.0f) {
        f32 sinx, siny, sinz;
        f32 cosx, cosy, cosz;

        math::SinCosDeg(&sinx, &cosx, mRotate.x);
        math::SinCosDeg(&siny, &cosy, mRotate.y);
        math::SinCosDeg(&sinz, &cosz, mRotate.z);
        const f32 cosz_cosx = cosz * cosx;
        const f32 siny_sinx = siny * sinx;
        const f32 sinz_cosx = sinz * cosx;
        this->mMtx._00 = cosz * cosy * scale.x;
        this->mMtx._10 = (sinz * cosy) * scale.x;
        this->mMtx._20 = (-siny) * scale.x;
        this->mMtx._01 = (-sinz_cosx + cosz * siny_sinx) * scale.y;
        this->mMtx._11 = (cosz_cosx + sinz * siny_sinx) * scale.y;
        this->mMtx._21 = (cosy * sinx) * scale.y;
        this->mMtx._02 = (sinz * sinx) + (cosz_cosx * siny);
        this->mMtx._12 = (-cosz * sinx) + (sinz_cosx * siny);
        this->mMtx._22 = (cosy * cosx);

    } else if (mRotate.z != 0.0f) {
        f32 sinz;
        f32 cosz;
        math::SinCosDeg(&sinz, &cosz, mRotate.z);
        this->mMtx._00 = cosz * scale.x;
        this->mMtx._01 = -sinz * scale.y;
        this->mMtx._02 = 0.0f;
        this->mMtx._10 = sinz * scale.x;
        this->mMtx._11 = cosz * scale.y;
        this->mMtx._12 = 0.0f;
        this->mMtx._20 = 0.0f;
        this->mMtx._21 = 0.0f;
        this->mMtx._22 = 1.0f;
    } else {
        this->mMtx._00 = scale.x;
        this->mMtx._01 = 0.0f;
        this->mMtx._02 = 0.0f;
        this->mMtx._10 = 0.0f;
        this->mMtx._11 = scale.y;
        this->mMtx._12 = 0.0f;
        this->mMtx._20 = 0.0f;
        this->mMtx._21 = 0.0f;
        this->mMtx._22 = 1.0f;
    }
    this->mMtx._03 = this->mTranslate.x;
    this->mMtx._13 = this->mTranslate.y;
    this->mMtx._23 = this->mTranslate.z;
    if (this->mpParent) {
        PSMTXConcat(this->mpParent->mGlbMtx, this->mMtx, this->mGlbMtx);
    } else {
        if (drawInfo.IsMultipleViewMtxOnDraw()) {
            this->mGlbMtx = this->mMtx;
        } else {
            PSMTXConcat(drawInfo.GetViewMtx(), this->mMtx, this->mGlbMtx);
        }
    }

    u8 alpha;
    bool b = drawInfo.IsInfluencedAlpha() && this->mpParent;
    if (b) {
        alpha = this->mAlpha * drawInfo.GetGlobalAlpha();
    } else {
        alpha = this->mAlpha;
    }
    this->mGlbAlpha = alpha;

    if (IsInfluencedAlpha() && this->mAlpha != 0xFF) {
        DrawInfo &mtDrawInfo = const_cast<DrawInfo &>(drawInfo);
        const f32 crGlobalAlpha = drawInfo.GetGlobalAlpha();
        const bool bCdInfluenced = drawInfo.IsInfluencedAlpha();
        mtDrawInfo.SetGlobalAlpha(crGlobalAlpha * mAlpha * (1.0f / 0xFF));
        mtDrawInfo.SetInfluencedAlpha(true);
        CalculateMtxChild(drawInfo);
        mtDrawInfo.SetGlobalAlpha(crGlobalAlpha);
        mtDrawInfo.SetInfluencedAlpha(bCdInfluenced);
    } else {
        CalculateMtxChild(drawInfo);
    }
}

// CalculateMtxChild__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo
// Guess but makes sense
void Pane::CalculateMtxChild(const DrawInfo &drawInfo) {
    for (ut::LinkList<Pane, 4>::Iterator it = this->mChildList.GetBeginIter(); it != this->mChildList.GetEndIter();
         it++) {
        it->CalculateMtx(drawInfo);
    }
}

// Draw__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo
void Pane::Draw(const DrawInfo &drawInfo) {
    if (IsVisible()) {
        this->DrawSelf(drawInfo);
        for (ut::LinkList<Pane, 4>::Iterator it = this->mChildList.GetBeginIter(); it != this->mChildList.GetEndIter();
             it++) {
            it->Draw(drawInfo);
        }
    }
}

// DrawSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo
void Pane::DrawSelf(const DrawInfo &drawInfo) {}

// Animate__Q34nw4r3lyt4PaneFUl
void Pane::Animate(u32 option) {
    this->AnimateSelf(option);
    if (IsVisible() || !(option & 1)) {
        for (ut::LinkList<Pane, 4>::Iterator it = this->mChildList.GetBeginIter(); it != this->mChildList.GetEndIter();
             it++) {
            it->Animate(option);
        }
    }
}

// AnimateSelf__Q34nw4r3lyt4PaneFUl
void Pane::AnimateSelf(u32 option) {
    for (ut::LinkList<AnimationLink, 0>::Iterator it = this->mAnimList.GetBeginIter();
         it != this->mAnimList.GetEndIter(); it++) {
        if (it->IsEnable()) {
            AnimTransform *animTrans = it->GetAnimTransform();
            animTrans->Animate(it->GetIndex(), this);
        }
    }
    if (IsVisible() || !detail::TestBit(option, 0)) {
        if (this->mpMaterial) {
            mpMaterial->Animate();
        }
    }
}

// BindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb
void Pane::BindAnimation(AnimTransform *pAnimTrans, bool bRecursive, bool bDisable) {
    pAnimTrans->Bind(this, bRecursive, bDisable);
}

// UnbindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformb
void Pane::UnbindAnimation(AnimTransform *pAnimTrans, bool bRecusive) {
    this->UnbindAnimationSelf(pAnimTrans);
    if (bRecusive) {
        for (ut::LinkList<Pane, 4>::Iterator it = this->mChildList.GetBeginIter(); it != this->mChildList.GetEndIter();
             it++) {
            it->UnbindAnimation(pAnimTrans, bRecusive);
        }
    }
}

// UnbindAllAnimation__Q34nw4r3lyt4PaneFb
void Pane::UnbindAllAnimation(bool bRecursive) {
    UnbindAnimation(nullptr, bRecursive);
}

// UnbindAnimationSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform
void Pane::UnbindAnimationSelf(AnimTransform *pAnimTrans) {
    if (this->mpMaterial) {
        this->mpMaterial->UnbindAnimation(pAnimTrans);
    }
    detail::UnbindAnimationLink(&this->mAnimList, pAnimTrans);
}

// AddAnimationLink__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimationLink
void Pane::AddAnimationLink(AnimationLink *pAnimationLink) {
    this->mAnimList.PushBack(pAnimationLink);
}

// FindAnimationLinkSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform
AnimationLink *Pane::FindAnimationLinkSelf(AnimTransform *pAnimTrans) {
    return detail::FindAnimationLink(&this->mAnimList, pAnimTrans);
}

// FindAnimationLinkSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResource
AnimationLink *Pane::FindAnimationLinkSelf(const AnimResource &animRes) {
    return detail::FindAnimationLink(&this->mAnimList, animRes);
}

// SetAnimationEnable__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb
void Pane::SetAnimationEnable(AnimTransform *pAnimTrans, bool bEnable, bool bRecursive) {
    AnimationLink *pAnimLink = FindAnimationLinkSelf(pAnimTrans);
    if (pAnimLink) {
        pAnimLink->SetEnable(bEnable);
    }

    u8 materialNum = GetMaterialNum();
    for (u8 i = 0; i < materialNum; i++) {
        GetMaterial(i)->SetAnimationEnable(pAnimTrans, bEnable);
    }
    if (bRecursive) {
        for (ut::LinkList<Pane, 4>::Iterator it = this->mChildList.GetBeginIter(); it != this->mChildList.GetEndIter();
             it++) {
            it->SetAnimationEnable(pAnimTrans, bEnable, bRecursive);
        }
    }
}

// SetAnimationEnable__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResourcebb
void Pane::SetAnimationEnable(const AnimResource &animRes, bool bEnable, bool bRecursive) {
    AnimationLink *pAnimLink = FindAnimationLinkSelf(animRes);
    if (pAnimLink) {
        pAnimLink->SetEnable(bEnable);
    }

    u8 materialNum = GetMaterialNum();
    for (u8 i = 0; i < materialNum; i++) {
        GetMaterial(i)->SetAnimationEnable(animRes, bEnable);
    }
    if (bRecursive) {
        for (ut::LinkList<Pane, 4>::Iterator it = this->mChildList.GetBeginIter(); it != this->mChildList.GetEndIter();
             it++) {
            it->SetAnimationEnable(animRes, bEnable, bRecursive);
        }
    }
}

// LoadMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo
void Pane::LoadMtx(const DrawInfo &drawInfo) {
    math::MTX34 mtx;
    math::MTX34 *mtxPtr;
    if (drawInfo.IsMultipleViewMtxOnDraw()) {
        PSMTXConcat(drawInfo.GetViewMtx(), this->mGlbMtx, mtx);
        mtxPtr = &mtx;
    } else {
        mtxPtr = &this->mGlbMtx;
    }
    GXLoadPosMtxImm(*mtxPtr, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
}

// GetVtxPos__Q34nw4r3lyt4PaneCFv
math::VEC2 Pane::GetVtxPos() const {
    math::VEC2 basePt(0.0f, 0.0f);

    switch (this->mBasePosition % 3) {
        default: basePt.x = 0.0f; break;
        case 1:  basePt.x = -this->mSize.width / 2; break;
        case 2:  basePt.x = -this->mSize.width; break;
    }
    switch (this->mBasePosition / 3) {
        default: basePt.y = 0.0f; break;
        case 1:  basePt.y = this->mSize.height / 2; break;
        case 2:  basePt.y = this->mSize.height; break;
    }
    return basePt;
}

// GetMaterialNum__Q34nw4r3lyt4PaneCFv
u8 Pane::GetMaterialNum() const {
    if (this->mpMaterial) {
        return 1;
    } else {
        return 0;
    }
}

// GetMaterial__Q34nw4r3lyt4PaneCFv
Material *Pane::GetMaterial() const {
    return this->mpMaterial;
}

// GetMaterial__Q34nw4r3lyt4PaneCFUl
Material *Pane::GetMaterial(u32 idx) const {
    if (idx == 0) {
        return GetMaterial();
    }
    return nullptr;
}

u16 Pane::GetExtUserDataNum() const {
    if (this->mpExtUserDataList) {
        return this->mpExtUserDataList->num;
    }
    return 0;
}
const res::ExtUserData *Pane::GetExtUserData() const {
    if (this->mpExtUserDataList) {
        return detail::ConvertOffsToPtr<res::ExtUserData>(this->mpExtUserDataList, sizeof(res::ExtUserDataList));
    }
    return nullptr;
}

const res::ExtUserData *Pane::FindExtUserDataByName(const char *name) {
    const res::ExtUserData *pUserData = GetExtUserData();

    if (!pUserData) {
        return nullptr;
    }
    int i = 0;
    for (int i = 0; i < this->mpExtUserDataList->num; i++, pUserData++) {
        const char *str = pUserData->GetName();
        if (strcmp(name, str) == 0) {
            return pUserData;
        };
    }
    return nullptr;
}

} // namespace lyt

} // namespace nw4r
