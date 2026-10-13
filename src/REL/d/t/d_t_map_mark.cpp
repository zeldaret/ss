#include "d/t/d_t_map_mark.h"

#include "d/d_stage.h"

SPECIAL_ACTOR_PROFILE(T_MAP_MARK, dTgMapMark_c, fProfile::T_MAP_MARK, 0x294, 0, 4);

int dTgMapMark_c::actorCreate() {
    mSceneflag = getFromParams(4, 0xFF);
    mToggleSceneflag = getFromParams(23, 0xFF);
    mTriggerStoryFlag = getFromParams(12, 0x7FF);
    mSceneflagCondition = getFromParams(0, 0x3);
    mStoryflagCondition = getFromParams(2, 0x3);
    field_0x104 = (mRotation.x >> 4) & 0x3F;
    field_0x0FE = (mRotation.z >> 8) & 0xFF;
    if (field_0x0FE > 0xFF) {
        field_0x0FE = 0xFF;
    }
    field_0x105 = mRotation.z;
    mShowPlusBtnHint = ((mRotation.x >> 10) & 0x3) == 0;
    mDisableOnToggle = ((mRotation.x >> 12) & 0x3) == 0;
    mMarkType = mRotation.x & 0xF;
    if (mMarkType >= 5) {
        mMarkType = 0;
    }
    mRotation.x.set(0);
    mRotation.z.set(0);
    mIsActive = true;
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
