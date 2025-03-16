#include "egg/gfx/eggModelEx.h"

#include "common.h"
#include "nw4r/g3d/g3d_calcworld.h"
#include "nw4r/g3d/g3d_scnmdl.h"
#include "nw4r/g3d/g3d_scnmdl1mat1shp.h"
#include "nw4r/g3d/g3d_scnmdlsmpl.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

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

void ModelEx::getShapeMinMax(u16 shapeIndex, math::VEC3 *pMin, math::VEC3 *pMax, bool doCalcWorld) {}

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

g3d::ResMdl ModelEx::getResMdl(u16) const {
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
