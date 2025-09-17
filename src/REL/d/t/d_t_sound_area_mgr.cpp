#include "d/t/d_t_sound_area_mgr.h"

#include "d/a/d_a_player.h"

SPECIAL_ACTOR_PROFILE(SOUND_AREA_MGR, dTgSndMg_c, fProfile::SOUND_AREA_MGR, 0x0145, 0, 0);

int dTgSndMg_c::create() {
    s8 tmpRoomId = mRoomID;
    addActorToRoom(-1);
    mRoomID = tmpRoomId;
    return SUCCEEDED;
}

int dTgSndMg_c::doDelete() {
    return SUCCEEDED;
}

int dTgSndMg_c::actorExecute() {
    if (dAcPy_c::LINK != nullptr) {
        dAcPy_c::LINK->mTgSndAreaFlags = 0;
    }
    mSndFlags = 0;
    return SUCCEEDED;
}

int dTgSndMg_c::draw() {
    return SUCCEEDED;
}
