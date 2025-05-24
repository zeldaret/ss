#include "d/t/d_t_gate_to_ground.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_player_act.h"
#include "d/d_sc_game.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/area_math.h"
#include "toBeSorted/event.h"

SPECIAL_ACTOR_PROFILE(GATE2GND_TAG, dTgGateToGround_c, fProfile::GATE2GND_TAG, 0x22, 0, 0);

int dTgGateToGround_c::create() {
    matrixCreateFromPosRotYScale(matrix, position, rotation.y, mScale, nullptr, 0.0f);

    delayFrames = 0;

    union {
        u32 val;
        struct {
            u16 a;
            u8 b;
            u8 c;
        };
    } v;
    v.val = params;
    params_FF_FF_00_00 = v.a;
    params_00_00_FF_00 = v.b;
    params_00_00_00_FF = v.c;

    return SUCCEEDED;
}

int dTgGateToGround_c::doDelete() {
    return SUCCEEDED;
}

int dTgGateToGround_c::actorExecute() {
    dAcPy_c *player;
    dCamera_c *camera;

    if (params_FF_FF_00_00 == -1 || StoryflagManager::sInstance->getCounterOrFlag(params_FF_FF_00_00) != 0) {
        player = dAcPy_c::LINK;
        if (checkIfVec3fInMatrix(matrix, player->position)) {
            if (player->getRidingActorType() == dAcPy_c::RIDING_LOFTWING) {
                if (params_00_00_00_FF != -1) {
                    camera = dScGame_c::getCamera(0);
                    fn_80080960(camera->getField_0xD98(), params_00_00_00_FF, 0, roomid, 0);
                }
                delayFrames = 0;
            } else if (delayFrames < 16) {
                ++delayFrames;
            } else {
                Event e = Event("CloudHole", 100, 0, 0, 0);
                mEventRelated.scheduleEvent(e, 0);
            }
        } else {
            delayFrames = 0;
        }
    }
    return SUCCEEDED;
}

int dTgGateToGround_c::actorExecuteInEvent() {
    if (mEventRelated.isThisActorInEvent() != 0) {
        mEventRelated.advanceNext();
    }
    return SUCCEEDED;
}

int dTgGateToGround_c::draw() {
    return SUCCEEDED;
}
