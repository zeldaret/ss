#include "d/t/d_t_tumble_weed.h"

#include "m/m_vec.h"
#include "s/s_Math.h"

SPECIAL_ACTOR_PROFILE(TUMBLE_WEED_TAG, dTgTumbleWeed_c, fProfile::TUMBLE_WEED_TAG, 0x0244, 0, 0);

STATE_DEFINE(dTgTumbleWeed_c, AreaOut);
STATE_DEFINE(dTgTumbleWeed_c, AreaIn);
STATE_DEFINE(dTgTumbleWeed_c, Wind);

int dTgTumbleWeed_c::create() {
    tumbleweedTimer = 0;
    windTimer = 0x96;
    mStateMgr.changeState(StateID_AreaOut);
    return SUCCEEDED;
}

int dTgTumbleWeed_c::doDelete() {
    return SUCCEEDED;
}

int dTgTumbleWeed_c::actorExecute() {
    mStateMgr.executeState();
    sLib::calcTimer(&tumbleweedTimer);
    return SUCCEEDED;
}

int dTgTumbleWeed_c::draw() {
    return SUCCEEDED;
}

void dTgTumbleWeed_c::initializeState_AreaOut() {}
void dTgTumbleWeed_c::executeState_AreaOut() {
    if (sLib::calcTimer(&windTimer) == 0) {
        windTimer = 0x96;
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
    if (tumbleweedTimer == 0) {
        if (shouldSpawnTumbleweed()) {
            doSpawnTumbleweed();
        }
        tumbleweedTimer = 600;
    }
    if (sLib::calcTimer(&windTimer) == 0) {
        windTimer = 0x96;
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
    getWind(&vec);
    if (childTumbleweed.get() != nullptr) {
        childTumbleweed.get()->setWind(vec);
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
    bool spawnAllowed = false;
    if (childTumbleweed.get() == nullptr && cM::rnd() <= 0.8f) {
        spawnAllowed = true;
    }

    if (spawnAllowed) {
        return true;
    }

    return false;
}

bool dTgTumbleWeed_c::shouldDoWind() {
    return childTumbleweed.get() != nullptr && cM::rnd() <= 0.5f;
}
void dTgTumbleWeed_c::doSpawnTumbleweed() {}
void dTgTumbleWeed_c::getWind(mVec3_c *) {}
