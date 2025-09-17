#include "d/t/d_t_time_door_beam.h"

#include "d/a/d_a_player.h"
#include "toBeSorted/area_math.h"

SPECIAL_ACTOR_PROFILE(TAG_TIME_DOOR_BEAM, dTgTimeDoorBeam_c, fProfile::TAG_TIME_DOOR_BEAM, 0x252, 0, 0);

int dTgTimeDoorBeam_c::create() {
    matrixCreateFromPosRotYScale(matrix, mPosition, mRotation.y, mScale, nullptr, 0.0f);
    return SUCCEEDED;
}

int dTgTimeDoorBeam_c::doDelete() {
    return SUCCEEDED;
}

int dTgTimeDoorBeam_c::actorExecute() {
    return SUCCEEDED;
}

int dTgTimeDoorBeam_c::draw() {
    return SUCCEEDED;
}

bool dTgTimeDoorBeam_c::checkPlayerPosInMtx() {
    if (dAcPy_c::LINK == nullptr) {
        return false;
    }

    return checkIfVec3fInMatrix(matrix, dAcPy_c::LINK->mPosition);
}
