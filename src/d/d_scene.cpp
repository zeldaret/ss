#include "d/d_scene.h"

#include "common.h"
#include "d/d_base.h"
#include "d/d_pad_nav.h"
#include "d/d_reset.h"
#include "d/d_rumble.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_fader_base.h"

static u16 RootActorID = fProfile::PROFILE_MAX;
static u32 RootActorParams = 0;
static bool gameStateIsActive = true;

extern "C" u32 lbl_80574FA0;
extern "C" u32 lbl_80574FA4;

dFader_c dScene_c::sFader;

dScene_c::dScene_c() {
    lbl_80574FA0 = 0;
    lbl_80574FA4 = 0;
    setProcControlFlag(ROOT_DISABLE_DRAW | ROOT_DISABLE_EXECUTE);
    dPadNav::setNavEnabled(false, false);
}

void dScene_c::postCreate(MAIN_STATE_e state) {
    if (state == SUCCESS) {
        dReset::Manage_c::GetInstance()->SetSoftResetFinish();
    }
    dBase_c::postCreate(state);
}

void dScene_c::postDelete(MAIN_STATE_e state) {
    if (state == SUCCESS) {
        gameStateIsActive = false;
        dReset::Manage_c::GetInstance()->PostDeleteScene();
    }
    dBase_c::postDelete(state);
}

int dScene_c::preExecute() {
    if (dBase_c::preExecute() == NOT_READY) {
        return NOT_READY;
    } else if (RootActorID != fProfile::PROFILE_MAX) {
        if (sFader.isStatus(mFaderBase_c::FADED_IN)) {
            sFader.fadeOut();
        } else if (sFader.isStatus(mFaderBase_c::FADED_OUT)) {
            deleteRequest();
            resetFlags();
            dRumble_c::stop(-1);
        }
        return NOT_READY;
    } else {
        if ((proc_control & ROOT_DISABLE_EXECUTE)) {
            if (!checkChildProcessCreateState()) {
                clearProcControlFlag(ROOT_DISABLE_EXECUTE);
                clearProcControlFlag(ROOT_DISABLE_DRAW);
            }
            return NOT_READY;
        }
        if (sFader.isStatus(mFaderBase_c::FADED_OUT)) {
            sFader.fadeIn();
        }
        return SUCCEEDED;
    }
}

void dScene_c::postExecute(MAIN_STATE_e status) {
    if (status != CANCELED) {
        dReset::Manage_c::GetInstance()->SetSoftResetScene();
    }
    dBase_c::postExecute(status);
    if (proc_control & ROOT_DISABLE_EXECUTE) {
        lbl_80574FA0++;
        lbl_80574FA4++;
    }
}

void dScene_c::staticCreate() {
    RootActorID = fProfile::BOOT;
    RootActorParams = 0;
    gameStateIsActive = false;
}

dBase_c *dScene_c::staticExecute() {
    if (gameStateIsActive || RootActorID == fProfile::PROFILE_MAX) {
        return nullptr;
    }

    dBase_c *base = dBase_c::createRoot(RootActorID, RootActorParams, SCENE);
    if (base != nullptr) {
        RootActorID = fProfile::PROFILE_MAX;
        gameStateIsActive = true;
        return base;
    }
    return nullptr;
}

void dScene_c::setRootActor(fProfile::PROFILE_NAME_e rootActor, u32 params, s32 fadeOutType, s32 fadeInType) {
    if (RootActorID != fProfile::PROFILE_MAX && rootActor != fProfile::TITLE) {
        return;
    }
    RootActorID = rootActor;
    RootActorParams = params;
    sFader.setTypes(fadeInType, fadeOutType);
}
