#include <d/tg/d_t_rock_boat.h>
#include <d/a/obj/d_a_obj_base.h>

SPECIAL_ACTOR_PROFILE(TAG_ROCK_BOAT, dTgRockBoat_c, fProfile::TAG_ROCK_BOAT, 0x173, 0, 3);

STATE_DEFINE(dTgRockBoat_c, Wait);

int dTgRockBoat_c::create() {
    mStateMgr.changeState(StateID_Wait);
    cooldown = 1;
    return SUCCEEDED;
}

int dTgRockBoat_c::doDelete() {
    return SUCCEEDED;
}

int dTgRockBoat_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dTgRockBoat_c::draw() {
    return SUCCEEDED;
}

void dTgRockBoat_c::initializeState_Wait() {}
void dTgRockBoat_c::executeState_Wait() {
    if (cooldown > 0 && --cooldown == 0) {
        dAcObjBase_c::create(fProfile::OBJ_ROCK_BOAT, roomid, (boatNum << 0x1c) | 0xff, &position, 0, 0, -1);
        cooldown = 0x259;
        if (++boatNum == 0xf) {
            boatNum = 0;
        }
    }
}
void dTgRockBoat_c::finalizeState_Wait() {}
