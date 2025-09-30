#include "d/a/obj/d_a_obj_warp_hole.h"

#include "c/c_lib.h"
#include "d/a/d_a_player.h"
#include "d/a/npc/d_a_npc_talk_kensei.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_sc_game.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_WARP_HOLE, dAcOwarpHole_c, fProfile::OBJ_WARP_HOLE, 0x25C, 0, 0);

dCcD_SrcCyl dAcOwarpHole_c::sCylSrc = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {~(AT_TYPE_BUGNET | AT_TYPE_0x80000 | AT_TYPE_BEETLE | AT_TYPE_WIND | AT_TYPE_0x8000), 0x111, {0, 0, 0x407}, 0, 0},
     /* mObjCo */ {0x29}},
    /* mCylInf */
    {320.f, 300.f}
};

// Unused(?) 600.0f float at start of rodata
const float dAcOwarpHole_c::dummy600 = 600.0f;

bool dAcOwarpHole_c::createHeap() {
    return true;
}

int dAcOwarpHole_c::create() {
    mExitListIdx = mParams;
    CREATE_ALLOCATOR(dAcOwarpHole_c);
    mStts.SetRank(0xD);
    mCcCyl.Set(sCylSrc);
    mCcCyl.SetStts(mStts);
    dCcS::GetInstance()->Set(&mCcCyl);
    mCcCyl.OnCoSet();
    mCcCyl.ClrTgSet();
    mLinkPos = dAcPy_c::GetLink()->mPosition;
    mEff.init(this);
    mWalkFramesMaybe = 0;
    mPositionCopy2 = mPosition;
    mPositionCopy2.y += 170.0f;
    mPositionCopy3 = mPositionCopy2;

    return SUCCEEDED;
}

int dAcOwarpHole_c::doDelete() {
    return SUCCEEDED;
}

int dAcOwarpHole_c::actorExecute() {
    if (!dAcPy_c::GetLink()->checkObjectProperty(0x200) && getDistanceTo(dAcPy_c::GetLinkM()->mPosition) < 600.0f) {
        Event ev = Event("BeforeLastBossBattleTalk", 100, 0x100001, nullptr, nullptr);
        EventManager::alsoSetAsCurrentEvent(dAcNpcTalkKensei_c::GetInstance(), &ev, nullptr);
    }

    dCcS::GetInstance()->Set(&mCcCyl);
    updateMatrix();
    mEff.createContinuousEffect(PARTICLE_RESOURCE_ID_MAPPING_914_, mWorldMtx, nullptr, nullptr);
    holdSound(0xC90);

    return SUCCEEDED;
}

int dAcOwarpHole_c::actorExecuteInEvent() {
    mEff.createContinuousEffect(PARTICLE_RESOURCE_ID_MAPPING_914_, mWorldMtx, nullptr, nullptr);
    holdSound(0xC90);
    bool advance = mEvent.isAdvance();
    mPositionCopy2 = mPosition;
    mPositionCopy2.y += 170.0f;
    s16 targetAngleY;

    switch (mEvent.getCurrentEventCommand()) {
        case 'none': mEvent.advanceNext(); break;
        case 'plwk':
            if (advance) {
                mLinkPos = dAcPy_c::GetLink()->mPosition;
                targetAngleY = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
                mLinkPos.x += 330.0f * nw4r::math::SinIdx(targetAngleY);
                mLinkPos.z += 330.0f * nw4r::math::CosIdx(targetAngleY);
            }
            if (EventManager::isInEvent()) {
                if (EventManager::isCurrentEvent("BeforeLastBossBattleChicken")) {
                    dAcPy_c::GetLinkM()->vt_0x2AC();
                    dAcPy_c::GetLinkM()->triggerMoveEventMaybe(); // TODO: parameters
                }
            }
            if (dAcPy_c::GetLinkM()->mPosition.absXZTo(mLinkPos) < 10.0f) {
                mEvent.advanceNext();
            }
            break;
        case 'warp':
            if (advance) {
                dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_916_, mWorldMtx, nullptr, nullptr, 0, 0);
                mWalkFramesMaybe = 80;
            }
            if (mWalkFramesMaybe) {
                mWalkFramesMaybe -= 1;
            }
            if (mWalkFramesMaybe == 77) {
                dAcPy_c::GetLinkM()->setObjectProperty(OBJ_PROP_0x200);
            }
            if (mWalkFramesMaybe == 20) {
                dScGame_c::GetInstance()->triggerExit(mRoomID, mExitListIdx);
            }
            if (!mWalkFramesMaybe) {
                mEvent.advanceNext();
            }
    }

    return SUCCEEDED;
}

int dAcOwarpHole_c::draw() {
    return SUCCEEDED;
}
