#include "d/t/d_t_gate_to_ground.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_camera.h"
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
    dCamera_c *cam;

    if (params_FF_FF_00_00 != -1 && StoryflagManager::sInstance->getCounterOrFlag(params_FF_FF_00_00) == 0) {
        return SUCCEEDED;
    }

    player = dAcPy_c::LINK;

    if (checkIfVec3fInMatrix(matrix, player->position)) {
        if (player->getRidingActorType() != dAcPy_c::RIDING_LOFTWING) {
            if (delayFrames > 15) {
                Event e("CloudHole", 100, 0, nullptr, nullptr);
                mEventRelated.scheduleEvent(e, 0);
            } else {
                delayFrames++;
            }
        } else {
            if (params_00_00_00_FF != -1) {
                cam = dScGame_c::getCamera(0);
                fn_80080960(cam->getField_0xD98(), params_00_00_00_FF, 0, roomid, 0);
            }
            delayFrames = 0;
        }
    } else {
        delayFrames = 0;
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
