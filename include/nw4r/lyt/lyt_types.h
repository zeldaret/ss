#ifndef NW4R_LYT_TYPES_H
#define NW4R_LYT_TYPES_H

#include <nw4r/ut/ut_LinkList.h>
#include <nw4r/ut/ut_algorithm.h>

namespace nw4r {
namespace lyt {

namespace detail {
template <typename T>
inline bool TestBit(T bits, int index) {
    T mask = 1 << index;
    return bits & mask;
}
template <typename T>
inline void SetBit(T *bits, int pos, bool val) {
    T mask = T(1 << pos);
    *bits = T((*bits & ~mask)) | (val << pos);
}

template <typename T>
T *ConvertOffsToPtr(const void *baseAddress, unsigned int offset) {
    return (T *)((u32)baseAddress + offset);
}
} // namespace detail
struct Size {
    Size() : width(), height() {}

    Size(f32 w, f32 h) : width(w), height(h) {}

    Size(const Size &r) {
        width = r.width;
        height = r.height;
    }

    f32 width, height;
};

inline bool operator==(const Size &a, const Size &b) {
    return (a.width == b.width && a.height == b.height);
}
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
struct AnimationShareInfo {
    const char *GetTargetGroupName() const {
        return targetGroupName;
    }
    const char *GetSrcPaneName() const {
        return srcPaneName;
    }

    char srcPaneName[17];     // at 0x00
    char targetGroupName[17]; // at 0x11
    u8 padding[2];            // at 0x12
};

class AnimationLink {
public:
    inline AnimationLink() : mLink() {
        Reset();
    }

    AnimTransform *GetAnimTransform() const {
        return mAnimTrans;
    }
    u16 GetIndex() const {
        return mIdx;
    }
    void SetIndex(u16 val) {
        mIdx = val;
    }

    bool IsEnable() const {
        return !mbDisable;
    }
    void SetEnable(bool enable) {
        mbDisable = !enable;
    }

    void Reset() {
        Set(NULL, 0, false);
    }

    void Set(AnimTransform *pTransform, u16 us, bool b) {
        mAnimTrans = pTransform;
        mIdx = us;
        mbDisable = b;
    }

private:
    ut::LinkListNode mLink;    // at 0x0
    AnimTransform *mAnimTrans; // at 0x8
    u16 mIdx;                  // at 0xC
    bool mbDisable;            // at 0xE
};

struct AnimationGroupRef {
    const char *GetName() const {
        return name;
    }

    char name[17]; // at 0x00
    u8 flag;       // at 0x11
    u8 padding[2]; // at 0x12
};

struct TexCoordGen {
    // __ct__Q34nw4r3lyt11TexCoordGenFv
    TexCoordGen() {
        reserve = 0;
        Set(GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX_IDENT);
    }

    // Set__Q34nw4r3lyt11TexCoordGenF13_GXTexGenType12_GXTexGenSrcUl
    void Set(GXTexGenType aTexGenType, GXTexGenSrc aTexGenSrc, u32 aTexMtx) {
        texGenType = aTexGenType;
        texGenSrc = aTexGenSrc;
        texMtx = aTexMtx;
    }

    // GetTexGenType__Q34nw4r3lyt11TexCoordGenCFv
    u8 GetTexGenType() const {
        return texGenType;
    }

    // GetTexGenSrc__Q34nw4r3lyt11TexCoordGenCFv
    u8 GetTexGenSrc() const {
        return texGenSrc;
    }

    // GetTexMtx__Q34nw4r3lyt11TexCoordGenCFv
    u8 GetTexMtx() const {
        return texMtx;
    }

    u8 texGenType; // at 0x0
    u8 texGenSrc;  // at 0x1
    u8 texMtx;     // at 0x2
    u8 reserve;    // at 0x3
};
struct ChanCtrl { // 17552
    // __ct__Q34nw4r3lyt8ChanCtrlFv
    // Set__Q34nw4r3lyt8ChanCtrlF11_GXColorSrc11_GXColorSrc
    // GetColorSrc__Q34nw4r3lyt8ChanCtrlCFv
    // GetAlphaSrc__Q34nw4r3lyt8ChanCtrlCFv

    u8 matSrcCol; // at 0x0
    u8 matSrcAlp; // at 0x1
    u8 reserve1;  // at 0x2
    u8 reserve2;  // at 0x3
};

struct BitGXNums {
    u32 texMap : 4;
    u32 texSRT : 4;
    u32 texCoordGen : 4;
    u32 indSRT : 2;
    u32 indStage : 3;
    u32 tevSwap : 1;
    u32 tevStage : 5;
    u32 chanCtrl : 1;
    u32 matCol : 1;
    u32 alpComp : 1;
    u32 blendMode : 1;
};
struct BlendMode { // 10c41
    // __ct__Q34nw4r3lyt9BlendModeFv
    // Set__Q34nw4r3lyt9BlendModeF12_GXBlendMode14_GXBlendFactor14_GXBlendFactor10_GXLogicOp
    // GetType__Q34nw4r3lyt9BlendModeCFv
    // GetSrcFactor__Q34nw4r3lyt9BlendModeCFv
    // GetDstFactor__Q34nw4r3lyt9BlendModeCFv
    // GetOp__Q34nw4r3lyt9BlendModeCFv

    u8 type;      // at 0x0
    u8 srcFactor; // at 0x1
    u8 dstFactor; // at 0x2
    u8 op;        // at 0x3
};

struct AlphaCompare { // 17457
    // __ct__Q34nw4r3lyt12AlphaCompareFv
    // Set__Q34nw4r3lyt12AlphaCompareF10_GXCompareUc10_GXAlphaOp10_GXCompareUc
    // GetComp0__Q34nw4r3lyt12AlphaCompareCFv
    // GetRef0__Q34nw4r3lyt12AlphaCompareCFv
    // GetOp__Q34nw4r3lyt12AlphaCompareCFv
    // GetComp1__Q34nw4r3lyt12AlphaCompareCFv
    // GetRef1__Q34nw4r3lyt12AlphaCompareCFv

    u8 comp; // at 0x0
    u8 op;   // at 0x1
    u8 ref0; // at 0x2
    u8 ref1; // at 0x3
};
struct IndirectStage { // 172da
    // __ct__Q34nw4r3lyt13IndirectStageFv
    // Set__Q34nw4r3lyt13IndirectStageF13_GXTexCoordID11_GXTexMapID14_GXIndTexScale14_GXIndTexScale
    // GetTexCoordGen__Q34nw4r3lyt13IndirectStageCFv
    // GetTexMap__Q34nw4r3lyt13IndirectStageCFv
    // GetScaleS__Q34nw4r3lyt13IndirectStageCFv
    // GetScaleT__Q34nw4r3lyt13IndirectStageCFv

    u8 texCoordGen; // at 0x0
    u8 texMap;      // at 0x1
    u8 scaleS;      // at 0x2
    u8 scaleT;      // at 0x3
};

struct TexSRT { // 17243

    math::VEC2 translate; // at 0x0
    f32 rotate;           // at 0x8
    math::VEC2 scale;     // at 0xC
};

struct TevStageInOp { // 16fe7
    // GetA__Q34nw4r3lyt12TevStageInOpCFv
    // GetB__Q34nw4r3lyt12TevStageInOpCFv
    // GetC__Q34nw4r3lyt12TevStageInOpCFv
    // GetD__Q34nw4r3lyt12TevStageInOpCFv
    // GetOp__Q34nw4r3lyt12TevStageInOpCFv
    // GetBias__Q34nw4r3lyt12TevStageInOpCFv
    // GetScale__Q34nw4r3lyt12TevStageInOpCFv
    // IsClamp__Q34nw4r3lyt12TevStageInOpCFv
    // GetOutReg__Q34nw4r3lyt12TevStageInOpCFv
    // GetKSel__Q34nw4r3lyt12TevStageInOpCFv
    // SetOp__Q34nw4r3lyt12TevStageInOpFUcUcUcbUcUc

    u8 ab; // at 0x0
    u8 cd; // at 0x1
    u8 op; // at 0x2
    u8 cl; // at 0x3
};
struct TevStage { // 17094
    // __ct__Q34nw4r3lyt8TevStageFv
    // GetTexCoordGen__Q34nw4r3lyt8TevStageCFv
    // GetTexMap__Q34nw4r3lyt8TevStageCFv
    // GetColorChan__Q34nw4r3lyt8TevStageCFv
    // GetRasSwapSel__Q34nw4r3lyt8TevStageCFv
    // GetTexSwapSel__Q34nw4r3lyt8TevStageCFv
    // GetColorInA__Q34nw4r3lyt8TevStageCFv
    // GetColorInB__Q34nw4r3lyt8TevStageCFv
    // GetColorInC__Q34nw4r3lyt8TevStageCFv
    // GetColorInD__Q34nw4r3lyt8TevStageCFv
    // GetColorOp__Q34nw4r3lyt8TevStageCFv
    // GetColorBias__Q34nw4r3lyt8TevStageCFv
    // GetColorScale__Q34nw4r3lyt8TevStageCFv
    // IsColorClamp__Q34nw4r3lyt8TevStageCFv
    // GetColorOutReg__Q34nw4r3lyt8TevStageCFv
    // GetKColorSel__Q34nw4r3lyt8TevStageCFv
    // GetAlphaInA__Q34nw4r3lyt8TevStageCFv
    // GetAlphaInB__Q34nw4r3lyt8TevStageCFv
    // GetAlphaInC__Q34nw4r3lyt8TevStageCFv
    // GetAlphaInD__Q34nw4r3lyt8TevStageCFv
    // GetAlphaOp__Q34nw4r3lyt8TevStageCFv
    // GetAlphaBias__Q34nw4r3lyt8TevStageCFv
    // GetAlphaScale__Q34nw4r3lyt8TevStageCFv
    // IsAlphaClamp__Q34nw4r3lyt8TevStageCFv
    // GetAlphaOutReg__Q34nw4r3lyt8TevStageCFv
    // GetKAlphaSel__Q34nw4r3lyt8TevStageCFv
    // GetIndMtxSel__Q34nw4r3lyt8TevStageCFv
    // GetIndStage__Q34nw4r3lyt8TevStageCFv
    // GetIndFormat__Q34nw4r3lyt8TevStageCFv
    // GetIndBiasSel__Q34nw4r3lyt8TevStageCFv
    // GetIndWrapS__Q34nw4r3lyt8TevStageCFv
    // GetIndWrapT__Q34nw4r3lyt8TevStageCFv
    // IsIndAddPrev__Q34nw4r3lyt8TevStageCFv
    // IsIndUtcLod__Q34nw4r3lyt8TevStageCFv
    // GetIndAlphaSel__Q34nw4r3lyt8TevStageCFv
    // SetOrder__Q34nw4r3lyt8TevStageF13_GXTexCoordID11_GXTexMapID12_GXChannelID13_GXTevSwapSel13_GXTevSwapSel
    // SetColorIn__Q34nw4r3lyt8TevStageF14_GXTevColorArg14_GXTevColorArg14_GXTevColorArg14_GXTevColorArg
    // SetAlphaIn__Q34nw4r3lyt8TevStageF14_GXTevAlphaArg14_GXTevAlphaArg14_GXTevAlphaArg14_GXTevAlphaArg
    // SetColorOp__Q34nw4r3lyt8TevStageF8_GXTevOp10_GXTevBias11_GXTevScaleb11_GXTevRegID15_GXTevKColorSel
    // SetAlphaOp__Q34nw4r3lyt8TevStageF8_GXTevOp10_GXTevBias11_GXTevScaleb11_GXTevRegID15_GXTevKAlphaSel
    // SetIndirect__Q34nw4r3lyt8TevStageF16_GXIndTexStageID15_GXIndTexFormat16_GXIndTexBiasSel14_GXIndTexMtxID13_GXIndTexWrap13_GXIndTexWrapbb17_GXIndTexAlphaSel

    u8 texCoordGen;     // at 0x0
    u8 colChan;         // at 0x1
    u8 texMap;          // at 0x2
    u8 swapSel;         // at 0x3
    TevStageInOp colIn; // at 0x4
    TevStageInOp alpIn; // at 0x8
    u8 indStage;        // at 0xC
    u8 inBiMt;          // at 0xD
    u8 indWrap;         // at 0xE
    u8 indFoAdUtAl;     // at 0xF
};

struct TevSwapMode { // 1750a

    // GetR__Q34nw4r3lyt11TevSwapModeCFv
    // GetG__Q34nw4r3lyt11TevSwapModeCFv
    // GetB__Q34nw4r3lyt11TevSwapModeCFv
    // GetA__Q34nw4r3lyt11TevSwapModeCFv
    // Set__Q34nw4r3lyt11TevSwapModeF15_GXTevColorChan15_GXTevColorChan15_GXTevColorChan15_GXTevColorChan

    u8 swap; // at 0x0
};

} // namespace lyt

} // namespace nw4r

#endif
