#include "d/t/d_t_tumble_weed.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/obj/d_a_obj_tumble_weed.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/d_sc_game.h"
#include "f/f_profile_name.h"
#include "m/m_vec.h"
#include "s/s_Math.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(TUMBLE_WEED_TAG, dTgTumbleWeed_c, fProfile::TUMBLE_WEED_TAG, 0x0244, 0, 0);

STATE_DEFINE(dTgTumbleWeed_c, AreaOut);
STATE_DEFINE(dTgTumbleWeed_c, AreaIn);
STATE_DEFINE(dTgTumbleWeed_c, Wind);

void rodata_floats() {
    const f32 arr[] = {0.f, 0.8f, 20.f, 20.f, 5.f, 0.5f, 7.f};
}

int dTgTumbleWeed_c::create() {
    mSpawnTimer = 0;
    mWindTimer = 150;
    mStateMgr.changeState(StateID_AreaOut);
    return SUCCEEDED;
}

int dTgTumbleWeed_c::doDelete() {
    return SUCCEEDED;
}

int dTgTumbleWeed_c::actorExecute() {
    mStateMgr.executeState();
    sLib::calcTimer(&mSpawnTimer);
    return SUCCEEDED;
}

int dTgTumbleWeed_c::draw() {
    return SUCCEEDED;
}

void dTgTumbleWeed_c::initializeState_AreaOut() {}
void dTgTumbleWeed_c::executeState_AreaOut() {
    if (sLib::calcTimer(&mWindTimer) == 0) {
        mWindTimer = 150;
        if (shouldDoWind()) {
            mStateMgr.changeState(StateID_Wind);
            return;
        }
    }
    if (isWithinPlayerRadius(mScale.x)) {
        mStateMgr.changeState(StateID_AreaIn);
    }
}
void dTgTumbleWeed_c::finalizeState_AreaOut() {}

void dTgTumbleWeed_c::initializeState_AreaIn() {}
void dTgTumbleWeed_c::executeState_AreaIn() {
    if (mSpawnTimer == 0) {
        if (shouldSpawnTumbleweed()) {
            doSpawnTumbleweed();
        }
        mSpawnTimer = 600;
    }
    if (sLib::calcTimer(&mWindTimer) == 0) {
        mWindTimer = 150;
        if (shouldDoWind()) {
            mStateMgr.changeState(StateID_Wind);
            return;
        }
    }
    if (!isWithinPlayerRadius(mScale.x)) {
        mStateMgr.changeState(StateID_AreaOut);
    }
}
void dTgTumbleWeed_c::finalizeState_AreaIn() {}

void dTgTumbleWeed_c::initializeState_Wind() {
    mVec3_c vec;
    getWind(vec);
    if (mSpawnedTumbleWeed.isLinked()) {
        mSpawnedTumbleWeed.get()->setWind(vec);
    }
}

void dTgTumbleWeed_c::executeState_Wind() {
    if (isWithinPlayerRadius(mScale.x)) {
        mStateMgr.changeState(StateID_AreaIn);
    } else {
        mStateMgr.changeState(StateID_AreaOut);
    }
}
void dTgTumbleWeed_c::finalizeState_Wind() {}

bool dTgTumbleWeed_c::shouldSpawnTumbleweed() {
    return !mSpawnedTumbleWeed.isLinked() && cM::rnd() <= 0.8f && !EventManager::isInEvent();
}

bool dTgTumbleWeed_c::shouldDoWind() {
    return mSpawnedTumbleWeed.isLinked() && cM::rnd() <= 0.5f;
}

void dTgTumbleWeed_c::doSpawnTumbleweed() {
    mVec3_c pos = mVec3_c::Ez;

    dCamera_c *pCam = dScGame_c::getCamera(0);
    if (pCam != nullptr) {
        mAng angle = pCam->getYRot();

        f32 change;
        if (cM::rnd() < 0.5f) {
            change = cM::rndFX(5.f) * 182.f + 10923.f;
        } else {
            change = cM::rndFX(5.f) * 182.f + -10923.f;
        }
        angle += change;
        pos.rotY(angle);

        pos *= 600.f;
        pos += pCam->getPositionMaybe();

        if (dBgS_ObjGndChk::CheckPos(pos + mVec3_c::Ey * 100.f)) {
            pos.y = dBgS_ObjGndChk::GetGroundHeight() + 10.f;
        }
    }

    dAcOTumbleWeed_c *pTumble = static_cast<dAcOTumbleWeed_c *>(
        dAcObjBase_c::create(fProfile::OBJ_TUMBLE_WEED, mRoomID, 0, &pos, nullptr, nullptr, 0xFFFFFFFF)
    );
    if (pTumble == nullptr) {
        return;
    }

    mSpawnedTumbleWeed.link(pTumble);
    if (pCam != nullptr) {
        mAng rot = mAng::fromVec(mPosition - pos);

        f32 change;
        if (cM::rnd() < 0.5f) {
            change = cM::rndFX(5.f) * 182.f;
        } else {
            change = cM::rndFX(5.f) * -182.f;
        }
        rot += change;

        pTumble->mAngle.y = rot;
        pTumble->mSpeed = cM::rndFX(1.f) + 20.f;
        pTumble->mVelocity.y = 0.f;
    }
}

void dTgTumbleWeed_c::getWind(mVec3_c &pWind) {
    mVec3_c wind = mVec3_c::Ez;

    // @bug dir may not be initialized
    mAng dir;

    if (mSpawnedTumbleWeed.isLinked()) {
        dir = mSpawnedTumbleWeed.get()->getAngle().y;
    }

    f32 change;
    if (cM::rnd() < 0.5f) {
        change = cM::rndFX(15.f) * 182.f + 8192.f;
    } else {
        change = cM::rndFX(15.f) * 182.f + -8192.f;
    }

    dir += change;
    wind.rotY(dir);
    wind *= cM::rndFX(3.f) + 7.f;

    pWind = wind;
}
