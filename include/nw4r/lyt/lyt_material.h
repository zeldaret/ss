#ifndef NW4R_LYT_MATERIAL_H
#define NW4R_LYT_MATERIAL_H
#include "nw4r/lyt/lyt_animation.h"
#include "nw4r/ut/ut_LinkList.h"
#include <nw4r/ut/ut_Color.h>
#include <rvl/GX.h>

#define MATERIAL_NAME_SIZE 20

namespace nw4r {
namespace lyt {

namespace res {
struct MaterialList {
    DataBlockHeader blockHeader; // at 0x00
    u16 materialNum;             // at 0x08
    u8 padding[2];               // at 0x0A
};

} // namespace res

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
    // IsBlendModeCap__Q34nw4r3lyt8MaterialCFv
    // IsAlphaCompareCap__Q34nw4r3lyt8MaterialCFv
    // IsTevSwapCap__Q34nw4r3lyt8MaterialCFv
    // IsMatColorCap__Q34nw4r3lyt8MaterialCFv
    // IsChanCtrlCap__Q34nw4r3lyt8MaterialCFv
    // SetTexSRTElement__Q34nw4r3lyt8MaterialFUlUlf
    // GetTexturePtr__Q34nw4r3lyt8MaterialFUc
    // GetTexSRTCap__Q34nw4r3lyt8MaterialCFv
    // GetIndTexSRTCap__Q34nw4r3lyt8MaterialCFv
    // SetIndTexSRTElement__Q34nw4r3lyt8MaterialFUlUlf

    ut::LinkList<AnimationLink, 0> *GetAnimationList() {
        return &mAnimList;
    }
    bool IsUserAllocated() const {
        return mbUserAllocated;
    }
    const char *GetName() const {
        return mName;
    }

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

private:
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
