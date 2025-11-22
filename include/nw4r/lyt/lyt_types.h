#ifndef NW4R_LYT_TYPES_H
#define NW4R_LYT_TYPES_H

#include "nw4r/ut/ut_LinkList.h"
#include "nw4r/ut/ut_algorithm.h"

#include "nw4r/math.h" // IWYU pragma: export

#include "rvl/GX.h" // IWYU pragma: export

namespace nw4r {
namespace lyt {

namespace detail {
struct BitGXNums {
    u32 texMap : 4;      // ( >> 28 ) &  F
    u32 texSRT : 4;      // ( >> 24 ) &  F
    u32 texCoordGen : 4; // ( >> 20 ) &  F
    u32 indSRT : 2;      // ( >> 18 ) &  3
    u32 indStage : 3;    // ( >> 15 ) &  7
    u32 tevSwap : 1;     // ( >> 14 ) &  1
    u32 tevStage : 5;    // ( >>  9 ) & 1F
    u32 chanCtrl : 1;    // ( >>  8 ) &  1
    u32 matCol : 1;      // ( >>  7 ) &  1
    u32 alpComp : 1;     // ( >>  6 ) &  1
    u32 blendMode : 1;   // ( >>  5 ) &  1
};

template <typename T>
inline bool TestBit(T bits, int index) {
    T mask = 1 << index;
    return (bits & mask) != 0;
}
template <typename T>
inline void SetBit(T *bits, int pos, bool val) {
    T mask = 1 << pos;

    *bits &= ~mask;
    *bits |= val ? mask : 0;
}

template <typename T>
inline T GetBits(T bits, int pos, int len) {
    u32 mask = T(1 << pos);
    return bits & mask;
}

template <typename T>
inline T *ConvertOffsToPtr(void *baseAddress, unsigned int offset) {
    return (T *)((u32)baseAddress + offset);
}

template <typename T>
inline const T *ConvertOffsToPtr(const void *baseAddress, unsigned int offset) {
    return (const T *)((u32)baseAddress + offset);
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
        Set(GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    }

    // Set__Q34nw4r3lyt11TexCoordGenF13_GXTexGenType12_GXTexGenSrcUl
    void Set(GXTexGenType aTexGenType, GXTexGenSrc aTexGenSrc, u32 aTexMtx) {
        texGenType = aTexGenType;
        texGenSrc = aTexGenSrc;
        texMtx = aTexMtx;
    }

    // GetTexGenType__Q34nw4r3lyt11TexCoordGenCFv
    GXTexGenType GetTexGenType() const {
        return (GXTexGenType)texGenType;
    }

    // GetTexGenSrc__Q34nw4r3lyt11TexCoordGenCFv
    GXTexGenSrc GetTexGenSrc() const {
        return (GXTexGenSrc)texGenSrc;
    }

    // GetTexMtx__Q34nw4r3lyt11TexCoordGenCFv
    u32 GetTexMtx() const {
        return texMtx;
    }

    u8 texGenType; // at 0x0
    u8 texGenSrc;  // at 0x1
    u8 texMtx;     // at 0x2
    u8 reserve;    // at 0x3
};
struct ChanCtrl { // 17552
    // __ct__Q34nw4r3lyt8ChanCtrlFv
    ChanCtrl() : reserve1(0), reserve2(0) {
        Set(GX_SRC_VTX, GX_SRC_VTX);
    }

    // Set__Q34nw4r3lyt8ChanCtrlF11_GXColorSrc11_GXColorSrc
    void Set(GXColorSrc colSrc, GXColorSrc alpSrc) {
        matSrcCol = colSrc;
        matSrcAlp = alpSrc;
    }

    // GetColorSrc__Q34nw4r3lyt8ChanCtrlCFv
    GXColorSrc GetColorSrc() const {
        return (GXColorSrc)matSrcCol;
    }

    // GetAlphaSrc__Q34nw4r3lyt8ChanCtrlCFv
    GXColorSrc GetAlphaSrc() const {
        return (GXColorSrc)matSrcAlp;
    }

    u8 matSrcCol; // at 0x0
    u8 matSrcAlp; // at 0x1
    u8 reserve1;  // at 0x2
    u8 reserve2;  // at 0x3
};

struct AlphaCompare { // 17457
    // __ct__Q34nw4r3lyt12AlphaCompareFv
    AlphaCompare() {
        Set(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    }

    // Set__Q34nw4r3lyt12AlphaCompareF10_GXCompareUc10_GXAlphaOp10_GXCompareUc
    void Set(GXCompare aComp0, u8 aRef0, GXAlphaOp aOp, GXCompare aComp1, u8 aRef1) {
        comp = (aComp0 & 0x7) | ((aComp1 & 0x7) << 4);
        op = aOp;
        ref0 = aRef0;
        ref1 = aRef1;
    }

    // GetComp0__Q34nw4r3lyt12AlphaCompareCFv
    GXCompare GetComp0() const {
        return (GXCompare)((comp >> 0) & 0xF);
    }

    // GetRef0__Q34nw4r3lyt12AlphaCompareCFv
    u8 GetRef0() const {
        return ref0;
    }

    // GetOp__Q34nw4r3lyt12AlphaCompareCFv
    GXAlphaOp GetOp() const {
        return (GXAlphaOp)op;
    }

    // GetComp1__Q34nw4r3lyt12AlphaCompareCFv
    GXCompare GetComp1() const {
        return (GXCompare)((comp >> 4) & 0xF);
    }

    // GetRef1__Q34nw4r3lyt12AlphaCompareCFv
    u8 GetRef1() const {
        return ref1;
    }

    u8 comp; // at 0x0
    u8 op;   // at 0x1
    u8 ref0; // at 0x2
    u8 ref1; // at 0x3
};

struct BlendMode { // 10c41
    // __ct__Q34nw4r3lyt9BlendModeFv
    BlendMode() {
        Set(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    }

    // Set__Q34nw4r3lyt9BlendModeF12_GXBlendMode14_GXBlendFactor14_GXBlendFactor10_GXLogicOp
    void Set(GXBlendMode aType, GXBlendFactor aSrcFactor, GXBlendFactor aDestFactor, GXLogicOp aOp) {
        type = aType;
        srcFactor = aSrcFactor;
        dstFactor = aDestFactor;
        op = aOp;
    }

    // GetType__Q34nw4r3lyt9BlendModeCFv
    GXBlendMode GetType() const {
        return GXBlendMode(type);
    }

    // GetSrcFactor__Q34nw4r3lyt9BlendModeCFv
    GXBlendFactor GetSrcFactor() const {
        return (GXBlendFactor)srcFactor;
    }

    // GetDstFactor__Q34nw4r3lyt9BlendModeCFv
    GXBlendFactor GetDstFactor() const {
        return (GXBlendFactor)dstFactor;
    }

    // GetOp__Q34nw4r3lyt9BlendModeCFv
    GXLogicOp GetOp() const {
        return (GXLogicOp)op;
    }

    u8 type;      // at 0x0
    u8 srcFactor; // at 0x1
    u8 dstFactor; // at 0x2
    u8 op;        // at 0x3
};

struct IndirectStage { // 172da
    // __ct__Q34nw4r3lyt13IndirectStageFv
    IndirectStage() {
        Set(GX_TEXCOORD0, GX_TEXMAP0, GX_ITS_1, GX_ITS_1);
    }

    // Set__Q34nw4r3lyt13IndirectStageF13_GXTexCoordID11_GXTexMapID14_GXIndTexScale14_GXIndTexScale
    void Set(GXTexCoordID aTexCoordGen, GXTexMapID aTexMap, GXIndTexScale aScaleS, GXIndTexScale aScaleT) {
        texCoordGen = aTexCoordGen;
        texMap = aTexMap;
        scaleS = aScaleS;
        scaleT = aScaleT;
    }

    // GetTexCoordGen__Q34nw4r3lyt13IndirectStageCFv
    GXTexCoordID GetTexCoordGen() const {
        return (GXTexCoordID)texCoordGen;
    }

    // GetTexMap__Q34nw4r3lyt13IndirectStageCFv
    GXTexMapID GetTexMap() const {
        return (GXTexMapID)texMap;
    }

    // GetScaleS__Q34nw4r3lyt13IndirectStageCFv
    GXIndTexScale GetScaleS() const {
        return (GXIndTexScale)scaleS;
    }

    // GetScaleT__Q34nw4r3lyt13IndirectStageCFv
    GXIndTexScale GetScaleT() const {
        return (GXIndTexScale)scaleT;
    }

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
    u8 GetA() const {
        return (ab >> 0) & 0xF;
    }

    // GetB__Q34nw4r3lyt12TevStageInOpCFv
    u8 GetB() const {
        return (ab >> 4) & 0xF;
    }

    // GetC__Q34nw4r3lyt12TevStageInOpCFv
    u8 GetC() const {
        return (cd >> 0) & 0xF;
    }

    // GetD__Q34nw4r3lyt12TevStageInOpCFv
    u8 GetD() const {
        return (cd >> 4) & 0xF;
    }

    // GetOp__Q34nw4r3lyt12TevStageInOpCFv
    u8 GetOp() const {
        return (op >> 0) & 0xF;
    }

    // GetBias__Q34nw4r3lyt12TevStageInOpCFv
    u8 GetBias() const {
        return (op >> 4) & 0x3;
    }

    // GetScale__Q34nw4r3lyt12TevStageInOpCFv
    u8 GetScale() const {
        return (op >> 6) & 0x3;
    }

    // IsClamp__Q34nw4r3lyt12TevStageInOpCFv
    bool IsClamp() const {
        return (cl >> 0) & 1;
    }

    // GetOutReg__Q34nw4r3lyt12TevStageInOpCFv
    u8 GetOutReg() const {
        return (cl >> 1) & 0x3;
    }

    // GetKSel__Q34nw4r3lyt12TevStageInOpCFv
    u8 GetKSel() const {
        return (cl >> 3) & 0x1F;
    }

    // SetIn__Q34nw4r3lyt12TevStageInOpFUcUcUcUc
    void SetIn(u8 a, u8 b, u8 c, u8 d) {
        ab = (a & 0xF) | ((b & 0xF) << 4);
        cd = (c & 0xF) | ((d & 0xF) << 4);
    }

    // SetOp__Q34nw4r3lyt12TevStageInOpFUcUcUcbUcUc
    void SetOp(u8 aOp, u8 bias, u8 scale, bool clamp, u8 outReg, u8 kSel) {
        op = (aOp & 0xF) | ((bias & 3) << 4) | ((scale & 0x3) << 6);
        cl = (clamp & 1) | ((outReg & 0x3) << 1) | ((kSel & 0x1F) << 3);
    }

    u8 ab; // at 0x0
    u8 cd; // at 0x1
    u8 op; // at 0x2
    u8 cl; // at 0x3
};

struct TevStage { // 17094
    // __ct__Q34nw4r3lyt8TevStageFv
    TevStage() {
        SetOrder(GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        SetColorIn(GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
        SetAlphaIn(GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
        SetColorOp(GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV, GX_TEV_KCSEL_K0);
        SetAlphaOp(GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV, GX_TEV_KASEL_K0_R);
        SetIndirect(
            GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF, GX_ITW_OFF, GX_ITW_OFF, false, false, GX_ITBA_OFF
        );
    }

    // GetTexCoordGen__Q34nw4r3lyt8TevStageCFv
    GXTexCoordID GetTexCoordGen() const {
        return (GXTexCoordID)texCoordGen;
    }

    // GetTexMap__Q34nw4r3lyt8TevStageCFv
    GXTexMapID GetTexMap() const {
        return (GXTexMapID)(texMap | (swapSel & 1) << 8);
    }

    // GetColorChan__Q34nw4r3lyt8TevStageCFv
    GXChannelID GetColorChan() const {
        return (GXChannelID)colChan;
    }

    // GetRasSwapSel__Q34nw4r3lyt8TevStageCFv
    GXTevSwapSel GetRasSwapSel() const {
        return (GXTevSwapSel)((swapSel >> 1) & 3);
    }

    // GetTexSwapSel__Q34nw4r3lyt8TevStageCFv
    GXTevSwapSel GetTexSwapSel() const {
        return (GXTevSwapSel)((swapSel >> 3) & 3);
    }

    // GetColorInA__Q34nw4r3lyt8TevStageCFv
    GXTevColorArg GetColorInA() const {
        return (GXTevColorArg)colIn.GetA();
    }

    // GetColorInB__Q34nw4r3lyt8TevStageCFv
    GXTevColorArg GetColorInB() const {
        return (GXTevColorArg)colIn.GetB();
    }

    // GetColorInC__Q34nw4r3lyt8TevStageCFv
    GXTevColorArg GetColorInC() const {
        return (GXTevColorArg)colIn.GetC();
    }

    // GetColorInD__Q34nw4r3lyt8TevStageCFv
    GXTevColorArg GetColorInD() const {
        return (GXTevColorArg)colIn.GetD();
    }

    // GetColorOp__Q34nw4r3lyt8TevStageCFv
    GXTevOp GetColorOp() const {
        return (GXTevOp)colIn.GetOp();
    }

    // GetColorBias__Q34nw4r3lyt8TevStageCFv
    GXTevBias GetColorBias() const {
        return (GXTevBias)colIn.GetBias();
    }

    // GetColorScale__Q34nw4r3lyt8TevStageCFv
    GXTevScale GetColorScale() const {
        return (GXTevScale)colIn.GetScale();
    }

    // IsColorClamp__Q34nw4r3lyt8TevStageCFv
    bool IsColorClamp() const {
        return colIn.IsClamp();
    }

    // GetColorOutReg__Q34nw4r3lyt8TevStageCFv
    GXTevRegID GetColorOutReg() const {
        return (GXTevRegID)colIn.GetOutReg();
    }

    // GetKColorSel__Q34nw4r3lyt8TevStageCFv
    GXTevKColorSel GetKColorSel() const {
        return (GXTevKColorSel)colIn.GetKSel();
    }

    // GetAlphaInA__Q34nw4r3lyt8TevStageCFv
    GXTevAlphaArg GetAlphaInA() const {
        return (GXTevAlphaArg)alpIn.GetA();
    }

    // GetAlphaInB__Q34nw4r3lyt8TevStageCFv
    GXTevAlphaArg GetAlphaInB() const {
        return (GXTevAlphaArg)alpIn.GetB();
    }

    // GetAlphaInC__Q34nw4r3lyt8TevStageCFv
    GXTevAlphaArg GetAlphaInC() const {
        return (GXTevAlphaArg)alpIn.GetC();
    }

    // GetAlphaInD__Q34nw4r3lyt8TevStageCFv
    GXTevAlphaArg GetAlphaInD() const {
        return (GXTevAlphaArg)alpIn.GetD();
    }

    // GetAlphaOp__Q34nw4r3lyt8TevStageCFv
    GXTevOp GetAlphaOp() const {
        return (GXTevOp)alpIn.GetOp();
    }

    // GetAlphaBias__Q34nw4r3lyt8TevStageCFv
    GXTevBias GetAlphaBias() const {
        return (GXTevBias)alpIn.GetBias();
    }

    // GetAlphaScale__Q34nw4r3lyt8TevStageCFv
    GXTevScale GetAlphaScale() const {
        return (GXTevScale)alpIn.GetScale();
    }

    // IsAlphaClamp__Q34nw4r3lyt8TevStageCFv
    bool IsAlphaClamp() const {
        return alpIn.IsClamp();
    }

    // GetAlphaOutReg__Q34nw4r3lyt8TevStageCFv
    GXTevRegID GetAlphaOutReg() const {
        return (GXTevRegID)alpIn.GetOutReg();
    }

    // GetKAlphaSel__Q34nw4r3lyt8TevStageCFv
    GXTevKAlphaSel GetKAlphaSel() const {
        return (GXTevKAlphaSel)alpIn.GetKSel();
    }

    // GetIndMtxSel__Q34nw4r3lyt8TevStageCFv
    GXIndTexMtxID GetIndMtxSel() const {
        return (GXIndTexMtxID)((inBiMt >> 3) & 0xF);
    }

    // GetIndStage__Q34nw4r3lyt8TevStageCFv
    GXIndTexStageID GetIndStage() const {
        return (GXIndTexStageID)indStage;
    }

    // GetIndFormat__Q34nw4r3lyt8TevStageCFv
    GXIndTexFormat GetIndFormat() const {
        return (GXIndTexFormat)((indFoAdUtAl >> 0) & 0x3);
    }

    // GetIndBiasSel__Q34nw4r3lyt8TevStageCFv
    GXIndTexBiasSel GetIndBiasSel() const {
        return (GXIndTexBiasSel)((inBiMt >> 0) & 0x7);
    }

    // GetIndWrapS__Q34nw4r3lyt8TevStageCFv
    GXIndTexWrap GetIndWrapS() const {
        return (GXIndTexWrap)((indWrap >> 0) & 0x7);
    }

    // GetIndWrapT__Q34nw4r3lyt8TevStageCFv
    GXIndTexWrap GetIndWrapT() const {
        return (GXIndTexWrap)((indWrap >> 3) & 0x7);
    }

    // IsIndAddPrev__Q34nw4r3lyt8TevStageCFv
    u8 IsIndAddPrev() const {
        return ((indFoAdUtAl >> 2) & 1);
    }

    // IsIndUtcLod__Q34nw4r3lyt8TevStageCFv
    u8 IsIndUtcLod() const {
        return ((indFoAdUtAl >> 3) & 1);
    }

    // GetIndAlphaSel__Q34nw4r3lyt8TevStageCFv
    GXIndTexAlphaSel GetIndAlphaSel() const {
        return (GXIndTexAlphaSel)((indFoAdUtAl >> 4) & 3);
    }

    // SetOrder__Q34nw4r3lyt8TevStageF13_GXTexCoordID11_GXTexMapID12_GXChannelID13_GXTevSwapSel13_GXTevSwapSel
    void SetOrder(
        GXTexCoordID aTexCoordGen, GXTexMapID aTexMap, GXChannelID aColChan, GXTevSwapSel rasSel, GXTevSwapSel texSel
    ) {
        texCoordGen = aTexCoordGen;
        colChan = aColChan;
        texMap = aTexMap;
        swapSel = ((rasSel & 3) << 1) | ((texSel & 3) << 3);
    }

    // SetColorIn__Q34nw4r3lyt8TevStageF14_GXTevColorArg14_GXTevColorArg14_GXTevColorArg14_GXTevColorArg
    void SetColorIn(GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d) {
        colIn.SetIn(a, b, c, d);
    }

    // SetAlphaIn__Q34nw4r3lyt8TevStageF14_GXTevAlphaArg14_GXTevAlphaArg14_GXTevAlphaArg14_GXTevAlphaArg
    void SetAlphaIn(GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d) {
        alpIn.SetIn(a, b, c, d);
    }

    // SetColorOp__Q34nw4r3lyt8TevStageF8_GXTevOp10_GXTevBias11_GXTevScaleb11_GXTevRegID15_GXTevKColorSel
    void SetColorOp(GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp, GXTevRegID outReg, GXTevKColorSel kSel) {
        colIn.SetOp(op, bias, scale, clamp, outReg, kSel);
    }

    // SetAlphaOp__Q34nw4r3lyt8TevStageF8_GXTevOp10_GXTevBias11_GXTevScaleb11_GXTevRegID15_GXTevKAlphaSel
    void SetAlphaOp(GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp, GXTevRegID outReg, GXTevKAlphaSel kSel) {
        alpIn.SetOp(op, bias, scale, clamp, outReg, kSel);
    }

    // SetIndirect__Q34nw4r3lyt8TevStageF16_GXIndTexStageID15_GXIndTexFormat16_GXIndTexBiasSel14_GXIndTexMtxID13_GXIndTexWrap13_GXIndTexWrapbb17_GXIndTexAlphaSel
    void SetIndirect(
        GXIndTexStageID stage, GXIndTexFormat format, GXIndTexBiasSel bias, GXIndTexMtxID mtxSel, GXIndTexWrap wrapS,
        GXIndTexWrap wrapT, bool addPrev, bool utcLod, GXIndTexAlphaSel alphaSel
    ) {
        indStage = stage;
        inBiMt = (bias & 0x7) | ((mtxSel & 0xF) << 4);
        indWrap = (wrapS & 0x7) | ((wrapT & 0x7) << 3);
        indFoAdUtAl = (format & 3) | (addPrev << 2) | (utcLod << 3) | ((alphaSel & 0x3) << 4);
    }

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
    GXTevColorChan GetR() const {
        return (GXTevColorChan)((swap >> 0) & 0x3);
    }

    // GetG__Q34nw4r3lyt11TevSwapModeCFv
    GXTevColorChan GetG() const {
        return (GXTevColorChan)((swap >> 2) & 0x3);
    }

    // GetB__Q34nw4r3lyt11TevSwapModeCFv
    GXTevColorChan GetB() const {
        return (GXTevColorChan)((swap >> 4) & 0x3);
    }

    // GetA__Q34nw4r3lyt11TevSwapModeCFv
    GXTevColorChan GetA() const {
        return (GXTevColorChan)((swap >> 6) & 0x3);
    }

    // Set__Q34nw4r3lyt11TevSwapModeF15_GXTevColorChan15_GXTevColorChan15_GXTevColorChan15_GXTevColorChan
    void Set(GXTevColorChan r, GXTevColorChan g, GXTevColorChan b, GXTevColorChan a) {
        swap = (r << 0) | (g << 2) | (b << 4) | (a << 6);
    }

    u8 swap; // at 0x0
};

} // namespace lyt

} // namespace nw4r

#endif
