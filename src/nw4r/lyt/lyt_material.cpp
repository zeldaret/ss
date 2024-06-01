#include <nw4r/lyt/lyt_material.h>

// DefaultBlackColor
// DefaultWhiteColor

// __eq__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRC11_GXColorS10RC11_GXColorS10
inline bool operator==(const GXColorS10 &a, const GXColorS10 &b) {
    return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}

inline bool operator!=(const GXColorS10 &a, const GXColorS10 &b) {
    return (a.r != b.r || a.g != b.g || a.b != b.b || a.a != b.a);
}
namespace nw4r {
namespace lyt {

namespace {

// CalcTextureMtx__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3lyt6TexSRT
void CalcTextureMtx(math::MTX34 *pMtx, const TexSRT &texSRT) {
    math::VEC2 center;
    f32 cosR;
    f32 sinR;
    f32 a0;
    f32 a1;
}

// CalcIndTexMtx__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FPA3_fRCQ34nw4r3lyt6TexSRT
void CalcIndTexMtx(f32 (*mtx)[3], const TexSRT &texSRT) {
    f32 cosR;
    f32 sinR;
}

// SetColorComponentValue__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FPQ34nw4r2ut5ColorUls
void SetColorComponentValue(ut::Color *pCol, u32 compIdx, s16 value) {
    u8 u8Val;
}

// SetIndTexMtx__Q34nw4r3lyt26@unnamed@lyt_material_cpp@F14_GXIndTexMtxIDPA3_Cf
void SetIndTexMtx(GXIndTexMtxID id, const f32 (*mtx)[3]) {
    f32 m00, m01, m02, m10, m11, m12, a00, a01, a02, a10, a11, a12;
    s8 scaleExp;
    f32 outMtx[3];
}

// InitTexSRT__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FPQ34nw4r3lyt6TexSRTUl
void InitTexSRT(TexSRT *texSRTs, u32 num) {
    u32 i;
}

// CalcOffsetTexSRTAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetTexSRTAry(const detail::BitGXNums &bitGXNums) {}

// CalcOffsetTexCoordGenAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetTexCoordGenAry(const detail::BitGXNums &bitGXNums) {}

// CalcOffsetChanCtrlAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetChanCtrlAry(const detail::BitGXNums &bitGXNums) {}

// CalcOffsetMatColAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetMatColAry(const detail::BitGXNums &bitGXNums) {}

// CalcOffsetTevSwapAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetTevSwapAry(const detail::BitGXNums &bitGXNums) {}

// CalcOffsetGetAlphaCompare__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetGetAlphaCompare(const detail::BitGXNums &bitGXNums) {}

// CalcOffsetBlendMode__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetBlendMode(const detail::BitGXNums &bitGXNums) {}

// CalcOffsetIndirectStageAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetIndirectStageAry(const detail::BitGXNums &bitGXNums) {}

// CalcOffsetIndTexSRTAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetIndTexSRTAry(const detail::BitGXNums &bitGXNums) {}

// CalcOffsetTevStageAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetTevStageAry(const detail::BitGXNums &bitGXNums) {}

} // namespace

// __ct__Q34nw4r3lyt8MaterialFPCQ44nw4r3lyt3res8MaterialRCQ34nw4r3lyt11ResBlockSet
Material::Material(const res::Material *pRes, const ResBlockSet &resBlockSet) {
    { int i; }
    { int i; }
    u32 resOffs;
    const res::TexMap *pResTexMap;
    const res::TexSRT *pResTexSRTs;
    const TexCoordGen *resTexCoordGen;
    u8 texMapNum;
    u8 texSRTNum;
    u8 texCoordGenNum;
    bool allocChanCtrl;
    bool allocMatCol;
    bool allocTevSwap;
    bool allocAlpComp;
    bool allocBlendMode;
    u8 indTexSRTNum;
    u8 indStageNum;
    u8 tevStageNum;
    const res::Texture *textures;
    const TexMap *texMaps;
    u8 di, si;
    const char *fileName;
    void *pTplRes;
    res::TexSRT *texSRTs;
    { int i; }
    TexCoordGen *texCoordGens;
    { int i; }
    const ChanCtrl *pResChanCtrl;
    ut::Color *pResMatCol;
    const TevSwapMode *pResTevSwap;
    const TevSwapMode *tevSwaps;
    { int i; }
    const TexSRT *indTexSRTs;
    const TexSRT *pResIndMtx;
    { int i; }
    IndirectStage *indirectStages;
    const IndirectStage *pResIndStg;
    { int i; }
    const TevStage *tevStages;
    const TevStage *pResTevStg;
    { int i; }
    const AlphaCompare *pResAlphaCompare;
    const BlendMode *pResBlendMode;
}

// Init__Q34nw4r3lyt8MaterialFv
void Material::Init() {}

// __dt__Q34nw4r3lyt8MaterialFv
Material::~Material() {}

// InitBitGXNums__Q34nw4r3lyt8MaterialFPQ44nw4r3lyt6detail9BitGXNums
void Material::InitBitGXNums(detail::BitGXNums *ptr) {}

// ReserveGXMem__Q34nw4r3lyt8MaterialFUcUcUcUcbUcUcbbbb
void Material::ReserveGXMem(u8 texMapNum, u8 texSRTNum, u8 texCoordGenNum, u8 tevStageNum, bool allocTevSwap,
        u8 indStageNum, u8 indSRTNum, bool allocChanCtrl, bool allocMatCol, bool allocAlpComp, bool allocBlendMode) {
    int tevSwapNum;
    int chanCtrlNum;
    int matColNum;
    int alpCompNum;
    int blendModeNum;
    const TevSwapMode *tevSwaps;
}

// GetTexMapAry__Q34nw4r3lyt8MaterialCFv
TexMap *Material::GetTexMapAry() const {}

// GetTexMapAry__Q34nw4r3lyt8MaterialFv
TexMap *Material::GetTexMapAry() {}

// GetTexSRTAry__Q34nw4r3lyt8MaterialFv
TexSRT *Material::GetTexSRTAry() {}

// GetTexCoordGenAry__Q34nw4r3lyt8MaterialFv
TexCoordGen *Material::GetTexCoordGenAry() {}

// GetChanCtrlAry__Q34nw4r3lyt8MaterialFv
ChanCtrl *Material::GetChanCtrlAry() {}

// GetMatColAry__Q34nw4r3lyt8MaterialFv
ut::Color *Material::GetMatColAry() {}

// GetTevSwapAry__Q34nw4r3lyt8MaterialFv
TevSwapMode *Material::GetTevSwapAry() {}

// GetAlphaComparePtr__Q34nw4r3lyt8MaterialFv
AlphaCompare *Material::GetAlphaComparePtr() {}

// GetBlendModePtr__Q34nw4r3lyt8MaterialFv
BlendMode *Material::GetBlendModePtr() {}

// GetIndirectStageAry__Q34nw4r3lyt8MaterialFv
IndirectStage *Material::GetIndirectStageAry() {}

// GetIndTexSRTAry__Q34nw4r3lyt8MaterialFv
TexSRT *Material::GetIndTexSRTAry() {}

// GetTevStageAry__Q34nw4r3lyt8MaterialFv
TevStage *Material::GetTevStageAry() {}

// SetName__Q34nw4r3lyt8MaterialFPCc
void Material::SetName(const char *name) {}

// SetTextureNum__Q34nw4r3lyt8MaterialFUc
void Material::SetTextureNum(u8 num) {
    TexMap *texMaps;
    u32 i;
}

// SetTexCoordGenNum__Q34nw4r3lyt8MaterialFUc
void Material::SetTexCoordGenNum(u8 num) {
    TexCoordGen *texCoordGens;
    u32 i;
}

// SetTevStageNum__Q34nw4r3lyt8MaterialFUc
void Material::SetTevStageNum(u8 num) {
    TevStage *tevStages;
    u32 i;
}

// SetIndStageNum__Q34nw4r3lyt8MaterialFUc
void Material::SetIndStageNum(u8 num) {
    IndirectStage *indStages;
    u32 i;
}

// SetColorElement__Q34nw4r3lyt8MaterialFUls
void Material::SetColorElement(u32 colorType, s16 value) {
    ut::Color *matCols;
    { u32 regIdx; }
    { u32 regIdx; }
    GXTevKColorSel kColSels[8];
    GXTevKAlphaSel kAlpSels[8];
}

// SetupGX__Q34nw4r3lyt8MaterialFbUc
bool Material::SetupGX(bool bModVtxCol, u8 alpha) {
    bool bUseVtxCol;
    bool bUseMatCol;
    const ChanCtrl *chanCtrls;
    bool bUseRasStage;
    ut::Color matCol;
    bool bSetTexMtx;
    bool bUseTexMtxl;
    { int i; }
    const TexCoordGen *texCoordGens;
    { int i; }
    u32 texMtx;
    const TexSRT *texSRTs;
    { int i; }
    { math::MTX34 texMtx; }
    u32 tlutID;
    u32 bigTlutID;
    const TexMap *texMaps;
    { int i; }
    // TexMap & texMap;
    GXTexObj texObj;
    u32 tlutName;
    GXTlutObj tlutObj;
    const TevSwapMode *tevSwaps;
    { int i; }
    bool bSetIndTexMtx;
    bool bUseIndTexMtx;
    { int i; }
    const TevStage *tevStages;
    { int i; }
    GXTevStageID tevStage;
    GXIndTexMtxID indMtxSel;
    u8 tevStageID;
    { GXTevStageID tevStage; }
    { GXTevStageID tevStage; }
    { GXTevStageID tevStage; }
    { int i; }
    { GXTevStageID tevStage; }
    GXTevColorArg colDIn;
    GXTevAlphaArg alpDIn;
    { GXTevStageID tevStage; }
    { GXTevStageID tevStage; }
    u8 tevStageNum;
    u8 id;
    { GXTevStageID tevStage; }
    const TexSRT *indTexSRTs;
    { int i; }
    f32 mtx[2][3];
    const IndirectStage *indirectStages;
    { int i; }
    GXIndTexStageID indStage;
    const AlphaCompare *pAlpComp;
    const BlendMode *pBlendMode;
}

namespace {
// GetTexMtx__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FUl
u32 GetTexMtx(u32 texMtxIdx) {}

// GetTexMtxIdx__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FUl
u32 GetTexMtxIdx(u32 texMtx) {}

} // namespace

// BindAnimation__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform
void Material::BindAnimation(AnimTransform *pAnimTrans) {}

// UnbindAnimation__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform
void Material::UnbindAnimation(AnimTransform *pAnimTrans) {}

// UnbindAllAnimation__Q34nw4r3lyt8MaterialFv
void Material::UnbindAllAnimation() {}

// Animate__Q34nw4r3lyt8MaterialFv
void Material::Animate() {
    ut::LinkList<AnimTransform, 0>::Iterator it;
    AnimTransform *animTrans;
}

// AddAnimationLink__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimationLink
void Material::AddAnimationLink(AnimationLink *pAnimationLink) {}

// FindAnimationLink__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform
AnimationLink *Material::FindAnimationLink(AnimTransform *pAnimTrans) {}

// FindAnimationLink__Q34nw4r3lyt8MaterialFRCQ34nw4r3lyt12AnimResource
AnimationLink *Material::FindAnimationLink(const AnimResource &animRes) {}

// SetAnimationEnable__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransformb
void Material::SetAnimationEnable(AnimTransform *pAnimTrans, bool bEnable) {}

// SetAnimationEnable__Q34nw4r3lyt8MaterialFRCQ34nw4r3lyt12AnimResourceb
void Material::SetAnimationEnable(const AnimResource &animRes, bool bEnable) {
    AnimationLink *pAnimLink;
}

namespace detail {

// GetTextureSize__Q34nw4r3lyt6detailFPQ34nw4r3lyt8MaterialUc
Size GetTextureSize(Material *pMaterial, u8 texMapIdx) {}

} // namespace detail

} // namespace lyt

} // namespace nw4r

// INLINED FROM OTHER FILES
// ut_inlines.h
// Min<s>__Q34nw4r2ut26@unnamed@lyt_material_cpp@Fss_s
// Max<s>__Q34nw4r2ut26@unnamed@lyt_material_cpp@Fss_s
// Min<Uc>__Q34nw4r2ut26@unnamed@lyt_material_cpp@FUcUc_Uc

// lyt_types.h
// ConvertOffsToPtr<Q34nw4r3lyt9BlendMode>__Q34nw4r3lyt6detailFPCvUi_PCQ34nw4r3lyt9BlendMode
// ConvertOffsToPtr<Q34nw4r3lyt12AlphaCompare>__Q34nw4r3lyt6detailFPCvUi_PCQ34nw4r3lyt12AlphaCompare
// ConvertOffsToPtr<Q34nw4r3lyt8TevStage>__Q34nw4r3lyt6detailFPCvUi_PCQ34nw4r3lyt8TevStage
// ConvertOffsToPtr<Q34nw4r3lyt13IndirectStage>__Q34nw4r3lyt6detailFPCvUi_PCQ34nw4r3lyt13IndirectStage
// ConvertOffsToPtr<Q34nw4r3lyt11TevSwapMode>__Q34nw4r3lyt6detailFPCvUi_PCQ34nw4r3lyt11TevSwapMode
// ConvertOffsToPtr<Q34nw4r2ut5Color>__Q34nw4r3lyt6detailFPCvUi_PCQ34nw4r2ut5Color
// ConvertOffsToPtr<Q34nw4r3lyt8ChanCtrl>__Q34nw4r3lyt6detailFPCvUi_PCQ34nw4r3lyt8ChanCtrl
// GetBits<i>__Q34nw4r3lyt6detailFiii_i
// GetBits<Uc>__Q34nw4r3lyt6detailFUcii_Uc
// ConvertOffsToPtr<Q44nw4r3lyt3res7Texture>__Q34nw4r3lyt6detailFPCvUi_PCQ44nw4r3lyt3res7Texture
// GetBits<Ul>__Q34nw4r3lyt6detailFUlii_Ul
// TestBit<Ul>__Q34nw4r3lyt6detailFUli_b
// ConvertOffsToPtr<Q34nw4r3lyt11TexCoordGen>__Q34nw4r3lyt6detailFPCvUi_PCQ34nw4r3lyt11TexCoordGen
// ConvertOffsToPtr<Q34nw4r3lyt6TexSRT>__Q34nw4r3lyt6detailFPCvUi_PCQ34nw4r3lyt6TexSRT
// ConvertOffsToPtr<Q44nw4r3lyt3res6TexMap>__Q34nw4r3lyt6detailFPCvUi_PCQ44nw4r3lyt3res6TexMap
// __ct__Q34nw4r3lyt9BlendModeFv
// Set__Q34nw4r3lyt9BlendModeF12_GXBlendMode14_GXBlendFactor14_GXBlendFactor10_GXLogicOp
// __ct__Q34nw4r3lyt12AlphaCompareFv
// Set__Q34nw4r3lyt12AlphaCompareF10_GXCompareUc10_GXAlphaOp10_GXCompareUc
// Set__Q34nw4r3lyt11TevSwapModeF15_GXTevColorChan15_GXTevColorChan15_GXTevColorChan15_GXTevColorChan
// __ct__Q34nw4r3lyt8ChanCtrlFv
// Set__Q34nw4r3lyt8ChanCtrlF11_GXColorSrc11_GXColorSrc
// ConvertOffsToPtr<Q34nw4r3lyt6TexMap>__Q34nw4r3lyt6detailFPvUi_PQ34nw4r3lyt6TexMap
// ConvertOffsToPtr<Q34nw4r3lyt6TexSRT>__Q34nw4r3lyt6detailFPvUi_PQ34nw4r3lyt6TexSRT
// ConvertOffsToPtr<Q34nw4r3lyt11TexCoordGen>__Q34nw4r3lyt6detailFPvUi_PQ34nw4r3lyt11TexCoordGen
// ConvertOffsToPtr<Q34nw4r3lyt8ChanCtrl>__Q34nw4r3lyt6detailFPvUi_PQ34nw4r3lyt8ChanCtrl
// ConvertOffsToPtr<Q34nw4r2ut5Color>__Q34nw4r3lyt6detailFPvUi_PQ34nw4r2ut5Color
// ConvertOffsToPtr<Q34nw4r3lyt11TevSwapMode>__Q34nw4r3lyt6detailFPvUi_PQ34nw4r3lyt11TevSwapMode
// ConvertOffsToPtr<Q34nw4r3lyt12AlphaCompare>__Q34nw4r3lyt6detailFPvUi_PQ34nw4r3lyt12AlphaCompare
// ConvertOffsToPtr<Q34nw4r3lyt9BlendMode>__Q34nw4r3lyt6detailFPvUi_PQ34nw4r3lyt9BlendMode
// ConvertOffsToPtr<Q34nw4r3lyt13IndirectStage>__Q34nw4r3lyt6detailFPvUi_PQ34nw4r3lyt13IndirectStage
// ConvertOffsToPtr<Q34nw4r3lyt8TevStage>__Q34nw4r3lyt6detailFPvUi_PQ34nw4r3lyt8TevStage
// __ct__Q34nw4r3lyt8TevStageFv
// SetIndirect__Q34nw4r3lyt8TevStageF16_GXIndTexStageID15_GXIndTexFormat16_GXIndTexBiasSel14_GXIndTexMtxID13_GXIndTexWrap13_GXIndTexWrapbb17_GXIndTexAlphaSel
// SetAlphaOp__Q34nw4r3lyt8TevStageF8_GXTevOp10_GXTevBias11_GXTevScaleb11_GXTevRegID15_GXTevKAlphaSel
// SetOp__Q34nw4r3lyt12TevStageInOpFUcUcUcbUcUc
// SetColorOp__Q34nw4r3lyt8TevStageF8_GXTevOp10_GXTevBias11_GXTevScaleb11_GXTevRegID15_GXTevKColorSel
// SetAlphaIn__Q34nw4r3lyt8TevStageF14_GXTevAlphaArg14_GXTevAlphaArg14_GXTevAlphaArg14_GXTevAlphaArg
// SetIn__Q34nw4r3lyt12TevStageInOpFUcUcUcUc
// SetColorIn__Q34nw4r3lyt8TevStageF14_GXTevColorArg14_GXTevColorArg14_GXTevColorArg14_GXTevColorArg
// SetOrder__Q34nw4r3lyt8TevStageF13_GXTexCoordID11_GXTexMapID12_GXChannelID13_GXTevSwapSel13_GXTevSwapSel
// __ct__Q34nw4r3lyt13IndirectStageFv
// Set__Q34nw4r3lyt13IndirectStageF13_GXTexCoordID11_GXTexMapID14_GXIndTexScale14_GXIndTexScale
// GetOp__Q34nw4r3lyt9BlendModeCFv
// GetDstFactor__Q34nw4r3lyt9BlendModeCFv
// GetSrcFactor__Q34nw4r3lyt9BlendModeCFv
// GetType__Q34nw4r3lyt9BlendModeCFv
// GetRef1__Q34nw4r3lyt12AlphaCompareCFv
// GetComp1__Q34nw4r3lyt12AlphaCompareCFv
// GetOp__Q34nw4r3lyt12AlphaCompareCFv
// GetRef0__Q34nw4r3lyt12AlphaCompareCFv
// GetComp0__Q34nw4r3lyt12AlphaCompareCFv
// GetScaleT__Q34nw4r3lyt13IndirectStageCFv
// GetScaleS__Q34nw4r3lyt13IndirectStageCFv
// GetTexMap__Q34nw4r3lyt13IndirectStageCFv
// GetTexCoordGen__Q34nw4r3lyt13IndirectStageCFv
// GetIndAlphaSel__Q34nw4r3lyt8TevStageCFv
// IsIndUtcLod__Q34nw4r3lyt8TevStageCFv
// IsIndAddPrev__Q34nw4r3lyt8TevStageCFv
// GetIndWrapT__Q34nw4r3lyt8TevStageCFv
// GetIndWrapS__Q34nw4r3lyt8TevStageCFv
// GetIndBiasSel__Q34nw4r3lyt8TevStageCFv
// GetIndFormat__Q34nw4r3lyt8TevStageCFv
// GetIndStage__Q34nw4r3lyt8TevStageCFv
// GetIndMtxSel__Q34nw4r3lyt8TevStageCFv
// GetKAlphaSel__Q34nw4r3lyt8TevStageCFv
// GetKSel__Q34nw4r3lyt12TevStageInOpCFv
// GetAlphaOutReg__Q34nw4r3lyt8TevStageCFv
// GetOutReg__Q34nw4r3lyt12TevStageInOpCFv
// IsAlphaClamp__Q34nw4r3lyt8TevStageCFv
// IsClamp__Q34nw4r3lyt12TevStageInOpCFv
// GetAlphaScale__Q34nw4r3lyt8TevStageCFv
// GetScale__Q34nw4r3lyt12TevStageInOpCFv
// GetAlphaBias__Q34nw4r3lyt8TevStageCFv
// GetBias__Q34nw4r3lyt12TevStageInOpCFv
// GetAlphaOp__Q34nw4r3lyt8TevStageCFv
// GetOp__Q34nw4r3lyt12TevStageInOpCFv
// GetAlphaInD__Q34nw4r3lyt8TevStageCFv
// GetD__Q34nw4r3lyt12TevStageInOpCFv
// GetAlphaInC__Q34nw4r3lyt8TevStageCFv
// GetC__Q34nw4r3lyt12TevStageInOpCFv
// GetAlphaInB__Q34nw4r3lyt8TevStageCFv
// GetB__Q34nw4r3lyt12TevStageInOpCFv
// GetAlphaInA__Q34nw4r3lyt8TevStageCFv
// GetA__Q34nw4r3lyt12TevStageInOpCFv
// GetKColorSel__Q34nw4r3lyt8TevStageCFv
// GetColorOutReg__Q34nw4r3lyt8TevStageCFv
// IsColorClamp__Q34nw4r3lyt8TevStageCFv
// GetColorScale__Q34nw4r3lyt8TevStageCFv
// GetColorBias__Q34nw4r3lyt8TevStageCFv
// GetColorOp__Q34nw4r3lyt8TevStageCFv
// GetColorInD__Q34nw4r3lyt8TevStageCFv
// GetColorInC__Q34nw4r3lyt8TevStageCFv
// GetColorInB__Q34nw4r3lyt8TevStageCFv
// GetColorInA__Q34nw4r3lyt8TevStageCFv
// GetTexSwapSel__Q34nw4r3lyt8TevStageCFv
// GetRasSwapSel__Q34nw4r3lyt8TevStageCFv
// GetColorChan__Q34nw4r3lyt8TevStageCFv
// GetTexMap__Q34nw4r3lyt8TevStageCFv
// GetTexCoordGen__Q34nw4r3lyt8TevStageCFv
// GetA__Q34nw4r3lyt11TevSwapModeCFv
// GetB__Q34nw4r3lyt11TevSwapModeCFv
// GetG__Q34nw4r3lyt11TevSwapModeCFv
// GetR__Q34nw4r3lyt11TevSwapModeCFv
