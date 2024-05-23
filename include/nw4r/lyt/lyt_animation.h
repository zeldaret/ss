#ifndef NW4R_LYT_ANIMATION_H
#define NW4R_LYT_ANIMATION_H
#include "common.h"
#include "nw4r/lyt/lyt_common.h"
#include "nw4r/lyt/lyt_resourceAccessor.h"
#include "nw4r/ut/ut_LinkList.h"

namespace nw4r {
namespace lyt {
namespace res {
struct AnimationBlock {};
} // namespace res

struct AnimationBlock {
    res::DataBlockHeader header; // at 0x0
    u16 frameSize;               // at 0x8
    bool loop;                   // at 0xA
    // . . .
};

struct AnimTransform {
    inline AnimTransform() : mLink(), mpRes(NULL), mFrame(0.0f) {}

    u16 GetFrameSize() const;
    bool IsLoopData() const;
    virtual ~AnimTransform() = 0;
    virtual void SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor) = 0;
    virtual void SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor, u16 animNum) = 0;
    virtual void Bind(Pane *pPane, bool bRecursive) = 0;
    virtual void Bind(Material *pMaterial, bool bDisable) = 0;
    virtual void Animate(u32 idx, Pane *pPane) = 0;
    virtual void Animate(u32 idx, Material *pMaterial) = 0;

    ut::LinkListNode mLink; // at 0x0
    AnimationBlock *mpRes;  // at 0xC
    f32 mFrame;             // at 0x10
};

struct AnimationLink {
    inline AnimationLink() : mLink() {
        Reset();
    }

    inline void Reset() {
        Set(NULL, 0, false);
    }

    inline void Set(AnimTransform *pTransform, u16 us, bool b) {
        mAnimTrans = pTransform;
        mIdx = us;
        mbDisable = b;
    }

    ut::LinkListNode mLink;    // at 0x0
    AnimTransform *mAnimTrans; // at 0x8
    u16 mIdx;                  // at 0xC
    bool mbDisable;            // at 0xE
};
struct AnimTransformBasic : AnimTransform {
    AnimTransformBasic();
    virtual ~AnimTransformBasic();                                                                          // at 0x08
    virtual void SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor);              // at 0x0C
    virtual void SetResource(const res::AnimationBlock *pRes, ResourceAccessor *pResAccessor, u16 animNum); // at 0x10
    virtual void Bind(Pane *pPane, bool bRecursive);                                                        // at 0x14
    virtual void Bind(Material *pMaterial, bool bDisable);                                                  // at 0x18
    virtual void Animate(u32 idx, Pane *pPane);                                                             // at 0x1C
    virtual void Animate(u32 idx, Material *pMaterial);                                                     // at 0x20

    void *mpFileResAry;          // at 0x14
    AnimationLink *mAnimLinkAry; // at 0x18
    u16 mAnimLinkNum;            // at 0x1C
};

namespace detail {
void UnbindAnimationLink(ut::LinkList<AnimationLink, 0> *, AnimTransform *);
AnimationLink *FindAnimationLink(ut::LinkList<AnimationLink, 0> *, AnimTransform *);
AnimationLink *FindAnimationLink(ut::LinkList<AnimationLink, 0> *, const AnimResource &);
void UnbindAnimationLink(ut::LinkList<AnimationLink, 0> *, AnimTransform *);
} // namespace detail

} // namespace lyt
} // namespace nw4r

#endif
