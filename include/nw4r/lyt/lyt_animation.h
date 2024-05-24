#ifndef NW4R_LYT_ANIMATION_H
#define NW4R_LYT_ANIMATION_H
#include "common.h"
#include <nw4r/lyt/lyt_common.h>
#include <nw4r/lyt/lyt_resourceAccessor.h>
#include <nw4r/lyt/lyt_types.h>
#include <nw4r/ut/ut_LinkList.h>

namespace nw4r {
namespace lyt {
namespace res {
struct AnimationBlock {
    DataBlockHeader blockHeader; // at 0x00
    u16 frameSize;               // at 0x08
    bool loop;                   // at 0x0A
    u8 padding1;                 // at 0x0B
    u16 fileNum;                 // at 0x0C
    u16 animContNum;             // at 0x0E
    u32 animContOffsetsOffset;   // at 0x10
};

struct AnimationTagBlock {
    DataBlockHeader blockHeader; // at 0x00
    u16 tagOrder;                // at 0x08
    u16 groupNum;                // at 0x0A
    u32 nameOffset;              // at 0x0C
    u32 groupsOffset;            // at 0x10
    u16 startFrame;              // at 0x14
    u16 endFrame;                // at 0x16
    u8 flag;                     // at 0x18
    u8 padding[3];               // at 0x19
};

struct AnimationShareBlock {
    DataBlockHeader blockHeader; // at 0x00
    u32 animShareInfoOffset;     // at 0x04
    u16 shareNum;                // at 0x0C
    u8 padding[2];               // at 0x0E
};
} // namespace res

struct AnimResource {
    AnimResource(const void *anmResBuf) {
        // TODO
    }

    const res::BinaryFileHeader *mpFileHeader;    // at 0x00
    const res::AnimationBlock *mpResBlock;        // at 0x04
    const res::AnimationTagBlock *mpTagBlock;     // at 0x08
    const res::AnimationShareBlock *mpShareBlock; // at 0x0C
};

namespace detail {
struct AnimPaneTree {
    AnimPaneTree(Pane *pTargetPane, const AnimResource &animRes) : mAnimRes(animRes) {
        // TODO
    }

    bool IsEnabled() const {
        // TODO
        return false;
    }

    AnimResource mAnimRes; // at 0x00
    u16 mAnimPaneIdx;      // at 0x10
    u16 mLinkNum;          // at 0x12
    u16 mAnimMatIdx[9];    // at 0x14
    u8 mAnimMatCnt;        // at 0x26
};
} // namespace detail
struct AnimationGroupRef {
    char name[17]; // at 0x00
    u8 flag;       // at 0x11
    u8 padding[2]; // at 0x12
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

    ut::LinkListNode mLink;     // at 0x4
    res::AnimationBlock *mpRes; // at 0xC
    f32 mFrame;                 // at 0x10
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
