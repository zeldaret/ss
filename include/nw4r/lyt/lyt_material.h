#ifndef NW4R_LYT_MATERIAL_H
#define NW4R_LYT_MATERIAL_H
#include "nw4r/lyt/lyt_animation.h"
#include "nw4r/ut/ut_LinkList.h"
#include <nw4r/ut/ut_Color.h>
#include <rvl/GX.h>

#define MATERIAL_NAME_SIZE 20

namespace nw4r {
namespace lyt {

struct BitGXNums {
    union {
        u32 texSRT;
        u32 texCoordGen;
        u32 indSRT;
        u32 indStage;
        u32 tevSwap;
        u32 tevStage;
        u32 chanCtrl;
        u32 matCol;
        u32 alpComp;
        u32 blendMode;
    };
};

struct Material {
    // SetupGX__Q34nw4r3lyt8MaterialFbUc
    // BindAnimation__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform
    // UnbindAnimation__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform
    // UnbindAllAnimation__Q34nw4r3lyt8MaterialFv
    // Animate__Q34nw4r3lyt8MaterialFv
    // FindAnimationLink__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform
    // FindAnimationLink__Q34nw4r3lyt8MaterialFRCQ34nw4r3lyt12AnimResource
    // SetAnimationEnable__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransformb
    // SetAnimationEnable__Q34nw4r3lyt8MaterialFRCQ34nw4r3lyt12AnimResourceb

    virtual ~Material();                                                        // at 0x08
    virtual bool SetupGX(bool bModVtxCok, u8 alpha);                            // at 0x0C
    virtual void BindAnimation(AnimTransform *pAnimTrans);                      // at 0x10
    virtual void UnbindAnimation(AnimTransform *pAnimTrans);                    // at 0x14
    virtual void UnbindAllAnimation();                                          // at 0x18
    virtual void Animate();                                                     // at 0x1C
    virtual AnimationLink *FindAnimationLink(AnimTransform *pAnimTrans);        // at 0x20
    virtual AnimationLink *FindAnimationLink(const AnimResource &animRes);      // at 0x24
    virtual void SetAnimationEnable(AnimTransform *pAnimTrans, bool bEnable);   // at 0x28
    virtual void SetAnimationEnable(const AnimResource &animRes, bool bEnable); // at 0x2C

    ut::LinkList<AnimationLink, 0> mAnimList; // at 0x4
    GXColorS10 mTevCols[3];                   // at 0x10
    ut::Color mTevKCols[4];                   // at 0x28
    BitGXNums mGXMemCap;                      // at 0x38
    BitGXNums mGXMemNum;                      // at 0x3C
    void *mpGXMem;                            // at 0x40
    char mName[MATERIAL_NAME_SIZE + 1];       // at 0x44
    bool mbUserAllocated;                     // at 0x59
    u8 mPadding[2];                           // at 0x5A
};
} // namespace lyt
} // namespace nw4r

#endif
