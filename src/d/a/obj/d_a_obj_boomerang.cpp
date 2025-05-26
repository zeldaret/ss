#include "d/a/obj/d_a_obj_boomerang.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_fanm.h"
#include "nw4r/g3d/g3d_anmchr.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_State.hpp"

SPECIAL_ACTOR_PROFILE(BOOMERANG, dAcBoomerang_c, fProfile::BOOMERANG, 0x125, 0, 0x4);

STATE_DEFINE(dAcBoomerang_c, Wait);
STATE_DEFINE(dAcBoomerang_c, Move);
STATE_DEFINE(dAcBoomerang_c, MoveCancelWait);
STATE_DEFINE(dAcBoomerang_c, ReturnWait);
STATE_DEFINE(dAcBoomerang_c, EventReturnWait);

const dAcBoomerang_c::ChrAnimation_t dAcBoomerang_c::sChrAnims[dAcBoomerang_c::RB_MAX] = {
    {    "RB_Set", 0.f},
    {"RB_Default", 3.f},
    {    "RB_Cut", 3.f},
    {   "RB_Hold", 3.f},
    {"RB_Hold_ed", 3.f},
    {   "RB_Back", 3.f},
};

void areaCallback(dAcObjBase_c *param0, mVec3_c *param1, u32 param2) {
    if (param2 != 0) {
        return;
    }
}

void dAcBoomerang_c::atHitCallback(cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB) {
    if (i_actorB != nullptr && GetLinkage().checkFlag(0x80)) {
        if (dAcPy_c::getCurrentBeetleType() > 2) {
            if (i_actorB == mGrabbedActor.get()) {
                return;
            }

            if (!mStateMgr.isState(StateID_Move)) {
                return;
            }

            if (tryGrabObject(i_actorB)) {
                setChrAnimation(RB_HOLD);
                mAnmChr[0].setRate(0.f);
                mAnmChr[0].setFrameOnly(i_actorB->GetLinkage().field_0x24);
                return;
            }
        }
    }

    if (mStateMgr.isState(StateID_MoveCancelWait)) {
        return;
    }

    if (i_objInfB->ChkTgBit23()) {
        setChrAnimation(RB_CUT);
    }
}

bool dAcBoomerang_atHitCallback(
    dAcObjBase_c *i_actorA, cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB
) {
    static_cast<dAcBoomerang_c *>(i_actorA)->atHitCallback(i_objInfA, i_actorB, i_objInfB);
    return true;
}

// ...

bool dAcBoomerang_c::tryGrabObject(dAcObjBase_c *pObj) {
    dAcPy_c *player = dAcPy_c::GetLink2();
    if (checkField_0x8CC(0x1000) && GetLinkage().checkFlag(0x80)) {}
}

// ...

void dAcBoomerang_c::setChrAnimation(dAcBoomerang_c::ChrAnimation_e requestedAnimation) {
    // Do not set anim if there is no change, or the request is to cut from hold
    if (requestedAnimation == mCurrentAnimation || (mCurrentAnimation == RB_HOLD && requestedAnimation == RB_CUT)) {
        return;
    }

    mAnmChr[0].setAnm(mMdl, mResFile.GetResAnmChr(sChrAnims[requestedAnimation].mName), m3d::PLAY_MODE_4);
    nw4r::g3d::AnmObjChr *pAnmObj = static_cast<nw4r::g3d::AnmObjChr *>(mAnmChr[0].getAnimObj());

    pAnmObj->Release();

    nw4r::g3d::ResMdl mdl = mMdl.getResMdl();
    for (int i = 0; i < mWindNodeID; ++i) {
        pAnmObj->Bind(mdl, i, nw4r::g3d::AnmObjChr::BIND_ONE);
    }

    if (requestedAnimation == RB_SET) {
        pAnmObj->Bind(mdl, mLeftWingNodeID, nw4r::g3d::AnmObjChr::BIND_ONE);
        pAnmObj->Bind(mdl, mRightWingNodeID, nw4r::g3d::AnmObjChr::BIND_ONE);
    }

    mCurrentAnimation = requestedAnimation;

    mMdl.setAnm(mAnmChrBlend, sChrAnims[requestedAnimation].mRate);
}
