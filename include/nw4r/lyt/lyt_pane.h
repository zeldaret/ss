#ifndef NW4R_LYT_PANE_H
#define NW4R_LYT_PANE_H
#include "common.h"
#include "nw4r/lyt/lyt_animation.h"
#include "nw4r/lyt/lyt_resources.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/math/math_types.h"
#include "nw4r/ut/ut_Color.h"
#include "nw4r/ut/ut_LinkList.h"
#include "nw4r/ut/ut_RuntimeTypeInfo.h"
#include "nw4r/ut/ut_algorithm.h"
#include "string.h"


namespace nw4r {
namespace lyt {
namespace detail {
struct PaneBase {
    PaneBase();
    virtual ~PaneBase();
    ut::LinkListNode mLink; // at 0x4
};

} // namespace detail

class Pane : detail::PaneBase {
public:
    // For use with TestBit<Uc>
    enum PaneBits {
        VISIBLE,
        INFLUENCED_ALPHA,
        LOCATION_ADJUST
    };

    bool IsVisible() const {
        return detail::TestBit<u8>(mFlag, VISIBLE);
    }
    void SetVisible(bool bVisible) {
        detail::SetBit(&mFlag, VISIBLE, bVisible);
    }
    bool IsInfluencedAlpha() const {
        return detail::TestBit<u8>(mFlag, INFLUENCED_ALPHA);
    }
    void SetInfluencedAlpha(bool bInfluenced) {
        detail::SetBit(&mFlag, INFLUENCED_ALPHA, bInfluenced);
    }
    bool IsLocationAdjust() const {
        return detail::TestBit<u8>(mFlag, LOCATION_ADJUST);
    }
    void SetLocationAdjust(bool bAdjust) {
        detail::SetBit(&mFlag, LOCATION_ADJUST, bAdjust);
    }
    bool IsUserAllocated() const {
        return mbUserAllocated;
    }

    ut::LinkList<Pane, 4> *GetChildList() {
        return &mChildList;
    }
    ut::LinkList<AnimationLink, 0> *GetAnimationList() {
        return &mAnimList;
    }

    const math::VEC3 &GetTranslate() const {
        return mTranslate;
    }

    void SetTranslate(const nw4r::math::VEC3 &value) {
        mTranslate = value;
    }

    const math::MTX34 &GetGlobalMtx() const {
        return mGlbMtx;
    }
    Pane *GetParent() const {
        return mpParent;
    }

    const math::VEC2 &GetScale() const {
        return mScale;
    }
    void SetScale(const math::VEC2 &value) {
        mScale = value;
    }

    u8 GetAlpha() const {
        return mAlpha;
    }
    void SetAlpha(u8 value) {
        mAlpha = value;
    }

    u8 GetGlobalAlpha() const {
        return mGlbAlpha;
    }

    const Size &GetSize() const {
        return mSize;
    }
    void SetSize(const Size &value) {
        mSize = value;
    }

    void SetSRTElement(u32 idx, f32 f) {
        ((f32 *)&mTranslate)[idx] = f;
    }

    const res::ExtUserDataList *GetExtUserDataList() const {
        return mpExtUserDataList;
    }
    void SetExtUserDataList(const res::ExtUserDataList *pBlock) {
        mpExtUserDataList = pBlock;
    }

    const char *GetName() const {
        return mName;
    }

    Pane();
    Pane(const res::Pane *pBlock);
    void Init();
    void SetName(const char *pName);
    void SetUserData(const char *pData);
    void AppendChild(Pane *pChild);
    void PrependChild(Pane *pChild);
    void InsertChild(Pane *at, Pane *pChild);
    void InsertChild(ut::LinkList<Pane, 4>::Iterator next, Pane *pChild);
    void RemoveChild(Pane *pChild);
    ut::Rect GetPaneRect() const;
    ut::Rect GetPaneRect(const DrawInfo &) const;
    void AddAnimationLink(AnimationLink *pAnimationLink);
    math::VEC2 GetVtxPos() const;
    void CalculateMtxChild(const DrawInfo &drawInfo);
    u16 GetExtUserDataNum() const;
    const res::ExtUserData *GetExtUserData() const;
    const res::ExtUserData *FindExtUserDataByName(const char *name);

    virtual ~Pane();                                                                             // at 0x8
    NW4R_UT_RTTI_DECL(Pane);                                                                     // at 0x0C
    virtual void CalculateMtx(const DrawInfo &drawInfo);                                         // at 0x10
    virtual void Draw(const DrawInfo &drawInfo);                                                 // at 0x14
    virtual void DrawSelf(const DrawInfo &drawInfo);                                             // at 0x18
    virtual void Animate(u32);                                                                   // at 0x1C
    virtual void AnimateSelf(u32);                                                               // at 0x20
    virtual ut::Color GetVtxColor(u32 idx) const;                                                // at 0x24
    virtual void SetVtxColor(u32, ut::Color);                                                    // at 0x28
    virtual u8 GetColorElement(u32 idx) const;                                                   // at 0x2C
    virtual void SetColorElement(u32 idx, u8 value);                                             // at 0x30
    virtual u8 GetVtxColorElement(u32 idx) const;                                                // at 0x34
    virtual void SetVtxColorElement(u32 idx, u8 value);                                          // at 0x38
    virtual Pane *FindPaneByName(const char *findName, bool bRecursive);                         // at 0x3C
    virtual Material *FindMaterialByName(const char *findName, bool bRecursive);                 // at 0x40
    virtual void BindAnimation(AnimTransform *pAnimTrans, bool bRecursive, bool bDisable);       // at 0x44
    virtual void UnbindAnimation(AnimTransform *pAnimTrans, bool bRecusive);                     // at 0x48
    virtual void UnbindAllAnimation(bool bRecursive);                                            // at 0x4C
    virtual void UnbindAnimationSelf(AnimTransform *pAnimTrans);                                 // at 0x50
    virtual AnimationLink *FindAnimationLinkSelf(AnimTransform *pAnimTrans);                     // at 0x54
    virtual AnimationLink *FindAnimationLinkSelf(const AnimResource &animRes);                   // at 0x58
    virtual void SetAnimationEnable(AnimTransform *pAnimTrans, bool bEnable, bool bRecursive);   // at 0x5C
    virtual void SetAnimationEnable(const AnimResource &animRes, bool bEnable, bool bRecursive); // at 0x60
    virtual u8 GetMaterialNum() const;                                                           // at 0x64
    virtual Material *GetMaterial() const;                                                       // at 0x68
    virtual Material *GetMaterial(u32 idx) const;                                                // at 0x6C
    virtual void LoadMtx(const DrawInfo &drawInfo);                                              // at 0x70

protected:
    Pane *mpParent;                                // at 0x0C
    ut::LinkList<Pane, 4> mChildList;              // at 0x10
    ut::LinkList<AnimationLink, 0> mAnimList;      // at 0x1C
    Material *mpMaterial;                          // at 0x28
    math::VEC3 mTranslate;                         // at 0x2C
    math::VEC3 mRotate;                            // at 0x38
    math::VEC2 mScale;                             // at 0x44
    Size mSize;                                    // at 0x4C
    math::MTX34 mMtx;                              // at 0x54
    math::MTX34 mGlbMtx;                           // at 0x84
    const res::ExtUserDataList *mpExtUserDataList; // at 0xB4
    u8 mAlpha;                                     // at 0xB8
    u8 mGlbAlpha;                                  // at 0xB9
    u8 mBasePosition;                              // at 0xBA
    u8 mFlag;                                      // at 0xBB
    char mName[PANE_NAME_SIZE + 1];                // at 0xBC
    char mUserData[PANE_USERDATA_SIZE + 1];        // at 0xCD
    bool mbUserAllocated;                          // at 0xD6
    u8 mPadding;                                   // at 0xD7
};
} // namespace lyt
} // namespace nw4r

#endif
