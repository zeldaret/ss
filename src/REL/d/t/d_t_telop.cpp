#include "d/t/d_t_telop.h"

#include "d/lyt/d_lyt_area_caption.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(TLP_TAG, dTgTelop_c, fProfile::TLP_TAG, 0x28C, 0, 0);

int dTgTelop_c::create() {
    mArea = getArea();
    mSceneflag = getSceneflag() != 0xFF ? getSceneflag() : 0xFF;
    mTempflag = getTempflag() != 0xFF ? getTempflag() : 0xFF;

    return SUCCEEDED;
}

int dTgTelop_c::doDelete() {
    return SUCCEEDED;
}

int dTgTelop_c::actorExecute() {
    if (EventManager::isInEvent()) {
        return SUCCEEDED;
    }

    if (mTempflag != 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, mTempflag)) {
        return SUCCEEDED;
    }

    if (mSceneflag != 0xFF && !SceneflagManager::sInstance->checkBoolFlag(roomid, mSceneflag)) {
        return SUCCEEDED;
    }

    if (mTempflag != 0xFF) {
        SceneflagManager::sInstance->setFlag(roomid, mTempflag);
    }

    dLytAreaCaption_c::setArea(mArea);
    deleteRequest();
    return SUCCEEDED;
}

int dTgTelop_c::draw() {
    return SUCCEEDED;
}
