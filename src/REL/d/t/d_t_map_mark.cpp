#include "d/t/d_t_map_mark.h"

#include "d/d_stage.h"

SPECIAL_ACTOR_PROFILE(T_MAP_MARK, dTgMapMark_c, fProfile::T_MAP_MARK, 0x294, 0, 4);

int dTgMapMark_c::actorCreate() {
    shift4_0x8 = getFromParams(4, 0xFF);
    shift23_0x8 = getFromParams(23, 0xFF);
    mTriggerStoryFlag = getFromParams(12, 0x7FF);
    shift0_0x3 = getFromParams(0, 0x3);
    shift2_0x3 = getFromParams(2, 0x3);
    shift4_0x3f = (mRotation.x >> 4) & 0x3F;
    rotz_shift8_0xff = (mRotation.z >> 8) & 0xFF;
    if (rotz_shift8_0xff > 0xFF) {
        rotz_shift8_0xff = 0xFF;
    }
    rotz_shift0_0xff = mRotation.z;
    rotz_shift10_0x3 = ((mRotation.x >> 10) & 0x3) == 0;
    rotz_shift12_0x3 = ((mRotation.x >> 12) & 0x3) == 0;
    rotz_shift0_0xf = mRotation.x & 0xF;
    if (rotz_shift0_0xf >= 5) {
        rotz_shift0_0xf = 0;
    }
    mRotation.x.set(0);
    mRotation.z.set(0);
    field_0x10E = true;
    return SUCCEEDED;
}

int dTgMapMark_c::actorPostCreate() {
    updateRoomId(10.0f);
    if (mRoomID < 0) {
        mRoomID = dStage_c::GetInstance()->getCurrRoomId();
    }
    return SUCCEEDED;
}

int dTgMapMark_c::doDelete() {
    return SUCCEEDED;
}

int dTgMapMark_c::actorExecute() {
    return SUCCEEDED;
}

int dTgMapMark_c::draw() {
    return SUCCEEDED;
}
