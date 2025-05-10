
#include "nw4r/lyt/lyt_material.h"

#include "nw4r/lyt/lyt_layout.h"
#include "string.h"


void float_ordering(u16 a) {
    0.5f;
    256.0f / 360.0f;
    0.0f;
    1.0f;
    2.0f;
    (f32) a;
}

// All 0 intialization puts it in sbss2??
static const GXColorS10 DefaultBlackColor = {0, 0, 0, 0};
static const GXColorS10 DefaultWhiteColor = {0xFF, 0xFF, 0xFF, 0xFF};

namespace nw4r {
namespace lyt {
namespace {
// __eq__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRC11_GXColorS10RC11_GXColorS10
bool operator==(const GXColorS10 &a, const GXColorS10 &b) {
    return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}

bool operator!=(const GXColorS10 &a, const GXColorS10 &b) {
    return !(a == b);
}

// CalcTextureMtx__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3lyt6TexSRT
void CalcTextureMtx(math::MTX34 *pMtx, const TexSRT &texSRT) {
    math::VEC2 center(0.5f, 0.5f);
    f32 cosR = math::CosDeg(texSRT.rotate);
    f32 sinR = math::SinDeg(texSRT.rotate);
    f32 a0, a1;

    a0 = cosR * texSRT.scale.x;
    a1 = -sinR * texSRT.scale.y;
    pMtx->_00 = a0;
    pMtx->_01 = a1;
    pMtx->_02 = 0.0f;
    pMtx->_03 = texSRT.translate.x + center.x + a0 * -center.x + a1 * -center.y;

    a0 = sinR * texSRT.scale.x;
    a1 = cosR * texSRT.scale.y;
    pMtx->_10 = a0;
    pMtx->_11 = a1;
    pMtx->_12 = 0.0f;
    pMtx->_13 = texSRT.translate.y + center.y + a0 * -center.x + a1 * -center.y;

    pMtx->_20 = 0.0f;
    pMtx->_21 = 0.0f;
    pMtx->_22 = 1.0f;
    pMtx->_23 = 0.0f;
}

// CalcIndTexMtx__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FPA3_fRCQ34nw4r3lyt6TexSRT
void CalcIndTexMtx(f32 (*mtx)[3], const TexSRT &texSRT) {
    f32 cosR = math::CosDeg(texSRT.rotate);
    f32 sinR = math::SinDeg(texSRT.rotate);

    mtx[0][0] = cosR * texSRT.scale.x;
    mtx[0][1] = -sinR * texSRT.scale.y;
    mtx[0][2] = texSRT.translate.x;
    mtx[1][0] = sinR * texSRT.scale.x;
    mtx[1][1] = cosR * texSRT.scale.y;
    mtx[1][2] = texSRT.translate.y;
}

// SetColorComponentValue__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FPQ34nw4r2ut5ColorUls
void SetColorComponentValue(ut::Color *pCol, u32 compIdx, s16 value) {
    u8 u8Val = ut::Min<s16>(ut::Max<s16>(value, 0), 0xFF);
    switch (compIdx % 4) {
        case 0:  pCol->r = u8Val; break;
        case 1:  pCol->g = u8Val; break;
        case 2:  pCol->b = u8Val; break;
        case 3:  pCol->a = u8Val; break;
        default: break;
    }
}

// SetIndTexMtx__Q34nw4r3lyt26@unnamed@lyt_material_cpp@F14_GXIndTexMtxIDPA3_Cf
void SetIndTexMtx(GXIndTexMtxID id, const f32 (*mtx)[3]) {
    f32 m00 = mtx[0][0];
    f32 m01 = mtx[0][1];
    f32 m02 = mtx[0][2];
    f32 m10 = mtx[1][0];
    f32 m11 = mtx[1][1];
    f32 m12 = mtx[1][2];
    f32 a00 = math::FAbs(m00);
    f32 a01 = math::FAbs(m01);
    f32 a02 = math::FAbs(m02);
    f32 a10 = math::FAbs(m10);
    f32 a11 = math::FAbs(m11);
    f32 a12 = math::FAbs(m12);
    s8 scaleExp = 0;

    if ((a00 >= 1.0f || a01 >= 1.0f || a02 >= 1.0f || a10 >= 1.0f || a11 >= 1.0f || a12 >= 1.0f)) {
        do {
            if (scaleExp >= 46) {
                break;
            }
            m00 *= 0.5f;
            m01 *= 0.5f;
            m02 *= 0.5f;
            m10 *= 0.5f;
            m11 *= 0.5f;
            m12 *= 0.5f;
            a00 *= 0.5f;
            a01 *= 0.5f;
            a02 *= 0.5f;
            a10 *= 0.5f;
            a11 *= 0.5f;
            a12 *= 0.5f;
            scaleExp++;
        } while ((a00 >= 1.0f || a01 >= 1.0f || a02 >= 1.0f || a10 >= 1.0f || a11 >= 1.0f || a12 >= 1.0f));
    } else if (a00 < 0.5f && a01 < 0.5f && a02 < 0.5f && a10 < 0.5f && a11 < 0.5f && a12 < 0.5f) {
        do {
            a00 *= 2.0f;
            m00 *= 2.0f;
            m01 *= 2.0f;
            m02 *= 2.0f;
            m10 *= 2.0f;
            m11 *= 2.0f;
            m12 *= 2.0f;
            a01 *= 2.0f;
            a02 *= 2.0f;
            a10 *= 2.0f;
            a11 *= 2.0f;
            a12 *= 2.0f;
            scaleExp--;
        } while (a00 < 0.5f && a01 < 0.5f && a02 < 0.5f && a10 < 0.5f && a11 < 0.5f && a12 < 0.5f && scaleExp > -17);
    }
    f32 outMtx[2][3] = {m00, m01, m02, m10, m11, m12};
    GXSetIndTexMtx(id, outMtx, scaleExp);
}
// GetTexMtx__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FUl
u32 GetTexMtx(u32 texMtxIdx) {
    return 30 + 3 * texMtxIdx;
}

// GetTexMtxIdx__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FUl
u32 GetTexMtxIdx(u32 texMtx) {
    return (texMtx - 30) / 3;
}

// InitTexSRT__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FPQ34nw4r3lyt6TexSRTUl
void InitTexSRT(TexSRT *texSRTs, u32 num) {
    for (u32 i = 0; i < num; i++) {
        texSRTs[i].translate = math::VEC2(0.0f, 0.0f);
        texSRTs[i].rotate = 0.0f;
        texSRTs[i].scale = math::VEC2(1.0f, 1.0f);
    }
}

// CalcOffsetTexSRTAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetTexSRTAry(const detail::BitGXNums &bitGXNums) {
    return bitGXNums.texMap * sizeof(TexMap);
}

// CalcOffsetTexCoordGenAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetTexCoordGenAry(const detail::BitGXNums &bitGXNums) {
    return bitGXNums.texSRT * ROUND_UP(sizeof(TexSRT), 4) + CalcOffsetTexSRTAry(bitGXNums);
}

// CalcOffsetChanCtrlAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetChanCtrlAry(const detail::BitGXNums &bitGXNums) {
    return bitGXNums.texCoordGen * ROUND_UP(sizeof(TexCoordGen), 4) + CalcOffsetTexCoordGenAry(bitGXNums);
}

// CalcOffsetMatColAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetMatColAry(const detail::BitGXNums &bitGXNums) {
    return bitGXNums.chanCtrl * ROUND_UP(sizeof(ChanCtrl), 4) + CalcOffsetChanCtrlAry(bitGXNums);
}

// CalcOffsetTevSwapAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetTevSwapAry(const detail::BitGXNums &bitGXNums) {
    return bitGXNums.matCol * ROUND_UP(sizeof(ut::Color), 4) + CalcOffsetMatColAry(bitGXNums);
}

// CalcOffsetGetAlphaCompare__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetGetAlphaCompare(const detail::BitGXNums &bitGXNums) {
    // just sizeof(TevSwapMode)] doesnt work here. Assuming its aligned to 4 for a ptr purposeD
    return bitGXNums.tevSwap * ROUND_UP(sizeof(TevSwapMode), 4) + CalcOffsetTevSwapAry(bitGXNums);
}

// CalcOffsetBlendMode__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetBlendMode(const detail::BitGXNums &bitGXNums) {
    return bitGXNums.alpComp * ROUND_UP(sizeof(AlphaCompare), 4) + CalcOffsetGetAlphaCompare(bitGXNums);
}

// CalcOffsetIndirectStageAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetIndirectStageAry(const detail::BitGXNums &bitGXNums) {
    return bitGXNums.blendMode * ROUND_UP(sizeof(BlendMode), 4) + CalcOffsetBlendMode(bitGXNums);
}

// CalcOffsetIndTexSRTAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetIndTexSRTAry(const detail::BitGXNums &bitGXNums) {
    return bitGXNums.indStage * ROUND_UP(sizeof(IndirectStage), 4) + CalcOffsetIndirectStageAry(bitGXNums);
}

// CalcOffsetTevStageAry__Q34nw4r3lyt26@unnamed@lyt_material_cpp@FRCQ44nw4r3lyt6detail9BitGXNums
u32 CalcOffsetTevStageAry(const detail::BitGXNums &bitGXNums) {
    return bitGXNums.indSRT * ROUND_UP(sizeof(TexSRT), 4) + CalcOffsetIndTexSRTAry(bitGXNums);
}

} // namespace

// __ct__Q34nw4r3lyt8MaterialFv
Material::Material() {
    /* Exists, not in SS. Needed for some function ordering */
}

#pragma dont_inline on
// __ct__Q34nw4r3lyt8MaterialFPCQ44nw4r3lyt3res8MaterialRCQ34nw4r3lyt11ResBlockSet
Material::Material(const res::Material *pRes, const ResBlockSet &resBlockSet) : mAnimList(), mTevKCols() {
    Init();
    SetName(pRes->name);
    for (int i = 0; i < 3; i++) {
        mTevCols[i] = pRes->tevCols[i];
    }
    for (int i = 0; i < 4; i++) {
        mTevKCols[i] = pRes->tevKCols[i];
    }

    u32 resOffs = sizeof(res::Material);
    const res::TexMap *pResTexMap = detail::ConvertOffsToPtr<res::TexMap>(pRes, resOffs);
    resOffs += pRes->resNum.GetTexMapNum() * sizeof(res::TexMap);

    const TexSRT *pResTexSRTs = detail::ConvertOffsToPtr<TexSRT>(pRes, resOffs);
    resOffs += pRes->resNum.GetTexSRTNum() * sizeof(TexSRT);

    const TexCoordGen *resTexCoordGen = detail::ConvertOffsToPtr<TexCoordGen>(pRes, resOffs);
    resOffs += pRes->resNum.GetTexCoordGenNum() * sizeof(TexCoordGen);

    u8 texMapNum = ut::Min<u8>(pRes->resNum.GetTexMapNum(), 8);
    u8 texSRTNum = ut::Min<u8>(pRes->resNum.GetTexSRTNum(), 10);
    u8 texCoordGenNum = ut::Min<u8>(pRes->resNum.GetTexCoordGenNum(), 8);
    bool allocChanCtrl = pRes->resNum.GetChanCtrlNum() != 0;
    bool allocMatCol = pRes->resNum.GetMatColNum() != 0;
    bool allocTevSwap = pRes->resNum.HasTevSwapTable();
    bool allocAlpComp = pRes->resNum.HasAlphaCompare();
    bool allocBlendMode = pRes->resNum.HasBlendMode();
    u8 indTexSRTNum = ut::Min<u8>(pRes->resNum.GetIndTexSRTNum(), 3);
    u8 indStageNum = ut::Min<u8>(pRes->resNum.GetIndTexStageNum(), 4);
    u8 tevStageNum = ut::Min<u8>(pRes->resNum.GetTevStageNum(), 16);

    ReserveGXMem(
        texMapNum, texSRTNum, texCoordGenNum, tevStageNum, allocTevSwap, indStageNum, indTexSRTNum, allocChanCtrl,
        allocMatCol, allocAlpComp, allocBlendMode
    );

    if (mpGXMem) {
        SetTextureNum(texMapNum);
        if (texMapNum != 0) {
            const res::Texture *textures =
                detail::ConvertOffsToPtr<res::Texture>(resBlockSet.pTextureList, sizeof(res::TextureList));
            TexMap *texMaps = GetTexMapAry();
            u8 di = 0;
            for (u8 si = 0; si < texMapNum; di++, si++) {
                const char *fileName =
                    detail::ConvertOffsToPtr<char>(textures, textures[pResTexMap[si].texIdx].nameStrOffset);
                void *pTplRes = resBlockSet.pResAccessor->GetResource('timg', fileName, nullptr);
                texMaps[di].ReplaceImage((TPLPalette *)pTplRes, 0);
                texMaps[di].SetWrapMode(pResTexMap[si].GetWarpModeS(), pResTexMap[si].GetWarpModeT());
                texMaps[di].SetFilter(pResTexMap[si].GetMinFilter(), pResTexMap[si].GetMagFilter());
            }
            SetTextureNum(di);
        }
        TexSRT *texSRTs = GetTexSRTAry();
        for (int i = 0; i < texSRTNum; i++) {
            texSRTs[i].translate = pResTexSRTs[i].translate;
            texSRTs[i].rotate = pResTexSRTs[i].rotate;
            texSRTs[i].scale = pResTexSRTs[i].scale;
        }

        TexCoordGen *texCoordGens = GetTexCoordGenAry();
        SetTexCoordGenNum(texCoordGenNum);
        for (int i = 0; i < mGXMemNum.texCoordGen; i++) {
            texCoordGens[i] = resTexCoordGen[i];
        }

        if (allocChanCtrl) {
            const ChanCtrl *pResChanCtrl = detail::ConvertOffsToPtr<ChanCtrl>(pRes, resOffs);
            GetChanCtrlAry()[0] = *pResChanCtrl;
            resOffs += sizeof(ChanCtrl);
        }

        if (allocMatCol) {
            const ut::Color *pResMatCol = detail::ConvertOffsToPtr<ut::Color>(pRes, resOffs);
            GetMatColAry()[0] = *pResMatCol;
            resOffs += sizeof(ut::Color);
        }

        if (allocTevSwap) {
            const TevSwapMode *tevSwaps = detail::ConvertOffsToPtr<TevSwapMode>(pRes, resOffs);
            TevSwapMode *pResTevSwap = GetTevSwapAry();
            for (int i = 0; i < 4; i++) {
                pResTevSwap[i] = tevSwaps[i];
            }
            resOffs += 4 * sizeof(TevSwapMode);
        }

        if (indTexSRTNum != 0) {
            TexSRT *indTexSRTs = GetIndTexSRTAry();
            const TexSRT *pResIndMtx = detail::ConvertOffsToPtr<TexSRT>(pRes, resOffs);
            for (int i = 0; i < indTexSRTNum; i++) {
                indTexSRTs[i] = pResIndMtx[i];
            }
        }
        resOffs += pRes->resNum.GetIndTexSRTNum() * sizeof(TexSRT);

        if (indStageNum != 0) {
            SetIndStageNum(indStageNum);
            IndirectStage *indirectStages = GetIndirectStageAry();
            const IndirectStage *pResIndStg = detail::ConvertOffsToPtr<IndirectStage>(pRes, resOffs);
            for (int i = 0; i < indStageNum; i++) {
                indirectStages[i] = pResIndStg[i];
            }
        }
        resOffs += pRes->resNum.GetIndTexStageNum() * sizeof(IndirectStage);

        if (tevStageNum != 0) {
            SetTevStageNum(tevStageNum);
            TevStage *tevStages = GetTevStageAry();
            const TevStage *pResTevStg = detail::ConvertOffsToPtr<TevStage>(pRes, resOffs);
            for (int i = 0; i < tevStageNum; i++) {
                tevStages[i] = pResTevStg[i];
            }
        }
        resOffs += pRes->resNum.GetTevStageNum() * sizeof(TevStage);

        if (allocAlpComp) {
            const AlphaCompare *pResAlphaCompare = detail::ConvertOffsToPtr<AlphaCompare>(pRes, resOffs);
            *GetAlphaComparePtr() = *pResAlphaCompare;
            resOffs += sizeof(AlphaCompare);
        }
        if (allocBlendMode) {
            const BlendMode *pResBlendMode = detail::ConvertOffsToPtr<BlendMode>(pRes, resOffs);
            *GetBlendModePtr() = *pResBlendMode;
        }
    }
}
#pragma dont_inline reset

// Init__Q34nw4r3lyt8MaterialFv
void Material::Init() {
    mTevCols[0] = DefaultBlackColor;
    mTevCols[1] = DefaultWhiteColor;
    mTevCols[2] = DefaultWhiteColor;
    InitBitGXNums(&mGXMemCap);
    InitBitGXNums(&mGXMemNum);
    mbUserAllocated = false;
    mpGXMem = nullptr;
}

// __dt__Q34nw4r3lyt8MaterialFv
Material::~Material() {
    UnbindAllAnimation();
    if (mpGXMem) {
        Layout::FreeMemory(mpGXMem);
        mpGXMem = nullptr;
    }
}

// InitBitGXNums__Q34nw4r3lyt8MaterialFPQ44nw4r3lyt6detail9BitGXNums
void Material::InitBitGXNums(detail::BitGXNums *ptr) {
    ptr->texMap = 0;
    ptr->texSRT = 0;
    ptr->texCoordGen = 0;
    ptr->indSRT = 0;
    ptr->indStage = 0;
    ptr->tevSwap = 0;
    ptr->tevStage = 0;
    ptr->chanCtrl = 0;
    ptr->matCol = 0;
    ptr->alpComp = 0;
    ptr->blendMode = 0;
}

// ReserveGXMem__Q34nw4r3lyt8MaterialFUcUcUcUcbUcUcbbbb
void Material::ReserveGXMem(
    u8 texMapNum, u8 texSRTNum, u8 texCoordGenNum, u8 tevStageNum, bool allocTevSwap, u8 indStageNum, u8 indSRTNum,
    bool allocChanCtrl, bool allocMatCol, bool allocAlpComp, bool allocBlendMode
) {
    int tevSwapNum = allocTevSwap;
    int chanCtrlNum = allocChanCtrl;
    int matColNum = allocMatCol;
    int alpCompNum = allocAlpComp;
    int blendModeNum = allocBlendMode;
    if (mGXMemCap.texMap < texMapNum || mGXMemCap.texSRT < texSRTNum || mGXMemCap.texCoordGen < texCoordGenNum ||
        mGXMemCap.tevStage < tevStageNum || mGXMemCap.tevSwap < tevSwapNum || mGXMemCap.indStage < indStageNum ||
        mGXMemCap.indSRT < indSRTNum || mGXMemCap.chanCtrl < chanCtrlNum || mGXMemCap.matCol < matColNum ||
        mGXMemCap.alpComp < alpCompNum || mGXMemCap.blendMode < blendModeNum) {
        if (mpGXMem) {
            Layout::FreeMemory(mpGXMem);
            mpGXMem = nullptr;
            InitBitGXNums(&mGXMemCap);
            InitBitGXNums(&mGXMemNum);
        }
        mpGXMem = Layout::AllocMemory(
            (texCoordGenNum + chanCtrlNum + matColNum + tevSwapNum + alpCompNum + blendModeNum + indStageNum) * 4 +
            texMapNum * sizeof(TexMap) + tevStageNum * sizeof(TevStage) + texSRTNum * sizeof(TexSRT) +
            indSRTNum * sizeof(TexSRT)
        );

        if (mpGXMem) {
            mGXMemCap.texMap = texMapNum;
            mGXMemCap.texSRT = texSRTNum;
            mGXMemCap.texCoordGen = texCoordGenNum;
            mGXMemCap.indSRT = indSRTNum;
            mGXMemCap.indStage = indStageNum;
            mGXMemCap.tevSwap = tevSwapNum;
            mGXMemCap.tevStage = tevStageNum;
            mGXMemCap.chanCtrl = chanCtrlNum;
            mGXMemCap.matCol = matColNum;
            mGXMemCap.alpComp = alpCompNum;
            mGXMemCap.blendMode = blendModeNum;

            mGXMemNum.texSRT = mGXMemCap.texSRT;
            InitTexSRT(GetTexSRTAry(), mGXMemNum.texSRT);

            mGXMemNum.indSRT = mGXMemCap.indSRT;
            InitTexSRT(GetIndTexSRTAry(), mGXMemNum.indSRT);

            mGXMemNum.chanCtrl = mGXMemCap.chanCtrl;
            if (mGXMemNum.chanCtrl != 0) {
                GetChanCtrlAry()[0] = ChanCtrl();
            }

            mGXMemNum.matCol = mGXMemCap.matCol;
            if (mGXMemNum.matCol != 0) {
                GetMatColAry()[0] = 0xFFFFFFFF;
            }

            mGXMemNum.tevSwap = mGXMemCap.tevSwap;
            if (mGXMemNum.tevSwap != 0) {
                TevSwapMode *tevSwaps = GetTevSwapAry();
                tevSwaps[0].Set(GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
                tevSwaps[1].Set(GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
                tevSwaps[2].Set(GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
                tevSwaps[3].Set(GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            }

            mGXMemNum.alpComp = mGXMemCap.alpComp;
            if (mGXMemNum.alpComp != 0) {
                *GetAlphaComparePtr() = AlphaCompare();
            }

            mGXMemNum.blendMode = mGXMemCap.blendMode;
            if (mGXMemNum.blendMode != 0) {
                *GetBlendModePtr() = BlendMode();
            }
        }
    }
}

// GetTexMapAry__Q34nw4r3lyt8MaterialCFv
const TexMap *Material::GetTexMapAry() const {
    return detail::ConvertOffsToPtr<TexMap>(mpGXMem, 0); // Might not use the function?
}

// GetTexMapAry__Q34nw4r3lyt8MaterialFv
TexMap *Material::GetTexMapAry() {
    return (detail::ConvertOffsToPtr<TexMap>(mpGXMem, 0));
}

// GetTexSRTAry__Q34nw4r3lyt8MaterialCFv
const TexSRT *Material::GetTexSRTAry() const {
    return detail::ConvertOffsToPtr<TexSRT>(mpGXMem, CalcOffsetTexSRTAry(mGXMemCap));
}

// GetTexSRTAry__Q34nw4r3lyt8MaterialFv
TexSRT *Material::GetTexSRTAry() {
    return detail::ConvertOffsToPtr<TexSRT>(mpGXMem, CalcOffsetTexSRTAry(mGXMemCap));
}

// GetTexCoordGenAry__Q34nw4r3lyt8MaterialFv
TexCoordGen *Material::GetTexCoordGenAry() {
    return detail::ConvertOffsToPtr<TexCoordGen>(mpGXMem, CalcOffsetTexCoordGenAry(mGXMemCap));
}

// GetChanCtrlAry__Q34nw4r3lyt8MaterialFv
ChanCtrl *Material::GetChanCtrlAry() {
    return detail::ConvertOffsToPtr<ChanCtrl>(mpGXMem, CalcOffsetChanCtrlAry(mGXMemCap));
}

// GetMatColAry__Q34nw4r3lyt8MaterialFv
ut::Color *Material::GetMatColAry() {
    return detail::ConvertOffsToPtr<ut::Color>(mpGXMem, CalcOffsetMatColAry(mGXMemCap));
}

// GetTevSwapAry__Q34nw4r3lyt8MaterialFv
TevSwapMode *Material::GetTevSwapAry() {
    return detail::ConvertOffsToPtr<TevSwapMode>(mpGXMem, CalcOffsetTevSwapAry(mGXMemCap));
}

// GetAlphaComparePtr__Q34nw4r3lyt8MaterialFv
AlphaCompare *Material::GetAlphaComparePtr() {
    return detail::ConvertOffsToPtr<AlphaCompare>(mpGXMem, CalcOffsetGetAlphaCompare(mGXMemCap));
}

// GetBlendModePtr__Q34nw4r3lyt8MaterialFv
BlendMode *Material::GetBlendModePtr() {
    return detail::ConvertOffsToPtr<BlendMode>(mpGXMem, CalcOffsetBlendMode(mGXMemCap));
}

// GetIndirectStageAry__Q34nw4r3lyt8MaterialFv
IndirectStage *Material::GetIndirectStageAry() {
    return detail::ConvertOffsToPtr<IndirectStage>(mpGXMem, CalcOffsetIndirectStageAry(mGXMemCap));
}

// GetIndTexSRTAry__Q34nw4r3lyt8MaterialFv
TexSRT *Material::GetIndTexSRTAry() {
    return detail::ConvertOffsToPtr<TexSRT>(mpGXMem, CalcOffsetIndTexSRTAry(mGXMemCap));
}

// GetTevStageAry__Q34nw4r3lyt8MaterialFv
TevStage *Material::GetTevStageAry() {
    return detail::ConvertOffsToPtr<TevStage>(mpGXMem, CalcOffsetTevStageAry(mGXMemCap));
}

// SetName__Q34nw4r3lyt8MaterialFPCc
void Material::SetName(const char *name) {
    strncpy(mName, name, 20);
    mName[20] = '\0';
}

// SetTextureNum__Q34nw4r3lyt8MaterialFUc
void Material::SetTextureNum(u8 num) {
    if (num != 0) {
        TexMap *texMaps = GetTexMapAry();
        for (u32 i = mGXMemNum.texMap; i < num; i++) {
            new (&texMaps[i]) TexMap(); // Placement operator required
            // texMaps[i] = TexMap();
        }
    }
    mGXMemNum.texMap = num;
}

// SetTexCoordGenNum__Q34nw4r3lyt8MaterialFUc
void Material::SetTexCoordGenNum(u8 num) {
    if (num != 0) {
        TexCoordGen *texCoordGens = GetTexCoordGenAry();
        for (u32 i = mGXMemNum.texCoordGen; i < num; i++) {
            new (&texCoordGens[i]) TexCoordGen();
        }
    }
    mGXMemNum.texCoordGen = num;
}

// SetTevStageNum__Q34nw4r3lyt8MaterialFUc
void Material::SetTevStageNum(u8 num) {
    if (num != 0) {
        TevStage *tevStages = GetTevStageAry();
        for (u32 i = mGXMemNum.tevStage; i < num; i++) {
            new (&tevStages[i]) TevStage();
        }
    }
    mGXMemNum.tevStage = num;
}

// SetIndStageNum__Q34nw4r3lyt8MaterialFUc
void Material::SetIndStageNum(u8 num) {
    if (num != 0) {
        IndirectStage *indStages = GetIndirectStageAry();
        for (u32 i = mGXMemNum.indStage; i < num; i++) {
            new (&indStages[i]) IndirectStage();
        }
    }
    mGXMemNum.indStage = num;
}

// GetColorElement__Q34nw4r3lyt8MaterialCFUl

// SetColorElement__Q34nw4r3lyt8MaterialFUls
void Material::SetColorElement(u32 colorType, s16 value) {
    // Some sort of Enum ?
    //  0:[ MatCol->r]  1:[ MatCol->g]  2:[ MatCol->b]  3:[ MatCol->a
    //  4:[ TevCol->r]  5:[ TevCol->g]  6:[ TevCol->b]  7:[ TevCol->a]
    //  8:[ TevCol->r]  9:[ TevCol->g] 10:[ TevCol->b] 11:[ TevCol->a]
    // 12:[ TevCol->r] 13:[ TevCol->g] 14:[ TevCol->b] 15:[ TevCol->a]
    // 16:[TevKCol->r] 17:[TevKCol->g] 18:[TevKCol->b] 19:[TevKCol->a]
    // 20:[TevKCol->r] 21:[TevKCol->g] 22:[TevKCol->b] 23:[TevKCol->a]
    // 24:[TevKCol->r] 25:[TevKCol->g] 26:[TevKCol->b] 27:[TevKCol->a]
    // 28:[TevKCol->r] 29:[TevKCol->g] 30:[TevKCol->b] 31:[TevKCol->a]
    // clang-format off
    switch (colorType) {
    case 0: case 1: case 2: case 3: {
        if (mGXMemNum.matCol < 1 ) {
            break;
        }
        ut::Color *matCols = GetMatColAry();
        SetColorComponentValue(matCols, colorType , value);
        break;
    }
    case  4: case  5: case  6: case  7: case  8: case  9:
    case 10: case 11: case 12: case 13: case 14: case 15: {
        u32 regIdx = (colorType - 4) / 4;
        switch ((colorType - 4) % 4) {
        case 0:
            mTevCols[regIdx].r = value;
            break;
        case 1:
            mTevCols[regIdx].g = value;
            break;
        case 2:
            mTevCols[regIdx].b = value;
            break;
        case 3:
            mTevCols[regIdx].a = value;
            break;
        }
    } break;
    case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23: 
    case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31: {
        u32 regIdx = (colorType - 16) / 4;
        SetColorComponentValue(&mTevKCols[regIdx], (colorType - 16) , value);
    } break;
    }
    // clang-format on
}

// SetupGX__Q34nw4r3lyt8MaterialFbUc
bool Material::SetupGX(bool bModVtxCol, u8 alpha) {
    bool bUseVtxCol = true;
    bool bUseMatCol = false;
    GXSetNumChans(1);
    if (IsChanCtrlCap()) {
        const ChanCtrl *chanCtrls = GetChanCtrlAry();
        GXSetChanCtrl(GX_COLOR0, false, GX_SRC_REG, chanCtrls->GetColorSrc(), GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_ALPHA0, false, GX_SRC_REG, chanCtrls->GetAlphaSrc(), GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);

        bUseVtxCol = chanCtrls->GetColorSrc() == GX_SRC_VTX || chanCtrls->GetAlphaSrc() == GX_SRC_VTX;
        bUseMatCol = chanCtrls->GetColorSrc() == GX_SRC_REG || chanCtrls->GetAlphaSrc() == GX_SRC_REG;
    } else {
        GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }

    // Why set to false and then check if true?
    bool bUseRasStage = false;
    if (bUseVtxCol) {
        bUseRasStage = bUseRasStage || bModVtxCol;
    }

    if (bUseMatCol) {
        ut::Color matCol = 0xFFFFFFFF;
        if (IsMatColorCap()) {
            matCol = GetMatColAry()[0];
        }
        matCol = detail::MultipleAlpha(matCol, alpha);
        GXSetChanMatColor(GX_COLOR0A0, matCol);
        bUseRasStage = bUseRasStage || matCol != 0xFFFFFFFF;
    }
    bool bSetTexMtx = false;
    bool bUseTexMtx[10];
    for (int i = 0; i < 10; i++) {
        bUseTexMtx[i] = false;
    }
    GXSetNumTexGens(mGXMemNum.texCoordGen);
    if (mGXMemNum.texCoordGen != 0) {
        const TexCoordGen *texCoordGens = GetTexCoordGenAry();
        for (int i = 0; i < mGXMemNum.texCoordGen; i++) {
            u32 texMtx = texCoordGens[i].GetTexMtx();
            if (texCoordGens[i].GetTexGenType() == GX_TEXMAP1 && texMtx != 0x3C) {
                bUseTexMtx[GetTexMtxIdx(texMtx)] = true;
                bSetTexMtx = true;
            }
            GXSetTexCoordGen2(
                (GXTexCoordID)i, texCoordGens[i].GetTexGenType(), texCoordGens[i].GetTexGenSrc(), texMtx, FALSE, 0x7D
            );
        }
    }
    if (bSetTexMtx) {
        const TexSRT *texSRTs = GetTexSRTAry();

        for (u8 i = 0; i < mGXMemNum.texSRT; i++) {
            if (bUseTexMtx[i]) {
                math::MTX34 texMtx;
                CalcTextureMtx(&texMtx, texSRTs[i]);
                GXLoadTexMtxImm(texMtx, GetTexMtx(i), GX_MTX2x4);
            }
        }
    }

    if (mGXMemNum.texMap) {
        u32 tlutID = 0;
        u32 bigTlutID = 16;

        const TexMap *texMaps = GetTexMapAry();
        for (int i = 0; i < mGXMemNum.texMap; i++) {
            const TexMap &texMap = texMaps[i];
            GXTexObj texObj;
            texMap.Get(&texObj);
            if (detail::IsCITexelFormat(texMap.GetTexelFormat())) {
                u32 tlutName;
                if (texMap.GetTexelFormat() == 10) {
                    tlutName = bigTlutID++;
                } else {
                    tlutName = tlutID++;
                }
                GXInitTexObjTlut(&texObj, tlutName);
                GXTlutObj tlutObj;
                texMap.Get(&tlutObj);
                GXLoadTlut(&tlutObj, tlutName);
            }
            GXLoadTexObj(&texObj, (GXTexMapID)i);
        }
    }

    GXSetTevColorS10(GX_TEVREG0, mTevCols[0]);
    GXSetTevColorS10(GX_TEVREG1, mTevCols[1]);
    GXSetTevColorS10(GX_TEVREG2, mTevCols[2]);
    GXSetTevKColor(GX_KCOLOR0, mTevKCols[0]);
    GXSetTevKColor(GX_KCOLOR1, mTevKCols[1]);
    GXSetTevKColor(GX_KCOLOR2, mTevKCols[2]);
    GXSetTevKColor(GX_KCOLOR3, mTevKCols[3]);

    if (IsTevSwapCap()) {
        const TevSwapMode *tevSwaps = GetTevSwapAry();
        for (int i = 0; i < 4; i++) {
            GXSetTevSwapModeTable(
                (GXTevSwapSel)i, tevSwaps[i].GetR(), tevSwaps[i].GetG(), tevSwaps[i].GetB(), tevSwaps[i].GetA()
            );
        }
    } else {
        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
    }

    bool bSetIndTexMtx = false;
    bool bUseIndTexMtx[3];
    for (int i = 0; i < 3; i++) {
        bUseIndTexMtx[i] = false;
    }
    if (mGXMemNum.tevStage != 0) {
        GXSetNumTevStages(mGXMemNum.tevStage);
        const TevStage *tevStages = GetTevStageAry();
        for (int i = 0; i < mGXMemNum.tevStage; i++) {
            GXTevStageID tevStage = (GXTevStageID)i;
            GXSetTevOrder(
                tevStage, tevStages[i].GetTexCoordGen(), tevStages[i].GetTexMap(), tevStages[i].GetColorChan()
            );
            GXSetTevSwapMode(tevStage, tevStages[i].GetRasSwapSel(), tevStages[i].GetTexSwapSel());
            GXSetTevColorIn(
                tevStage, tevStages[i].GetColorInA(), tevStages[i].GetColorInB(), tevStages[i].GetColorInC(),
                tevStages[i].GetColorInD()
            );
            GXSetTevColorOp(
                tevStage, tevStages[i].GetColorOp(), tevStages[i].GetColorBias(), tevStages[i].GetColorScale(),
                tevStages[i].IsColorClamp(), tevStages[i].GetColorOutReg()
            );
            GXSetTevKColorSel(tevStage, tevStages[i].GetKColorSel());
            GXSetTevAlphaIn(
                tevStage, tevStages[i].GetAlphaInA(), tevStages[i].GetAlphaInB(), tevStages[i].GetAlphaInC(),
                tevStages[i].GetAlphaInD()
            );
            GXSetTevAlphaOp(
                tevStage, tevStages[i].GetAlphaOp(), tevStages[i].GetAlphaBias(), tevStages[i].GetAlphaScale(),
                tevStages[i].IsAlphaClamp(), tevStages[i].GetAlphaOutReg()
            );
            GXSetTevKAlphaSel(tevStage, tevStages[i].GetKAlphaSel());
            GXIndTexMtxID indMtxSel = tevStages[i].GetIndMtxSel();
            GXSetTevIndirect(
                tevStage, tevStages[i].GetIndStage(), tevStages[i].GetIndFormat(), tevStages[i].GetIndBiasSel(),
                indMtxSel, tevStages[i].GetIndWrapS(), tevStages[i].GetIndWrapT(), tevStages[i].IsIndAddPrev(),
                tevStages[i].IsIndUtcLod(), tevStages[i].GetIndAlphaSel()
            );
            if (1 <= indMtxSel && indMtxSel <= 3) {
                bUseIndTexMtx[indMtxSel - 1] = true;
                bSetIndTexMtx = true;
            }
        }
        bUseRasStage = true;
    } else {
        static GXTevKColorSel kColSels[8] = {
            GX_TEV_KCSEL_K3_A, GX_TEV_KCSEL_K3_B, GX_TEV_KCSEL_K3_G, GX_TEV_KCSEL_K3_R,
            GX_TEV_KCSEL_K2_A, GX_TEV_KCSEL_K2_B, GX_TEV_KCSEL_K2_G, GX_TEV_KCSEL_K2_R,
        };
        static GXTevKAlphaSel kAlpSels[8] = {
            GX_TEV_KASEL_K3_A, GX_TEV_KASEL_K3_B, GX_TEV_KASEL_K3_G, GX_TEV_KASEL_K3_R,
            GX_TEV_KASEL_K2_A, GX_TEV_KASEL_K2_B, GX_TEV_KASEL_K2_G, GX_TEV_KASEL_K2_R,
        };
        u8 tevStageID = 0;
        if (mGXMemNum.texMap == 0) {
            GXTevStageID tevStage = GX_TEVSTAGE0;
            GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_C1, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_A1, GX_CA_RASA, GX_CA_ZERO);
            bUseRasStage = true;
            tevStageID = 1;
        } else {
            if (mGXMemNum.texMap == 1) {
                GXTevStageID tevStage = GX_TEVSTAGE0;
                GXSetTevOrder(tevStage, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorIn(tevStage, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevAlphaIn(tevStage, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
                tevStageID = 1;
            } else {
                if (mGXMemNum.texMap == 2) {
                    GXTevStageID tevStage = GX_TEVSTAGE0;
                    GXSetTevOrder(tevStage, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                    GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                    GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);

                    tevStage = GX_TEVSTAGE1;
                    GXSetTevOrder(tevStage, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                    GXSetTevColorIn(tevStage, GX_CC_TEXC, GX_CC_CPREV, GX_CC_KONST, GX_CC_ZERO);
                    GXSetTevAlphaIn(tevStage, GX_CA_TEXA, GX_CA_APREV, GX_CA_KONST, GX_CA_ZERO);
                    GXSetTevKColorSel(tevStage, kColSels[0]);
                    GXSetTevKAlphaSel(tevStage, kAlpSels[0]);
                    tevStageID = 2;

                } else {
                    for (int i = 0; i < mGXMemNum.texMap; i++) {
                        GXTevStageID tevStage = (GXTevStageID)tevStageID;
                        GXSetTevOrder(tevStage, (GXTexCoordID)i, (GXTexMapID)i, GX_COLOR_NULL);
                        GXTevColorArg colDIn = i == 0 ? GX_CC_ZERO : GX_CC_CPREV;
                        GXTevAlphaArg alpDIn = i == 0 ? GX_CA_ZERO : GX_CA_APREV;
                        GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, colDIn);
                        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, alpDIn);
                        GXSetTevKColorSel(tevStage, kColSels[i]);
                        GXSetTevKAlphaSel(tevStage, kAlpSels[i]);
                        tevStageID++;
                    }
                }
                if (mTevCols[0] != DefaultBlackColor || mTevCols[1] != DefaultWhiteColor) {
                    GXTevStageID tevStage = (GXTevStageID)tevStageID;
                    GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
                    GXSetTevColorIn(tevStage, GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
                    GXSetTevAlphaIn(tevStage, GX_CA_A0, GX_CA_A1, GX_CA_APREV, GX_CA_ZERO);
                    tevStageID++;
                }
            }
            if (bUseRasStage) {
                GXTevStageID tevStage = (GXTevStageID)tevStageID;
                GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
                tevStageID++;
            }
        }

        const u8 tevStageNum = tevStageID;

        for (u8 id = 0; id < tevStageNum; id++) {
            GXTevStageID tevStage = (GXTevStageID)id;
            GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevDirect(tevStage);
            GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
        }
        GXSetNumTevStages(tevStageNum);
    }
    if (bSetIndTexMtx) {
        const TexSRT *indTexSRTs = GetIndTexSRTAry();
        for (int i = 0; i < mGXMemNum.indSRT; i++) {
            if (bUseIndTexMtx[i]) {
                f32 mtx[2][3];
                CalcIndTexMtx(mtx, indTexSRTs[i]);
                SetIndTexMtx((GXIndTexMtxID)(i + 1), mtx);
            }
        }
    }

    GXSetNumIndStages(mGXMemNum.indStage);
    if (mGXMemNum.indStage != 0) {
        const IndirectStage *indirectStages = GetIndirectStageAry();
        for (int i = 0; i < mGXMemNum.indStage; i++) {
            GXIndTexStageID indStage = (GXIndTexStageID)i;
            GXSetIndTexOrder(indStage, indirectStages[i].GetTexCoordGen(), indirectStages[i].GetTexMap());
            GXSetIndTexCoordScale(indStage, indirectStages[i].GetScaleS(), indirectStages[i].GetScaleT());
        }
    }
    if (IsAlphaCompareCap()) {
        const AlphaCompare *pAlpComp = GetAlphaComparePtr();
        GXSetAlphaCompare(
            pAlpComp->GetComp0(), pAlpComp->GetRef0(), pAlpComp->GetOp(), pAlpComp->GetComp1(), pAlpComp->GetRef1()
        );
    } else {
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    }
    if (IsBlendModeCap()) {
        const BlendMode *pBlendMode = GetBlendModePtr();
        GXSetBlendMode(
            pBlendMode->GetType(), pBlendMode->GetSrcFactor(), pBlendMode->GetDstFactor(), pBlendMode->GetOp()
        );

    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    }
    return bUseRasStage && bUseVtxCol;
}

// BindAnimation__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform
void Material::BindAnimation(AnimTransform *pAnimTrans) {
    pAnimTrans->Bind(this, false);
}

// UnbindAnimation__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform
void Material::UnbindAnimation(AnimTransform *pAnimTrans) {
    detail::UnbindAnimationLink(GetAnimationList(), pAnimTrans);
}

// UnbindAllAnimation__Q34nw4r3lyt8MaterialFv
void Material::UnbindAllAnimation() {
    UnbindAnimation(nullptr);
}

// Animate__Q34nw4r3lyt8MaterialFv
void Material::Animate() {
    for (ut::LinkList<AnimationLink, 0>::Iterator it = mAnimList.GetBeginIter(); it != mAnimList.GetEndIter(); it++) {
        if (it->IsEnable()) {
            AnimTransform *pAnimTrans = it->GetAnimTransform();
            pAnimTrans->Animate(it->GetIndex(), this);
        }
    }
}

// AddAnimationLink__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimationLink
void Material::AddAnimationLink(AnimationLink *pAnimationLink) {
    mAnimList.PushBack(pAnimationLink);
}

// FindAnimationLink__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform
AnimationLink *Material::FindAnimationLink(AnimTransform *pAnimTrans) {
    return detail::FindAnimationLink(&mAnimList, pAnimTrans);
}

// FindAnimationLink__Q34nw4r3lyt8MaterialFRCQ34nw4r3lyt12AnimResource
AnimationLink *Material::FindAnimationLink(const AnimResource &animRes) {
    return detail::FindAnimationLink(&mAnimList, animRes);
}

// SetAnimationEnable__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransformb
void Material::SetAnimationEnable(AnimTransform *pAnimTrans, bool bEnable) {
    AnimationLink *pAnimLink = FindAnimationLink(pAnimTrans);
    if (pAnimLink) {
        pAnimLink->SetEnable(bEnable);
    }
}

// SetAnimationEnable__Q34nw4r3lyt8MaterialFRCQ34nw4r3lyt12AnimResourceb
void Material::SetAnimationEnable(const AnimResource &animRes, bool bEnable) {
    AnimationLink *pAnimLink = FindAnimationLink(animRes);
    if (pAnimLink) {
        pAnimLink->SetEnable(bEnable);
    }
}

namespace detail {

// GetTextureSize__Q34nw4r3lyt6detailFPQ34nw4r3lyt8MaterialUc
Size GetTextureSize(Material *pMaterial, u8 texMapIdx) {
    if (texMapIdx >= pMaterial->GetTextureNum()) {
        return Size();
    }
    const TexMap &texMap = pMaterial->GetTexMapAry()[texMapIdx];
    return Size(texMap.GetWidth(), texMap.GetHeight());
}

} // namespace detail

} // namespace lyt

} // namespace nw4r
