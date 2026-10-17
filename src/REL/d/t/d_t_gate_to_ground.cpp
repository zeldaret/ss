#include "d/t/d_t_gate_to_ground.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_sc_game.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/event.h"

SPECIAL_ACTOR_PROFILE(GATE2GND_TAG, dTgGateToGround_c, fProfile::GATE2GND_TAG, 0x22, 0, 0);

int dTgGateToGround_c::create() {
    mArea.set(mPosition, mRotation.y, mScale, nullptr, 0.0f);

    mDelayFrameCount = 0;

    union {
        u32 val;
        struct {
            u16 a;
            u8 b;
            u8 c;
        };
    } v;
    v.val = mParams;
    mStoryflag = v.a;
    field_0x182 = v.b;
    mCameraID = v.c;

    return SUCCEEDED;
}

int dTgGateToGround_c::doDelete() {
    return SUCCEEDED;
}

int dTgGateToGround_c::actorExecute() {
    if (mStoryflag != -1 && StoryflagManager::sInstance->getCounterOrFlag(mStoryflag) == 0) {
        return SUCCEEDED;
    }

    dAcPy_c *player = dAcPy_c::GetLinkM();

    if (mArea.checkAreaBox(player->mPosition)) {
        if (player->getRidingActorType() != dAcPy_c::RIDING_LOFTWING) {
            if (mDelayFrameCount > 15) {
                Event e("CloudHole", 100, 0, nullptr, nullptr);
                mEventRelated.scheduleEvent(e, 0);
            } else {
                mDelayFrameCount++;
            }
        } else {
            if (mCameraID != -1) {
                // TODO Cast is maybe fake but I can't get overrideCam to match otherwise
                dScGame_c::getCamera(0)->getGameCam1()->overrideCam(mCameraID, 0, (u16)mRoomID, false);
            }
            mDelayFrameCount = 0;
        }
    } else {
        mDelayFrameCount = 0;
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
