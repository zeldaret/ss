#include "d/t/d_t_gekotag.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/e/d_a_e_geko.h"
#include "d/a/obj/d_a_obj_base.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "s/s_Math.h"

SPECIAL_ACTOR_PROFILE(GEKO_TAG, dTgGekoTag, fProfile::GEKO_TAG, 0xEE, 0, 0);

STATE_DEFINE(dTgGekoTag, Wait);

int dTgGekoTag::create() {
    mNumGeko = getNumGeko() != 0xF ? getNumGeko() : 1;

    for (int i = 0; i < mNumGeko; i++) {
        mRefs[i].unlink();
    }

    mNoSpawnRadius = getNoSpawnRadius();

    f32 rad = mNoSpawnRadius;
    if (!rad) {
        mNoSpawnRadius = 100.0f;
    }

    mGekoParam = getGekoParam() != 3 ? getGekoParam() : 0;
    field_0x1FD = getParm3();

    mStateMgr.changeState(StateID_Wait);

    return SUCCEEDED;
}

int dTgGekoTag::doDelete() {
    return SUCCEEDED;
}

int dTgGekoTag::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dTgGekoTag::draw() {
    return SUCCEEDED;
}

void dTgGekoTag::initializeState_Wait() {}
void dTgGekoTag::executeState_Wait() {
    doExecute();
}
void dTgGekoTag::finalizeState_Wait() {}

void dTgGekoTag::doExecute() {
    sLib::calcTimer(&mTimer2);
    if (sLib::calcTimer(&mTimer1) != 0) {
        return;
    }

    if (dAcPy_c::LINK->getSquareDistanceTo(position) < mNoSpawnRadius * mNoSpawnRadius) {
        return;
    }
    field_0x1FD = 0xFF;

    u32 gekoParmTmp = (params & 0xFF) | 0x3FC00000;
    gekoParmTmp = (gekoParmTmp & 0xFFFF00FF) | (params & 0xFF00);
    u32 gekoParm = gekoParmTmp | ((mGekoParam != 0 ? 3 : 1) << 16);

    mAng3_c gekoRot;
    gekoRot.clear();
    gekoRot.y = cM::rndF(65535.0f);
    int numFullGekoRefs = 0;
    for (int i = 0; i < mNumGeko; i++) {
        if (mRefs[i].get() == nullptr) {
            if (mTimer2 == 0) {
                dAcObjBase_c *b =
                    dAcObjBase_c::create(fProfile::E_GEKO, roomid, gekoParm, &position, &gekoRot, nullptr, 0xFFFFFFFF);

                if (b != nullptr) {
                    mRefs[i].link(static_cast<dAcEgeko_c *>(b));
                    mTimer1 = 20;
                    return;
                }
            }
        } else {
            numFullGekoRefs++;
        }
    }

    if (numFullGekoRefs == mNumGeko) {
        mTimer2 = 0x96;
    }
}
