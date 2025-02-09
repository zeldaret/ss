#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void CalcWorld(math::MTX34* pModelMtxArray, u32* pModelMtxAttribArray,
               const u8* pByteCode, const math::MTX34* pBaseMtx, ResMdl mdl,
               AnmObjChr* pAnmChr, FuncObjCalcWorld* pFuncObj, u32 rootAttrib) {

#define pCalcCmd reinterpret_cast<const ResByteCodeData::CalcParams*>(pByteCode)
#define pMtxDupCmd                                                             \
    reinterpret_cast<const ResByteCodeData::MtxDupParams*>(pByteCode)

    u8 c;

    ChrAnmResult result;
    const ChrAnmResult* pResult = NULL;

    if (pByteCode == NULL) {
        pByteCode = mdl.GetResByteCode("NodeTree");

        if (pByteCode == NULL) {
            return;
        }
    }

    bool xsi = mdl.GetResMdlInfo().GetScalingRule() == SCALINGRULE_SOFTIMAGE;

    bool ignoreTrans = detail::WorldMtxAttr::IsIgnoreTrans(rootAttrib);

    math::VEC3* pScaleArray = detail::workmem::GetScaleTemporary();
    pScaleArray[0].x = pScaleArray[0].y = pScaleArray[0].z = 1.0f;

    math::MTX34Copy(pModelMtxArray, pBaseMtx);

    u32* pMtxIDArray = detail::workmem::GetMtxIDTemporary();
    u32 numMtxID = 0;

    pModelMtxAttribArray[0] = rootAttrib;

    while ((c = *pByteCode) != ResByteCodeData::END) {
        if (c == ResByteCodeData::CALC) {
            u32 nodeID = (pCalcCmd->nodeIdHi << 8) + pCalcCmd->nodeIdLo;

            u32 parentMtxID =
                (pCalcCmd->parentMtxIdHi << 8) + pCalcCmd->parentMtxIdLo;

            ResNode node = mdl.GetResNode(nodeID);

            u32 mtxID = node.GetMtxID();
            pMtxIDArray[numMtxID++] = mtxID;

            math::MTX34* pModelMtx = &pModelMtxArray[mtxID];
            math::VEC3* pScale = &pScaleArray[mtxID];

            math::MTX34* pParentModelMtx = &pModelMtxArray[parentMtxID];
            math::VEC3* pParentScale = &pScaleArray[parentMtxID];

            u32 parentAttr = pModelMtxAttribArray[parentMtxID];

            if (pAnmChr != NULL) {
                pResult = pAnmChr->GetResult(&result, node.GetID());
            }

            if (pAnmChr == NULL || pResult->flags == 0) {
                node.CalcChrAnmResult(&result);
                pResult = &result;
            }

            if (nodeID != 0 && ignoreTrans) {
                if (pResult != &result) {
                    result = *pResult;
                    pResult = &result;
                }

                result.flags |= ChrAnmResult::FLAG_PATCH_TRANS;
                node.PatchChrAnmResult(&result);
            }

            if (pFuncObj != NULL) {
                if (pResult != &result) {
                    result = *pResult;
                    pResult = &result;
                }

                pFuncObj->CheckCallbackA(nodeID, &result, mdl);
            }

            if (xsi) {
                pModelMtxAttribArray[mtxID] = detail::dcc::CalcWorldMtx_Xsi(
                    pModelMtx, pScale, pParentModelMtx, pParentScale,
                    parentAttr, pResult);
            } else if (pResult->flags & ChrAnmResult::FLAG_SSC_APPLY) {
                pModelMtxAttribArray[mtxID] =
                    detail::dcc::CalcWorldMtx_Maya_SSC_Apply(
                        pModelMtx, pScale, pParentModelMtx, pParentScale,
                        parentAttr, pResult);
            } else {
                pModelMtxAttribArray[mtxID] = detail::dcc::CalcWorldMtx_Basic(
                    pModelMtx, pScale, pParentModelMtx, pParentScale,
                    parentAttr, pResult);
            }

            pModelMtxAttribArray[mtxID] = detail::WorldMtxAttr::SetBillboard(
                pModelMtxAttribArray[mtxID], node.GetBillboardMode());

            if (pFuncObj != NULL) {
                pFuncObj->CheckCallbackB(nodeID, pModelMtx, pScale,
                                         &pModelMtxAttribArray[mtxID], mdl);
            }

            pByteCode += sizeof(ResByteCodeData::CalcParams);

        } else /* Assume MTXDUP */ {
            u32 toMtxID = (pMtxDupCmd->toMtxIdHi << 8) + pMtxDupCmd->toMtxIdLo;

            u32 fromMtxID =
                (pMtxDupCmd->fromMtxIdHi << 8) + pMtxDupCmd->fromMtxIdLo;

            pMtxIDArray[numMtxID++] = toMtxID;
            pModelMtxAttribArray[toMtxID] = pModelMtxAttribArray[fromMtxID];

            math::MTX34Copy(&pModelMtxArray[toMtxID],
                            &pModelMtxArray[fromMtxID]);

            pScaleArray[toMtxID] = pScaleArray[fromMtxID];

            pByteCode += sizeof(ResByteCodeData::MtxDupParams);
        }
    }

    for (u32 i = 0; i < numMtxID; i++) {
        u32 mtxID = pMtxIDArray[i];

        if (pScaleArray[mtxID].x == 1.0f && pScaleArray[mtxID].y == 1.0f &&
            pScaleArray[mtxID].z == 1.0f) {
            continue;
        }

        math::MTX34Scale(&pModelMtxArray[mtxID], &pModelMtxArray[mtxID],
                         &pScaleArray[mtxID]);
    }

    if (pFuncObj != NULL) {
        pFuncObj->CheckCallbackC(pModelMtxArray, mdl);
    }

#undef pCalcCmd
#undef pMtxDupCmd
}

void CalcWorld(math::MTX34* pModelMtxArray, u32* pModelMtxAttribArray,
               const u8* pByteCode, const math::MTX34* pBaseMtx, ResMdl mdl,
               AnmObjChr* pAnmChr, FuncObjCalcWorld* pFuncObj) {

    CalcWorld(pModelMtxArray, pModelMtxAttribArray, pByteCode, pBaseMtx, mdl,
              pAnmChr, pFuncObj, detail::WorldMtxAttr::GetRootMtxAttr());
}

void CalcSkinning(math::MTX34* pModelMtxArray, u32* pModelMtxAttribArray,
                  const ResMdl mdl, const u8* pByteCode) {

    // Allows struct offsets inside assembly
    using nw4r::math::MTX34;

#define pWeightCmd                                                             \
    reinterpret_cast<const ResByteCodeData::WeightParams*>(pByteCode)
#define pWeightEntry                                                           \
    reinterpret_cast<const ResByteCodeData::WeightEntry*>(pByteCode)
#define pEvpMtxCmd                                                             \
    reinterpret_cast<const ResByteCodeData::EvpMtxParams*>(pByteCode)

    u8 c;

    if (pByteCode == NULL) {
        pByteCode = mdl.GetResByteCode("NodeMix");

        if (pByteCode == NULL) {
            return;
        }
    }

    math::MTX34* pSkinMtxArray = detail::workmem::GetSkinningMtxTemporary();

    while ((c = *pByteCode) != ResByteCodeData::END) {
        if (c == ResByteCodeData::WEIGHT) {
            u32 targetMtxID = (pWeightCmd->tgtIdHi << 8) + pWeightCmd->tgtIdLo;
            u32 numBlendMtx = pWeightCmd->numBlendMtx;

            pModelMtxAttribArray[targetMtxID] =
                detail::WorldMtxAttr::GetRootMtxAttr();

            pByteCode += sizeof(ResByteCodeData::WeightParams);

            register f32 M00, M02;
            register f32 M10, M12;
            register f32 M20, M22;

            M00 = 0.0f;

            // clang-format off
            asm {
                ps_merge00 M00, M00, M00
                ps_merge00 M02, M00, M00
                ps_merge00 M10, M00, M00
                ps_merge00 M12, M00, M00
                ps_merge00 M20, M00, M00
                ps_merge00 M22, M00, M00
            }
            // clang-format on

            for (u32 i = 0; i < numBlendMtx; i++) {
                u32 mtxID =
                    (pWeightEntry->mtxIdHi << 8) + pWeightEntry->mtxIdLo;

                u32 iraito = pWeightEntry->fWeight0 << 24 |
                             pWeightEntry->fWeight1 << 16 |
                             pWeightEntry->fWeight2 << 8 |
                             pWeightEntry->fWeight3;

                register f32 R = *reinterpret_cast<f32*>(&iraito);

                register f32 T00, T02;
                register f32 T10, T12;
                register f32 T20, T22;

                register math::MTX34* pT = &pSkinMtxArray[mtxID];

                // clang-format off
                asm {
                    psq_l T00, MTX34._00(pT), 0, 0
                    psq_l T02, MTX34._02(pT), 0, 0
                    psq_l T10, MTX34._10(pT), 0, 0
                    psq_l T12, MTX34._12(pT), 0, 0
                    psq_l T20, MTX34._20(pT), 0, 0
                    psq_l T22, MTX34._22(pT), 0, 0

                    ps_madds0 M00, T00, R, M00
                    ps_madds0 M02, T02, R, M02
                    ps_madds0 M10, T10, R, M10
                    ps_madds0 M12, T12, R, M12
                    ps_madds0 M20, T20, R, M20
                    ps_madds0 M22, T22, R, M22
                }
                // clang-format on

                pModelMtxAttribArray[targetMtxID] &=
                    pModelMtxAttribArray[mtxID];

                pByteCode += sizeof(ResByteCodeData::WeightEntry);
            }

            register math::MTX34* pTargetMtx = &pModelMtxArray[targetMtxID];

            // clang-format off
            asm {
                psq_st M00, MTX34._00(pTargetMtx), 0, 0
                psq_st M02, MTX34._02(pTargetMtx), 0, 0
                psq_st M10, MTX34._10(pTargetMtx), 0, 0
                psq_st M12, MTX34._12(pTargetMtx), 0, 0
                psq_st M20, MTX34._20(pTargetMtx), 0, 0
                psq_st M22, MTX34._22(pTargetMtx), 0, 0
            }
            // clang-format on

        } else /* Assume EVPMTX */ {
            u32 mtxID = (pEvpMtxCmd->mtxIdHi << 8) + pEvpMtxCmd->mtxIdLo;
            u32 nodeID = (pEvpMtxCmd->nodeIdHi << 8) + pEvpMtxCmd->nodeIdLo;

            math::MTX34Mult(&pSkinMtxArray[mtxID], &pModelMtxArray[mtxID],
                            static_cast<const math::MTX34*>(
                                &mdl.GetResNode(nodeID).ref().invModelMtx));

            pByteCode += sizeof(ResByteCodeData::EvpMtxParams);
        }
    }

#undef pWeightCmd
#undef pWeightEntry
#undef pEvpMtxCmd
}

} // namespace g3d
} // namespace nw4r
