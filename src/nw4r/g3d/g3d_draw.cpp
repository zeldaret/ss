#include "nw4r/g3d.h" // IWYU pragma: export

#include "nw4r/ut.h" // IWYU pragma: export

#include "rvl/BASE.h" // IWYU pragma: export

#include <algorithm>

namespace nw4r {
namespace g3d {
namespace detail {

G3DState::IndMtxOp *GetIndMtxOp(ResMat mat, ResNode node, ResShp shp) {
    if (!mat.IsValid() || !shp.IsValid()) {
        return NULL;
    }

    ResMatMisc misc = mat.GetResMatMisc();

    ResMatMiscData::IndirectMethod method[GX_ITM_2 - GX_ITM_0 + 1];
    s8 lightRef[GX_ITM_2 - GX_ITM_0 + 1];

    misc.GetIndirectTexMtxCalcMethod(GX_ITM_0, &method[GX_ITM_0 - 1], &lightRef[GX_ITM_0 - 1]);

    misc.GetIndirectTexMtxCalcMethod(GX_ITM_1, &method[GX_ITM_1 - 1], &lightRef[GX_ITM_1 - 1]);

    misc.GetIndirectTexMtxCalcMethod(GX_ITM_2, &method[GX_ITM_2 - 1], &lightRef[GX_ITM_2 - 1]);

    if (method[GX_ITM_0 - 1] == ResMatMiscData::WARP && method[GX_ITM_1 - 1] == ResMatMiscData::WARP &&
        method[GX_ITM_2 - 1] == ResMatMiscData::WARP) {
        return NULL;
    }

    bool nrmMtxInit = false;
    math::MTX34 nrmMtx;

    u32 i;

    G3DState::IndMtxOp &rOp = *G3DState::GetIndMtxOp();
    rOp.Reset();

    for (i = 0; i < GX_ITM_2 - GX_ITM_0 + 1; i++) {
        GXIndTexMtxID id = static_cast<GXIndTexMtxID>(i + 1);

        if (method[i] == ResMatMiscData::WARP) {
            continue;
        }

        if (!nrmMtxInit) {
            nrmMtxInit = true;

            if (shp.ptr()->curMtxIdx >= 0) {
                if (shp.ptr()->curMtxIdx == node.GetMtxID()) {
                    const math::MTX33 *pViewNrm = G3DState::GetViewNrmMtxPtr(shp.ptr()->curMtxIdx);

                    nrmMtx._00 = pViewNrm->_00;
                    nrmMtx._01 = pViewNrm->_01;
                    nrmMtx._02 = pViewNrm->_02;

                    nrmMtx._10 = pViewNrm->_10;
                    nrmMtx._11 = pViewNrm->_11;
                    nrmMtx._12 = pViewNrm->_12;

                    nrmMtx._20 = pViewNrm->_20;
                    nrmMtx._21 = pViewNrm->_21;
                    nrmMtx._22 = pViewNrm->_22;
                } else {
                    ResMdl mdl = mat.GetParent();

                    int shpNodeID = mdl.GetResMdlInfo().GetNodeIDFromMtxID(shp.ptr()->curMtxIdx);

                    ResNode shpNode = mdl.GetResNode(shpNodeID);

                    math::MTX34Mult(
                        &nrmMtx, static_cast<math::MTX34 *>(&shpNode.ref().invModelMtx),
                        static_cast<math::MTX34 *>(&node.ref().modelMtx)
                    );

                    const math::MTX33 *pViewNrm = G3DState::GetViewNrmMtxPtr(shp.ptr()->curMtxIdx);

                    // clang-format off
                    math::MTX34 viewNrm34(
                        pViewNrm->_00, pViewNrm->_01, pViewNrm->_02, 0.0f,
                        pViewNrm->_10, pViewNrm->_11, pViewNrm->_12, 0.0f,
                        pViewNrm->_20, pViewNrm->_21, pViewNrm->_22, 0.0f);
                    // clang-format on

                    math::MTX34Mult(&nrmMtx, &viewNrm34, &nrmMtx);
                }

                nrmMtx._03 = nrmMtx._13 = nrmMtx._23 = 0.0f;

                math::VEC3 col0(nrmMtx._00, nrmMtx._10, nrmMtx._20);
                math::VEC3Normalize(&col0, &col0);
                nrmMtx._00 = col0.x;
                nrmMtx._10 = col0.y;
                nrmMtx._20 = col0.z;

                math::VEC3 col1(nrmMtx._01, nrmMtx._11, nrmMtx._21);
                math::VEC3Normalize(&col1, &col1);
                nrmMtx._01 = col1.x;
                nrmMtx._11 = col1.y;
                nrmMtx._21 = col1.z;

                math::VEC3 col2(nrmMtx._02, nrmMtx._12, nrmMtx._22);
                math::VEC3Normalize(&col2, &col2);
                nrmMtx._02 = col2.x;
                nrmMtx._12 = col2.y;
                nrmMtx._22 = col2.z;

            } else {
                math::MTX34Copy(&nrmMtx, G3DState::GetCameraMtxPtr());
            }
        }

        const LightObj *pLight = G3DState::GetLightObj(lightRef[i]);

        if (pLight != NULL && pLight->IsEnable()) {
            math::VEC3 lightVec;

            if (pLight->IsSpotLight()) {
                pLight->GetLightDir(&lightVec);

                if (lightVec.x == 0.0f && lightVec.y == 0.0f && lightVec.z == 0.0f) {
                    pLight->GetLightPos(&lightVec);
                    lightVec = -lightVec;
                    math::VEC3Normalize(&lightVec, &lightVec);
                }
            } else if (pLight->IsDiffuseLight()) {
                pLight->GetLightPos(&lightVec);
                lightVec = -lightVec;
                math::VEC3Normalize(&lightVec, &lightVec);
            } else {
                math::VEC3 H;
                pLight->GetLightDir(&H);

                lightVec.x = -2.0f * H.z * H.x;
                lightVec.y = -2.0f * H.z * H.y;
                lightVec.z = -2.0f * H.z * H.z + 1.0f;
                math::VEC3Normalize(&lightVec, &lightVec);
            }

            rOp.SetNrmMapMtx(id, &lightVec, &nrmMtx, method[i]);
        } else {
            rOp.SetNrmMapMtx(id, NULL, &nrmMtx, method[i]);
        }
    }

    return &rOp;
}

} // namespace detail

namespace {

void SetupDraw1Mat1ShpSwap(Draw1Mat1ShpSwap *pSwap, DrawResMdlReplacement *pReplacement, u32 id) {
    if (pReplacement->texObjDataArray != NULL) {
        pSwap->texObj = ResTexObj(&pReplacement->texObjDataArray[id]);
    } else {
        pSwap->texObj = ResTexObj(NULL);
    }

    if (pReplacement->tlutObjDataArray != NULL) {
        pSwap->tlutObj = ResTlutObj(&pReplacement->tlutObjDataArray[id]);
    } else {
        pSwap->tlutObj = ResTlutObj(NULL);
    }

    if (pReplacement->texSrtDataArray != NULL) {
        pSwap->texSrt = ResTexSrt(&pReplacement->texSrtDataArray[id]);
    } else {
        pSwap->texSrt = ResTexSrt(NULL);
    }

    if (pReplacement->chanDataArray != NULL) {
        pSwap->chan = ResMatChan(&pReplacement->chanDataArray[id]);
    } else {
        pSwap->chan = ResMatChan(NULL);
    }

    if (pReplacement->genModeDataArray != NULL) {
        pSwap->genMode = ResGenMode(&pReplacement->genModeDataArray[id]);
    } else {
        pSwap->genMode = ResGenMode(NULL);
    }

    if (pReplacement->matMiscDataArray != NULL) {
        pSwap->misc = ResMatMisc(&pReplacement->matMiscDataArray[id]);
    } else {
        pSwap->misc = ResMatMisc(NULL);
    }

    if (pReplacement->pixDLArray != NULL) {
        pSwap->pix = ResMatPix(&pReplacement->pixDLArray[id]);
    } else {
        pSwap->pix = ResMatPix(NULL);
    }

    if (pReplacement->tevColorDLArray != NULL) {
        pSwap->tevColor = ResMatTevColor(&pReplacement->tevColorDLArray[id]);
    } else {
        pSwap->tevColor = ResMatTevColor(NULL);
    }

    if (pReplacement->indMtxAndScaleDLArray != NULL) {
        pSwap->indMtxAndScale = ResMatIndMtxAndScale(&pReplacement->indMtxAndScaleDLArray[id]);
    } else {
        pSwap->indMtxAndScale = ResMatIndMtxAndScale(NULL);
    }

    if (pReplacement->texCoordGenDLArray != NULL) {
        pSwap->texCoordGen = ResMatTexCoordGen(&pReplacement->texCoordGenDLArray[id]);
    } else {
        pSwap->texCoordGen = ResMatTexCoordGen(NULL);
    }

    if (pReplacement->tevDataArray != NULL) {
        pSwap->tev = ResTev(&pReplacement->tevDataArray[id]);
    } else {
        pSwap->tev = ResTev(NULL);
    }

    pSwap->vtxPosTable = pReplacement->vtxPosTable;
    pSwap->vtxNrmTable = pReplacement->vtxNrmTable;
    pSwap->vtxClrTable = pReplacement->vtxClrTable;
}

inline bool FrontToBack(const detail::workmem::MdlZ &rLhs, const detail::workmem::MdlZ &rRhs) {
    if (rLhs.priority < rRhs.priority) {
        return true;
    }

    if (rLhs.priority > rRhs.priority) {
        return false;
    }

    if (rLhs.Z > rRhs.Z) {
        return true;
    }

    return false;
}

inline bool BackToFront(const detail::workmem::MdlZ &rLhs, const detail::workmem::MdlZ &rRhs) {
    if (rLhs.priority < rRhs.priority) {
        return true;
    }

    if (rLhs.priority > rRhs.priority) {
        return false;
    }

    if (rLhs.Z < rRhs.Z) {
        return true;
    }

    if (rLhs.Z == rRhs.Z && rLhs.matID < rRhs.matID) {
        return true;
    }

    return false;
}

void DrawResMdlLoop(const ResMdl mdl, const u8 *pByteCode, u32 drawMode) {
#define pDrawCmd reinterpret_cast<const ResByteCodeData::DrawParams *>(pByteCode)

    u8 c;

    ResMat mat;
    ResMat prevMat;

    ResShp shp;
    ResNode node;

    bool ignoreMat = (drawMode & RESMDL_DRAWMODE_IGNORE_MATERIAL);
    u32 ctrl = DRAW1MAT1SHP_CTRL_NOPPCSYNC;

    if (drawMode & RESMDL_DRAWMODE_FORCE_LIGHTOFF) {
        ctrl |= DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF;
    }

    for (; (c = *pByteCode) != ResByteCodeData::END; pByteCode += sizeof(ResByteCodeData::DrawParams)) {
        node = mdl.GetResNode(pDrawCmd->nodeIdHi << 8 | pDrawCmd->nodeIdLo);
        if (!node.IsVisible()) {
            continue;
        }

        mat = mdl.GetResMat(pDrawCmd->matIdHi << 8 | pDrawCmd->matIdLo);
        shp = mdl.GetResShp(pDrawCmd->shpIdHi << 8 | pDrawCmd->shpIdLo);

        Draw1Mat1ShpDirectly(
            ignoreMat || mat == prevMat ? ResMat(NULL) : mat, shp, NULL, NULL, ctrl, NULL,
            detail::GetIndMtxOp(mat, node, shp)
        );

        prevMat = mat;
    }

#undef pDrawCmd
}

void DrawResMdlLoop(const ResMdl mdl, const u8 *pByteCode, DrawResMdlReplacement *pReplacement, u32 drawMode) {
#define pDrawCmd reinterpret_cast<const ResByteCodeData::DrawParams *>(pByteCode)

    u8 c;

    ResMat mat;
    ResMat prevMat;

    ResShp shp;
    ResNode node;

    Draw1Mat1ShpSwap swap;

    bool ignoreMat = (drawMode & RESMDL_DRAWMODE_IGNORE_MATERIAL);
    u32 ctrl = DRAW1MAT1SHP_CTRL_NOPPCSYNC;

    if (drawMode & RESMDL_DRAWMODE_FORCE_LIGHTOFF) {
        ctrl |= DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF;
    }

    for (; (c = *pByteCode) != ResByteCodeData::END; pByteCode += sizeof(ResByteCodeData::DrawParams)) {
        node = mdl.GetResNode(pDrawCmd->nodeIdHi << 8 | pDrawCmd->nodeIdLo);

        bool visible;
        // @bug Replacement pointer not validated
        if (pReplacement->visArray != NULL) {
            visible = pReplacement->visArray[node.GetID()] != 0;
        } else {
            visible = node.IsVisible();
        }

        if (!visible) {
            continue;
        }

        mat = mdl.GetResMat(pDrawCmd->matIdHi << 8 | pDrawCmd->matIdLo);
        shp = mdl.GetResShp(pDrawCmd->shpIdHi << 8 | pDrawCmd->shpIdLo);

        SetupDraw1Mat1ShpSwap(&swap, pReplacement, mat.GetID());

        Draw1Mat1ShpDirectly(
            ignoreMat || mat == prevMat ? ResMat(NULL) : mat, shp, NULL, NULL, ctrl, &swap,
            detail::GetIndMtxOp(mat, node, shp)
        );

        prevMat = mat;
    }

#undef pDrawCmd
}

void DrawResMdlLoop(const ResMdl mdl, const detail::workmem::MdlZ *pMdlZArray, u32 numMdlZ, u32 drawMode) {
    u32 i;

    ResMat mat;
    ResMat prevMat;

    ResShp shp;
    ResNode node;

    bool ignoreMat = (drawMode & RESMDL_DRAWMODE_IGNORE_MATERIAL);
    u32 ctrl = DRAW1MAT1SHP_CTRL_NOPPCSYNC;

    if (drawMode & RESMDL_DRAWMODE_FORCE_LIGHTOFF) {
        ctrl |= DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF;
    }

    for (i = 0; i < numMdlZ; i++) {
        const detail::workmem::MdlZ &rMdlZ = pMdlZArray[i];

        mat = mdl.GetResMat(rMdlZ.matID);
        shp = mdl.GetResShp(rMdlZ.shpID);
        node = mdl.GetResNode(rMdlZ.nodeID);

        Draw1Mat1ShpDirectly(
            ignoreMat || mat == prevMat ? ResMat(NULL) : mat, shp, NULL, NULL, ctrl, NULL,
            detail::GetIndMtxOp(mat, node, shp)
        );

        prevMat = mat;
    }
}

void DrawResMdlLoop(
    const ResMdl mdl, const detail::workmem::MdlZ *pMdlZArray, u32 numMdlZ, DrawResMdlReplacement *pReplacement,
    u32 drawMode
) {
    u32 i;

    ResMat mat;
    ResMat prevMat;

    ResShp shp;
    ResNode node;

    Draw1Mat1ShpSwap swap;

    bool ignoreMat = (drawMode & RESMDL_DRAWMODE_IGNORE_MATERIAL);
    u32 ctrl = DRAW1MAT1SHP_CTRL_NOPPCSYNC;

    if (drawMode & RESMDL_DRAWMODE_FORCE_LIGHTOFF) {
        ctrl |= DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF;
    }

    for (i = 0; i < numMdlZ; i++) {
        const detail::workmem::MdlZ &rMdlZ = pMdlZArray[i];

        mat = mdl.GetResMat(rMdlZ.matID);
        shp = mdl.GetResShp(rMdlZ.shpID);
        node = mdl.GetResNode(rMdlZ.nodeID);

        SetupDraw1Mat1ShpSwap(&swap, pReplacement, mat.GetID());

        Draw1Mat1ShpDirectly(
            ignoreMat || mat == prevMat ? ResMat(NULL) : mat, shp, NULL, NULL, ctrl, &swap,
            detail::GetIndMtxOp(mat, node, shp)
        );

        prevMat = mat;
    }
}

detail::workmem::MdlZ *SetUpMdlZ(
    u32 *pNumMdlZ, const ResMdl mdl, const math::MTX34 *pViewPosMtxArray, const u8 *pByteCode,
    DrawResMdlReplacement *pReplacement
) {
#define pDrawCmd reinterpret_cast<const ResByteCodeData::DrawParams *>(pByteCode)

    u16 nodeID;
    u8 c;
    u32 mdlZNum = 0;
    ResNode node;
    u32 mtxID;

    u32 viewMtxNum = mdl.GetResMdlInfo().GetNumViewMtx();
    detail::workmem::MdlZ *pMdlZArray = detail::workmem::GetMdlZTemporary();

    for (; (c = *pByteCode) != ResByteCodeData::END; pByteCode += sizeof(ResByteCodeData::DrawParams)) {
        nodeID = pDrawCmd->nodeIdHi << 8 | pDrawCmd->nodeIdLo;
        node = mdl.GetResNode(nodeID);

        bool visible;
        if (pReplacement == NULL || pReplacement->visArray == NULL) {
            visible = node.IsVisible();
        } else {
            visible = pReplacement->visArray[node.GetID()] != 0;
        }

        if (!visible) {
            continue;
        }

        detail::workmem::MdlZ &rMdlZ = pMdlZArray[mdlZNum];

        rMdlZ.nodeID = nodeID;
        rMdlZ.matID = pDrawCmd->matIdHi << 8 | pDrawCmd->matIdLo;
        rMdlZ.shpID = pDrawCmd->shpIdHi << 8 | pDrawCmd->shpIdLo;

        // @bug Matrix ID not validated
        mtxID = node.GetMtxID();
        rMdlZ.Z = pViewPosMtxArray[mtxID]._23;

        rMdlZ.priority = pDrawCmd->priority;

        mdlZNum++;
    }

    *pNumMdlZ = mdlZNum;
    return pMdlZArray;

#undef pDrawCmd
}

} // namespace

void DrawResMdlDirectly(
    const ResMdl mdl, const math::MTX34 *pViewPosMtxArray, const math::MTX33 *pViewNrmMtxArray,
    const math::MTX34 *pViewEnvMtxArray, const u8 *pByteCodeOpa, const u8 *pByteCodeXlu,
    DrawResMdlReplacement *pReplacement, u32 drawMode
) {
    G3DState::SetViewPosNrmMtxArray(pViewPosMtxArray, pViewNrmMtxArray, pViewEnvMtxArray);

    if (!(drawMode & RESMDL_DRAWMODE_NOPPCSYNC)) {
        ut::LC::QueueWait(0);
        PPCSync();
    }

    if (pByteCodeOpa != NULL) {
        if (drawMode & RESMDL_DRAWMODE_SORT_OPA_Z) {
            u32 numMdlZ;
            detail::workmem::MdlZ *pMdlZArray = SetUpMdlZ(&numMdlZ, mdl, pViewPosMtxArray, pByteCodeOpa, pReplacement);

            std::sort(pMdlZArray, pMdlZArray + numMdlZ, FrontToBack);

            if (pReplacement != NULL) {
                DrawResMdlLoop(mdl, pMdlZArray, numMdlZ, pReplacement, drawMode);
            } else {
                DrawResMdlLoop(mdl, pMdlZArray, numMdlZ, drawMode);
            }
        } else if (pReplacement != NULL) {
            DrawResMdlLoop(mdl, pByteCodeOpa, pReplacement, drawMode);
        } else {
            DrawResMdlLoop(mdl, pByteCodeOpa, drawMode);
        }
    }

    if (pByteCodeXlu != NULL) {
        if (drawMode & RESMDL_DRAWMODE_SORT_XLU_Z) {
            u32 numMdlZ;
            detail::workmem::MdlZ *pMdlZArray = SetUpMdlZ(&numMdlZ, mdl, pViewPosMtxArray, pByteCodeXlu, pReplacement);

            std::sort(pMdlZArray, pMdlZArray + numMdlZ, BackToFront);

            if (pReplacement != NULL) {
                DrawResMdlLoop(mdl, pMdlZArray, numMdlZ, pReplacement, drawMode);
            } else {
                DrawResMdlLoop(mdl, pMdlZArray, numMdlZ, drawMode);
            }
        } else if (pReplacement != NULL) {
            DrawResMdlLoop(mdl, pByteCodeXlu, pReplacement, drawMode);
        } else {
            DrawResMdlLoop(mdl, pByteCodeXlu, drawMode);
        }
    }

    G3DState::SetViewPosNrmMtxArray(NULL, NULL, NULL);
}

} // namespace g3d
} // namespace nw4r
