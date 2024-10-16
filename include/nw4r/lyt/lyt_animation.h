#ifndef NW4R_LYT_ANIMATION_H
#define NW4R_LYT_ANIMATION_H
#include "common.h"
#include "nw4r/lyt/lyt_common.h"
#include "nw4r/lyt/lyt_material.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_resourceAccessor.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/ut/ut_LinkList.h"

namespace nw4r {
namespace lyt {
class AnimTransform {
public:
    AnimTransform();

    u16 GetFrameSize() const;
    bool IsLoopData() const;
    virtual ~AnimTransform() = 0;                                                                  // at 0x08
    virtual void SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor) = 0; // at 0x0C
    virtual void SetResource(
        const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor, //
        u16 animNum
    ) = 0;                                                              // at 0x10
    virtual void Bind(Pane *pPane, bool bRecursive, bool bDisable) = 0; // at 0x14
    virtual void Bind(Material *pMaterial, bool bDisable) = 0;          // at 0x18
    virtual void Animate(u32 idx, Pane *pPane) = 0;                     // at 0x1C
    virtual void Animate(u32 idx, Material *pMaterial) = 0;             // at 0x20

    f32 GetFrameMax() const {
        return mFrame; // TOOD
    }

    const res::AnimationBlock *GetAnimResource() const {
        return mpRes;
    }
    f32 GetFrame() const {
        return mFrame;
    }
    void SetFrame(f32 frame) {
        mFrame = frame;
    }

    ut::LinkListNode mLink; // at 0x4
protected:
    const res::AnimationBlock *mpRes; // at 0xC
    f32 mFrame;                       // at 0x10
};

class AnimResource {
public:
    AnimResource();
    AnimResource(const void *anmResBuf) {
        Set(anmResBuf);
    }
    void Set(const void *anmResBuf);
    void Init();
    u16 GetGroupNum() const;
    const AnimationGroupRef *GetGroupArray() const;
    bool IsDescendingBind() const;
    u16 GetAnimationShareInfoNum() const;
    const AnimationShareInfo *GetAnimationShareInfoArray() const;
    u16 CalcAnimationNum(Pane *pPane, bool bRecursive) const;
    u16 CalcAnimationNum(Group *pGroup, bool bRecursive) const;

    const res::AnimationBlock *GetResourceBlock() const {
        return mpResBlock;
    }
    const res::AnimationTagBlock *GetTagBlock() const {
        return mpTagBlock;
    }
    const res::AnimationShareBlock *GetShareBlock() const {
        return mpShareBlock;
    }

private:
    const res::BinaryFileHeader *mpFileHeader;    // at 0x00
    const res::AnimationBlock *mpResBlock;        // at 0x04
    const res::AnimationTagBlock *mpTagBlock;     // at 0x08
    const res::AnimationShareBlock *mpShareBlock; // at 0x0C
};

class AnimTransformBasic : public AnimTransform {
public:
    AnimTransformBasic();
    AnimationLink *FindUnbindLink(AnimationLink *pLink) const;

    virtual ~AnimTransformBasic();                                                                          // at 0x08
    virtual void SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor);              // at 0x0C
    virtual void SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor, u16 animNum); // at 0x10
    virtual void Bind(Pane *pPane, bool bRecursive, bool bDisable);                                         // at 0x14
    virtual void Bind(Material *pMaterial, bool bDisable);                                                  // at 0x18
    virtual void Animate(u32 idx, Pane *pPane);                                                             // at 0x1C
    virtual void Animate(u32 idx, Material *pMaterial);                                                     // at 0x20

    template <typename T>
    AnimationLink *Bind(T *pTarget, AnimationLink *pAnimLink, u16 idx, bool bDisable);

    AnimationLink *GetAnimLinkAry() const {
        return mAnimLinkAry;
    }

protected:
    void **mpFileResAry;         // at 0x14
    AnimationLink *mAnimLinkAry; // at 0x18
    u16 mAnimLinkNum;            // at 0x1C
};

namespace detail {
class AnimPaneTree {
public:
    AnimPaneTree(Pane *pTargetPane, const AnimResource &animRes) : mAnimRes() {
        Init();
        Set(pTargetPane, animRes);
    }
    bool IsEnabled() const {
        // TODO
        return false;
    }
    static u16 FindAnimContent(const res::AnimationBlock *pAnimBlock, const char *animContName, u8 animContType);
    void Init();
    void Set(Pane *pTargetPane, const AnimResource &animRes);
    AnimTransform *Bind(Layout *pLayout, Pane *pTargetPane, ResourceAccessor *pResAccessor) const;

    // Inline Names Guess
    u16 GetLinkNum() const {
        return mLinkNum;
    }

private:
    AnimResource mAnimRes; // at 0x00
    u16 mAnimPaneIdx;      // at 0x10
    u16 mLinkNum;          // at 0x12
    u16 mAnimMatIdx[9];    // at 0x14
    u8 mAnimMatCnt;        // at 0x26
};

AnimationLink *FindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, AnimTransform *pAnimTrans);
AnimationLink *FindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, const AnimResource &animRes);
void UnbindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, AnimTransform *pAnimTrans);
void UnbindAnimationLink(ut::LinkList<AnimationLink, 0> *pAnimList, const AnimResource &animRes);

} // namespace detail

} // namespace lyt
} // namespace nw4r

#endif
