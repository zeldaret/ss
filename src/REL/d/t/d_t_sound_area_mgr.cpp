#include <d/tg/d_t_sound_area_mgr.h>
#include <d/d_player.h>

SPECIAL_ACTOR_PROFILE(SOUND_AREA_MGR, dTgSndMg_c, fProfile::SOUND_AREA_MGR, 0x0145, 0, 0);

int dTgSndMg_c::create() {
    s8 tmpRoomId = roomid;
    addActorToRoom(-1);
    roomid = tmpRoomId;
    return 1;
}

int dTgSndMg_c::doDelete() {
    return 1;
}

int dTgSndMg_c::actorExecute() {
    if (dPlayer::LINK != nullptr) {
        dPlayer::LINK->field_0xe8 = 0;
    }
    mBgmFlags = 0;
    return 1;
}

int dTgSndMg_c::draw() {
    return 1;
}
