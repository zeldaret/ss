#include "d/t/d_t_barrel_pos.h"

#include "d/a/obj/d_a_obj_stage_sink.h"
#include "d/t/d_t_barrel.h"
#include "f/f_base.h"

SPECIAL_ACTOR_PROFILE(TAG_BARREL_POS, dTgBarrelPos_c, fProfile::TAG_BARREL_POS, 0x222, 0, 0);

int dTgBarrelPos_c::create() {
    mLinkIndex = getFromParams(0, 0xFF);
    if (mLinkIndex == 0xFF) {
        mLinkIndex = 0;
    }

    mLinkId = getFromParams(8, 0xF);

    mStageRef.link(dAcOstageSink_c::GetInstance());

    mSpawnPosition = mPosition;

    return SUCCEEDED;
}

int dTgBarrelPos_c::doDelete() {
    return SUCCEEDED;
}

int dTgBarrelPos_c::actorExecute() {
    dAcOstageSink_c *pStage = mStageRef.get();

    mVec3_c pos = pStage->mPosition;
    mAng3_c rot = pStage->mRotation;

    mVec3_c stageDist = mSpawnPosition - pos;
    mMtx_c m;
    m.transS(pos);
    m.ZXYrotM(rot);
    m.multVecSR(stageDist, mPosition);

    mPosition += pos;

    return SUCCEEDED;
}

int dTgBarrelPos_c::draw() {
    return SUCCEEDED;
}
