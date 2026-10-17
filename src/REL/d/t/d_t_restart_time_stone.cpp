#include "d/t/d_t_restart_time_stone.h"

#include "f/f_base.h"
#include "toBeSorted/d_path.h"

SPECIAL_ACTOR_PROFILE(TAG_RESTART_TIME_STONE, dTgRestartTimeStone_c, fProfile::TAG_RESTART_TIME_STONE, 0x29A, 0, 0);

int dTgRestartTimeStone_c::create() {
    mArea.set(mPosition, mRotation.y, mScale, nullptr, 0.0f);

    ActorOnRail3 rail;
    u8 pathIdx = getFromParams(0, 0xFF);
    if (pathIdx != 0xFF) {
        rail.init(pathIdx, mRoomID, 0, 0, false);
        mPathStartPos.copyFrom(rail.getPath().getPoint(0));
    }
    return SUCCEEDED;
}

int dTgRestartTimeStone_c::doDelete() {
    return SUCCEEDED;
}

int dTgRestartTimeStone_c::actorExecute() {
    return SUCCEEDED;
}

int dTgRestartTimeStone_c::draw() {
    return SUCCEEDED;
}
