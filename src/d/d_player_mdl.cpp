#include "d/a/d_a_player.h"
#include "m/m_vec.h"

// TODO: This file contains the vtable and a bunch of weak functions
// for daPlayerModelBase_c. How do we generate the vtable here though?
// All virtual methods of daPlayerModelBase_c seem to be inline, so
// there's not much opportunity to get the vtable to be emitted here
// except for dtor hacks...

void daPlBaseCalcWorldCallback_c::ExecCallbackC(
        nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl,
        nw4r::g3d::FuncObjCalcWorld *pFuncObj
    ) {

    }

void daPlayerModelBase_c::fn_8005EAC0() {
    // Just getting weak functions to appear here
    someDivingHrabbingStuff();
    vt_0x308();
}

void daPlayerModelBase_c::fn_8005ED60() {
    // Just getting weak functions to appear here
    vt_0x2E0();
    vt_0x308();
    mVec3_c v;
    vt_0x30C(v);
    isMPPose();
    isOnTightRope();
    isOnVines();
}

void daPlayerModelBase_c::fn_8005F890() {
    // Just getting weak functions to appear here
    vt_0x2E8();
    vt_0x304();
    mAng a1, a2;
    isOnClawTargetMaybe(0, a1, a2);
}

void daPlayerModelBase_c::fn_80061410() {
    // Just getting weak functions to appear here
    alwaysRet0();
}

void dummy_triggerDtor(daPlayerModelBase_c *pl) {
    delete pl;
}
