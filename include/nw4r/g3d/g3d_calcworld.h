#ifndef NW4R_G3D_CALC_WORLD_H
#define NW4R_G3D_CALC_WORLD_H
#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

#include "nw4r/math.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {

// Forward declarations
class AnmObjChr;
struct ChrAnmResult;
class FuncObjCalcWorld;
class WorldMtxManip;

/******************************************************************************
 *
 * Functions
 *
 ******************************************************************************/
void CalcWorld(
    math::MTX34 *pModelMtxArray, u32 *pModelMtxAttribArray, const u8 *pByteCode, const math::MTX34 *pBaseMtx,
    ResMdl mdl, AnmObjChr *pAnmChr, FuncObjCalcWorld *pFuncObj, u32 rootAttrib
);

void CalcWorld(
    math::MTX34 *pModelMtxArray, u32 *pModelMtxAttribArray, const u8 *pByteCode, const math::MTX34 *pBaseMtx,
    ResMdl mdl, AnmObjChr *pAnmChr, FuncObjCalcWorld *pFuncObj
);

void CalcSkinning(math::MTX34 *pModelMtxArray, u32 *pModelMtxAttribArray, const ResMdl mdl, const u8 *pByteCode);

/******************************************************************************
 *
 * ICalcWorldCallback
 *
 ******************************************************************************/
class ICalcWorldCallback {
public:
    virtual ~ICalcWorldCallback() {}; // at 0x8

    virtual void ExecCallbackA(ChrAnmResult *pResult, ResMdl mdl,
                               FuncObjCalcWorld *pFuncObj) = 0; // at 0xC

    virtual void ExecCallbackB(WorldMtxManip *pManip, ResMdl mdl,
                               FuncObjCalcWorld *pFuncObj) = 0; // at 0x10

    virtual void ExecCallbackC(math::MTX34 *pMtxArray, ResMdl mdl,
                               FuncObjCalcWorld *pFuncObj) = 0; // at 0x14
};

/******************************************************************************
 *
 * WorldMtxManip
 *
 ******************************************************************************/
class WorldMtxManip {
public:
    WorldMtxManip(math::MTX34 *pM, math::VEC3 *pS, u32 *pWMAttr) : mpM(pM), mpS(pS), mpWMAttr(pWMAttr) {}

    void SetScale(f32, f32, f32);
    void GetMtx(nw4r::math::MTX34 *out) {
        if (out != nullptr) {
            PSMTXCopy(*mpM, *out);
        }
    }

    void SetMtx(const nw4r::math::MTX34 *arg) {
        if (arg != nullptr) {
            PSMTXCopy(*arg, *mpM);
        } else {
            PSMTXIdentity(*mpM);
        }
    }

private:
    math::MTX34 *mpM; // at 0x0
    math::VEC3 *mpS;  // at 0x4
    u32 *mpWMAttr;    // at 0x8
};

/******************************************************************************
 *
 * FuncObjCalcWorld
 *
 ******************************************************************************/
class FuncObjCalcWorld {
public:
    FuncObjCalcWorld(ICalcWorldCallback *pCallback, u32 timing, u32 nodeID)
        : mpCallback(pCallback), mTiming(timing), mNodeID(nodeID) {}

    ~FuncObjCalcWorld() {}

    u16 GetCallbackNodeID() const {
        return mNodeID;
    }

    void SetCallbackNodeID(u16 id) {
        mNodeID = id;
    }

    void CheckCallbackA(u32 nodeID, ChrAnmResult *pResult, ResMdl mdl) {
        if (nodeID == mNodeID && (mTiming & ScnObj::CALLBACK_TIMING_A)) {
            mpCallback->ExecCallbackA(pResult, mdl, this);
        }
    }

    void CheckCallbackB(u32 nodeID, math::MTX34 *pM, math::VEC3 *pS, u32 *pWMAttr, ResMdl mdl) {
        if (nodeID == mNodeID && (mTiming & ScnObj::CALLBACK_TIMING_B)) {
            WorldMtxManip manip(pM, pS, pWMAttr);
            mpCallback->ExecCallbackB(&manip, mdl, this);
        }
    }

    void CheckCallbackC(math::MTX34 *pMtxArray, ResMdl mdl) {
        if (mTiming & ScnObj::CALLBACK_TIMING_C) {
            mpCallback->ExecCallbackC(pMtxArray, mdl, this);
        }
    }

private:
    ICalcWorldCallback *mpCallback; // at 0x0
    u8 mTiming;                     // at 0x4
    u8 PADDING_0x5;                 // at 0x5
    u16 mNodeID;                    // at 0x6
};

} // namespace g3d
} // namespace nw4r

#endif
