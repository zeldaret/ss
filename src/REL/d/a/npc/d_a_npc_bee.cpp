#include "d/a/npc/d_a_npc_bee.h"
#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_rumble.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "s/s_Math.h"

SPECIAL_ACTOR_PROFILE(NPC_BEE, dAcNpcBee_c, fProfile::NPC_BEE, 0x123, 0, 0);

bool dAcNpcBee_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("Bee"));
    static const char* sModelNames[4] = {
        "fly_A",
        "fly_B",
        "walk_A",
        "walk_B",
    };
    mSwarmBeeCount = 0x14;
    for (int i = 0; i < (int)ARRAY_LENGTH(sModelNames); i++) {
        // instruction reordering
        TRY_CREATE(mBeeModels[i].create(mSwarmBeeCount, mRes.GetResMdl(sModelNames[i]),  &mAllocator, true, nullptr));
    }
    return SUCCEEDED;
}

static dCcD_SrcSph sph;

int dAcNpcBee_c::create() {
    mSceneflag = getFromParams(0x10, 0xFF);
    // mRoom regalloc
    if (mSceneflag != 0xFF && SceneflagManager::sInstance && SceneflagManager::sInstance->checkFlag(mRoomID, mSceneflag)) {
        return FAILED;
    }
    CREATE_ALLOCATOR(dAcNpcBee_c);
    dAcNpcBeeSingleBee* bee = &mBees[0];
    for (int i = 0; i < mSwarmBeeCount; i++, bee++) {
        bee->mIndex = i;
        bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_CRAWL;
        bee->mPos = mPosition;
        bee->mRotY = cM::rndF(65536);
        mColliderList.addCc(bee->mCollider, sph);
        bee->mCollider.mTg.OffSPrm(0x100);
        bee->mCollider.mAt.OnSPrm(0x100);
        if (i < mSwarmBeeCount / 2) {
            bee->field_0x008 = 1;
        }
    }
    // probably inline here
    mStts.SetRank(3);
    mInteractionFlags |= 0x80;
    mColliderList.SetStts(mStts);
    field_0x037C = cM::rndF(65536);
    dAcObjBase_c* honeycomb = dAcObjBase_c::create(fProfile::NPC_HONEYCOMB, -1, mParams, &mPosition, nullptr, nullptr, -1);
    if (honeycomb == nullptr) {
        return FAILED;
    } else {
        mHiveRef.link(honeycomb);
        mAttackRef.link(dAcPy_c::GetLinkM());
    }
    return SUCCEEDED;
}

int dAcNpcBee_c::execute() {
    field_0x037C++;
    updateSwarmBeeColliders();
    actuallyUpdateSwarmBees();
    if (field_0x0379) {
        field_0x0379--;
        if (field_0x0379 == 7) {
            dRumble_c::start(dRumble_c::sRumblePreset1, 1);
        } else if (field_0x0379 == 5) {
            dRumble_c::stop(-1);
        }
    }
    return SUCCEEDED;
}

void dAcNpcBee_c::fn_14_1F40(dAcNpcBeeSingleBee* bee) {
    bee->field_0x003 = 1;
    switch (bee->field_0x005) {
        case 0: {
            bee->field_0x054 = mStartingPos + (bee->mPos - mStartingPos) * (cM::rndF(0.5f) + 1.5f);
            bee->field_0x002 = 0x14;
            bee->field_0x005 = 1;
            // fallthrough
        }
        case 1: {
            cLib::addCalcPos2(&bee->mPos, bee->field_0x054, 0.2f, 15.0f);
            sLib::addCalcAngle(bee->field_0x060.y.ref(), cLib::targetAngleY(bee->mPos, mAttackActor->mPositionCopy2), 2, 0x800);
            sLib::addCalcAngle(bee->field_0x060.x.ref(), 0, 2, 0x800);
            sLib::addCalcAngle(bee->field_0x060.z.ref(), 0, 2, 0x800);
            if (bee->field_0x002 == 0) {
                bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_CHASE;
                bee->field_0x005 = 0;
                bee->field_0x00E = cM::rndF(155.f) + 300.f; 
            }
        }
    }   
}

u32 fn_14_2690(dAcNpcBeeSingleBee* bee) {
    dBgS_ObjGndChk gndChk;
    dBgS_ObjLinChk linChk;
    u32 ret = 0;
    mVec3_c local_148_2 = bee->mPos - bee->field_0x03C;
    mVec3_c local_148 = local_148_2;
    local_148.y = 0.f;
    local_148.normalize();
    mVec3_c scaled = local_148 * 50.f;
    mVec3_c another = bee->mPos + scaled;
    linChk.Set(&bee->mPos, &another, nullptr);
    if (dBgS::GetInstance()->LineCross(&linChk)) {
        ret = 2;
        bee->mPos.x = linChk.GetLinEnd().x - scaled.x;
        bee->mPos.z = linChk.GetLinEnd().z - scaled.z;
    }
    mVec3_c local_149 = bee->mPos;
    local_149.z += 100.f;
    gndChk.SetPos(&local_149);
    if (bee->mPos.y <= dBgS::GetInstance()->GroundCross(&gndChk) + 15.f) {
        // imagine using temp variables...
        bee->mPos.y = dBgS::GetInstance()->GroundCross(&gndChk) + 15.f;
        ret |= 1;
    }
    return ret;
}
