#include <d/tg/d_t_sound_area_mgr.h>
#include <d/a/d_a_player.h>

SPECIAL_ACTOR_PROFILE(SOUND_AREA_MGR, dTgSndMg_c, fProfile::SOUND_AREA_MGR, 0x0145, 0, 0);

int dTgSndMg_c::create() {
    s8 tmpRoomId = roomid;
    addActorToRoom(-1);
    roomid = tmpRoomId;
    return SUCCEEDED;
}

int dTgSndMg_c::doDelete() {
    return SUCCEEDED;
}

int dTgSndMg_c::actorExecute() {
    if (dAcPy_c::LINK != nullptr) {
        dAcPy_c::LINK->field_0xe8 = 0;
    }
    mBgmFlags = 0;
    return SUCCEEDED;
}

int dTgSndMg_c::draw() {
    return SUCCEEDED;
}
