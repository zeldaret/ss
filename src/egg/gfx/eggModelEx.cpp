#include "egg/gfx/eggModelEx.h"

#include "common.h"
#include "egg/gfx/eggAnalizeDL.h"
#include "egg/gfx/eggG3DUtility.h"
#include "egg/gfx/eggIScnProcModel.h"
#include "nw4r/g3d/g3d_calcworld.h"
#include "nw4r/g3d/g3d_scnmdl.h"
#include "nw4r/g3d/g3d_scnmdl1mat1shp.h"
#include "nw4r/g3d/g3d_scnmdlsmpl.h"
#include "nw4r/g3d/g3d_scnproc.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "nw4r/math/math_types.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/mtxvec.h"

namespace EGG {

using namespace nw4r;

const char *ModelEx::sByteCodeCalcStr = "NodeTree";
const char *ModelEx::sByteCodeMixStr = "NodeMix";

ModelEx::ModelEx(g3d::ScnObj *obj)
    : mType(cType_None), mScnObj(g3d::G3dObj::DynamicCast<g3d::ScnLeaf>(obj)), mFlag(0), mpBoundingInfo(nullptr) {
    if (g3d::G3dObj::DynamicCast<g3d::ScnMdl>(mScnObj) != nullptr) {
        mType = cType_ScnMdl;
    } else if (g3d::G3dObj::DynamicCast<g3d::ScnMdlSimple>(mScnObj) != nullptr) {
        mType = cType_ScnMdlSimple;
    } else if (g3d::G3dObj::DynamicCast<g3d::ScnMdl1Mat1Shp>(mScnObj) != nullptr) {
        mType = cType_ScnMdl1Mat1Shp;
    }
}

static const f32 sFaceMin[] = {-22.61115f, -55.770135f, -45.93067f};
static const f32 sFaceMax[] = {81.17467f, 49.677597f, 45.93065f};

static const f32 sNoneMin[] = {0.0f, 0.0f, 0.0f};
static const f32 sNoneMax[] = {0.0f, 0.0f, 0.0f};

// Not sure what this was for but it didn't get stripped due to data pooling
static const G3DUtility::MdlSearch sMdlSearch[6] = {
    G3DUtility::searchStringResNode, G3DUtility::searchStringResMat, G3DUtility::searchStringResTexPlttInfo,
    G3DUtility::searchStringResNode, G3DUtility::searchStringResMat, G3DUtility::searchStringResTexPlttInfo,
};

void thisProbablyGotDeadStripped() {
    sMdlSearch[0](g3d::ResMdl(nullptr), "", nullptr, 0);
}

extern "C" s32 lbl_8057682C;

void ModelEx::getShapeMinMax(u16 shapeIndex, math::VEC3 *pMin, math::VEC3 *pMax, bool doCalcWorld) {
    switch (mType) {
        case cType_ScnMdlSimple:
        case cType_ScnMdl:
        case cType_ScnMdl1Mat1Shp: {
            g3d::ResShp shp = getResShp(shapeIndex);
            lbl_8057682C = 0;
            g3d::ResMdl mdl = getResMdl();
            math::MTX34 tmpMtx;
            g3d::ResMdlInfo info = mdl.GetResMdlInfo();
            math::MTX34 *pMtxArr = G3DUtility::GetWorldMtxArray(info.GetNumPosNrmMtx() * sizeof(math::MTX34), 1);
            if (doCalcWorld) {
                PSMTXIdentity(tmpMtx);
                calcWorld(pMtxArr, &tmpMtx);
            }

            s32 defaultMatrixIdx = shp.ref().curMtxIdx;
            AnalizeDL analyze(shp);

            bool firstIteration = true;
            AnalizeDL::Status status;
            while ((status = analyze.step()) != AnalizeDL::STATUS_END) {
                if (status != AnalizeDL::STATUS_VTX) {
                    continue;
                }

                const AnalizeDL::VtxResult &result = analyze.getVtxResult();
                s32 mtxIdx = result.mMtxIdx;
                if (mtxIdx == -1) {
                    mtxIdx = defaultMatrixIdx;
                }
                nw4r::math::VEC3 tmpVec;
                PSMTXMultVec(pMtxArr[mtxIdx], &analyze.getVtxResult().field_0x08, tmpVec);
                if (firstIteration) {
                    firstIteration = false;
                    *pMin = *pMax = tmpVec;
                } else {
                    math::VEC3Minimize(pMin, pMin, &tmpVec);
                    math::VEC3Maximize(pMax, pMax, &tmpVec);
                }
            }
            lbl_8057682C = 0;
            break;
        }
        case cType_ScnProcModel:
            static_cast<IScnProcModel *>(static_cast<g3d::ScnProc *>(mScnObj)->GetUserData())
                ->getMinMaxScnProcModel(pMin, pMax);
            break;
        case cType_ScnRfl: {
            const f32 *min = sFaceMin;
            const f32 *max = sFaceMax;
            pMin->x = min[0];
            pMin->y = min[1];
            pMin->z = min[2];
            pMax->x = max[0];
            pMax->y = max[1];
            pMax->z = max[2];
            break;
        }

        case cType_None:
            const f32 *min = sNoneMin;
            const f32 *max = sNoneMax;
            pMin->x = min[0];
            pMin->y = min[1];
            pMin->z = min[2];
            pMax->x = max[0];
            pMax->y = max[1];
            pMax->z = max[2];
            break;
    }
}

void ModelEx::calcWorld(math::MTX34 *pWorldMtxArray, const math::MTX34 *worldMtx) const {
    switch (mType) {
        case cType_ScnMdlSimple:
        case cType_ScnMdl:       {
            g3d::ScnMdlSimple *pMdl = (g3d::ScnMdlSimple *)mScnObj;
            // cast for regalloc
            u32 *wldAttrib = (u32 *)pMdl->GetWldMtxAttribArray();
            g3d::ResMdl mdl = pMdl->GetResMdl();
            const math::MTX34 *worldMtx2 = worldMtx == nullptr ? pMdl->GetMtxPtr(g3d::ScnObj::MTX_WORLD) : worldMtx;

            g3d::CalcWorld(
                pWorldMtxArray, wldAttrib, mdl.GetResByteCode(sByteCodeCalcStr), worldMtx2, mdl, nullptr, nullptr
            );

            if (mdl.GetResByteCode(sByteCodeMixStr) != nullptr) {
                g3d::CalcSkinning(pWorldMtxArray, wldAttrib, mdl, mdl.GetResByteCode(sByteCodeMixStr));
            }
        } break;
        case cType_ScnMdl1Mat1Shp:
        case cType_ScnProcModel:
        case cType_ScnRfl:         getMtxType0(pWorldMtxArray); break;
        case cType_None:
        default:                   break;
    }
}

g3d::ResMdl ModelEx::getResMdl() const {
    g3d::ScnMdlSimple *mdl = getScnMdlSimple();
    if (mdl != nullptr) {
        return mdl->GetResMdl();
    }

    return g3d::ResMdl(nullptr);
}

g3d::ResShp ModelEx::getResShp(u16 shapeIndex) const {
    g3d::ScnMdlSimple *mdl = getScnMdlSimple();
    if (mdl != nullptr) {
        return mdl->GetResMdl().GetResShp(shapeIndex);
    }

    g3d::ScnMdl1Mat1Shp *mdl1 = getScnMdl1Mat1Shp();
    if (mdl1 != nullptr) {
        return mdl1->GetResShp();
    }

    return g3d::ResShp(nullptr);
}

} // namespace EGG
