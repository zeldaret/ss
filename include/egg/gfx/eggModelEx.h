#ifndef EGG_MODEL_EX_H
#define EGG_MODEL_EX_H

#include "common.h"
#include "egg/egg_types.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
namespace EGG {

class ModelEx {
public:
    enum EType {
        cType_ScnMdlSimple,
        cType_ScnMdl,
        cType_ScnMdl1Mat1Shp,
        cType_ScnProcModel,
        cType_ScnRfl,
        cType_None
    };

    enum EFlag {
        cFlag_HasOriginalBV = (1 << 0)
    };

    enum EDrawShape {
        cDrawShape_None = (1 << 0)
    };

private:
    nw4r::g3d::ScnObj *mScnObj;        // at 0x0
    EType mType;                       // at 0x4
    u16 mFlag;                         // at 0x8
    ModelBoundingInfo *mpBoundingInfo; // at 0xC

    static u32 sDrawFlag;
    static const char *sByteCodeCalcStr;
    static const char *sByteCodeMixStr;

public:
    ModelEx(nw4r::g3d::ScnObj *);
    void getShapeMinMax(u16, nw4r::math::VEC3 *, nw4r::math::VEC3 *, bool);
    void calcWorld(nw4r::math::MTX34 *, const nw4r::math::MTX34 *worldMtx) const;
    nw4r::g3d::ResMdl getResMdl() const;
    nw4r::g3d::ResShp getResShp(u16) const;

    nw4r::g3d::ScnMdl *getScnMdl() const {
        return (mType == cType_ScnMdl) ? (nw4r::g3d::ScnMdl *)mScnObj : NULL;
    }

    nw4r::g3d::ScnMdlSimple *getScnMdlSimple() const {
        return (mType == cType_ScnMdlSimple || mType == cType_ScnMdl) ? (nw4r::g3d::ScnMdlSimple *)mScnObj : NULL;
    }

    nw4r::g3d::ScnMdl1Mat1Shp *getScnMdl1Mat1Shp() const {
        return (mType == cType_ScnMdl1Mat1Shp) ? (nw4r::g3d::ScnMdl1Mat1Shp *)mScnObj : NULL;
    }

    void getMtxType0(nw4r::math::MTX34 *pMtx) const {
        nw4r::math::MTX34Copy(pMtx, mScnObj->GetMtxPtr(nw4r::g3d::ScnObj::MTX_LOCAL));
    }
};

} // namespace EGG

#endif
