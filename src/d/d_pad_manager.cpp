#include "d/d_pad_manager.h"

#include "common.h"
#include "d/d_cs_base.h"
#include "d/d_dvd_unk.h"
#include "d/d_gfx.h"
#include "d/d_pad.h"
#include "d/d_pad_nav.h"
#include "d/d_sys.h"
#include "d/lyt/d_lyt_battery.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "m/m_pad.h"

dPadManager_c::dPadManager_c() {
    sInstance = this;
    field_0x20 = true;
    mpWindow = nullptr;
    mCalibrationFinished = false;
    field_0x25 = false;
}

bool dPadManager_c::create(EGG::Heap *pHeap) {
    new (pHeap) dPadManager_c();
    sInstance->init();
    return true;
}

dPadManager_c::~dPadManager_c() {}

bool dPadManager_c::execute() {
    executeProc();
    return true;
}

bool dPadManager_c::draw() {
    dGfx_c::doDrawCapTexture();
    if (mpWindow != nullptr) {
        mpWindow->draw();
    }
    return true;
}

void dPadManager_c::init() {
    mSavedCsDrawDirectly = false;
    field_0x23 = false;
    ModeRequestNormal();
    field_0x1E = false;
    field_0x1F = false;
    if (mpWindow != nullptr) {
        mpWindow->init();
    }
    field_0x24 = false;
}

void dPadManager_c::buildWindow() {
    sInstance->mpWindow = new dLytSystemWindowMain_c();
    sInstance->mpWindow->build();
}

s32 dPadManager_c::get30SecondsInTicks() {
    return 1800 / dSys_c::getFrameRate();
}

s32 dPadManager_c::get3Point3SecondsInTicks() {
    return 200 / dSys_c::getFrameRate();
}

void dPadManager_c::requestMplsCalibration() {
    mCalibrationFinished = false;
    dPad::ex_c::setCalibrateMpls();
}

void dPadManager_c::executeProc() {
    static const ModeFunc procs[] = {
        &dPadManager_c::ModeProc_Normal,
        &dPadManager_c::ModeProc_ReconnectPad,
        &dPadManager_c::ModeProc_AttachMplsAndNunchuk,
        &dPadManager_c::ModeProc_AttachNunchuk,
        &dPadManager_c::ModeProc_Calibrating,
        &dPadManager_c::ModeProc_LowBattery,
        &dPadManager_c::ModeProc_PointCenter,
        &dPadManager_c::ModeProc_CalibrationFail,
    };
    ModeFunc f = procs[mMode];
    if (f != nullptr) {
        (this->*f)();
    }

    if (mpWindow != nullptr) {
        mpWindow->calc();
    }
}

void dPadManager_c::ModeRequestNormal() {
    ModeRequest(Normal);
    mIsError = false;
    field_0x1D = false;
    if (dCsBase_c::GetInstance() != nullptr) {
        dCsBase_c::GetInstance()->setDrawDirectly(mSavedCsDrawDirectly);
        if (field_0x23) {
            dPadNav::setCursorStickVisible();
        } else {
            dPadNav::setCursorStickInvisible();
        }
    }
}

void dPadManager_c::ModeProc_Normal() {
    PadStatus_e status = getPadStatus();
    if (status == PAD_NORMAL || status == PAD_CONFIGURING_MPLS) {
        if (isOutOfBattery() == true) {
            mIsError = true;
            dDvdUnk::FontUnk::GetInstance()->onError();
            ModeRequestLowBattery();
        }
        return;
    }

    // Some sort of error occured

    mIsError = true;
    mPadStatus = status;
    dCsBase_c *cs = dCsBase_c::GetInstance();
    mSavedCsDrawDirectly = cs->getDrawDirectly();
    cs->setDrawDirectly(false);
    cs->setCursorStickVisible(false);
    field_0x23 = dPadNav::isCursorStickVisible();
    dPadNav::setCursorStickInvisible();
    dDvdUnk::FontUnk::GetInstance()->onError();

    if (status == PAD_DISCONNECTED) {
        ModeRequestReconnectPad();
    } else if (status == PAD_NO_MPLS) {
        ModeRequestAttachMplsAndNunchuk();
    } else if (status == PAD_NO_NUNCHUK) {
        ModeRequestAttachNunchuk();
    } else if (status == PAD_NEED_POINT_CENTER) {
        ModeRequestPointCenter();
    } else {
        ModeRequestCalibrating();
    }
}

void dPadManager_c::ModeRequestReconnectPad() {
    ModeRequest(ReconnectPad);
    field_0x1D = true;
    mCalibrationFinished = false;
}
void dPadManager_c::ModeProc_ReconnectPad() {
    PadStatus_e status = getPadStatus();
    if (status == PAD_NEED_CALIBRATION || status == PAD_CONFIGURING_MPLS) {
        status = PAD_NORMAL;
    }

    switch (mStep) {
        case 0:
            // "Communications with the Wii Remote have been interrupted. Reconnect the Wii Remote.
            // Press (A) without touching the Control Stick."
            if (mpWindow->setProperties("SYS_CONT_10", true, nullptr) == true) {
                mStep = 1;
            }
            break;
        case 1:
            if (mPad::getCore()->isConnected() && mpWindow->requestABtnOut() == true) {
                mPad::getCore()->startPatternRumble("***--***--***", 0, false);
                mStepTimer = 0;
                mStep = 2;
            }
            break;
        case 2:
            if (mStepTimer < get3Point3SecondsInTicks()) {
                mStepTimer++;
            } else if (mpWindow->fn_80150EE0() == true) {
                mStep = 3;
            }
            break;
        case 3:
            if (mpWindow->getField_0xDF7() == true) {
                mStep = 4;
            }
            break;
        case 4: ModeRequestNext(status); break;
    }
}

void dPadManager_c::ModeRequestAttachMplsAndNunchuk() {
    ModeRequest(AttachMplsAndNunchuk);
    field_0x1D = true;
    mCalibrationFinished = false;
}
void dPadManager_c::ModeProc_AttachMplsAndNunchuk() {
    PadStatus_e status = getPadStatus();
    if (status == PAD_NEED_CALIBRATION || status == PAD_CONFIGURING_MPLS) {
        status = PAD_NORMAL;
    }

    switch (mStep) {
        case 0:
            // "Connect the Wii Remote and Nunchuk to the Wii MotionPlus accessory. Next, wait a few moments without
            // touching the Control Stick."
            if (mpWindow->setProperties("SYS_CONT_01_02", false, nullptr) == true) {
                mpWindow->fn_80150F30(2);
                mStep = 1;
            }
            break;
        case 1:
            if (mpWindow->getField_0xDF7() == true) {
                mStep++;
            }
            // fall-through
        case 2:
            if (status == PAD_DISCONNECTED) {
                if (mpWindow->fn_80150EE0() == true) {
                    mStep = 4;
                }
            } else {
                if (dPad::ex_c::isMissingMpls() != true) {
                    mStep = 3;
                    mStepTimer = 0;
                }
            }
            break;
        case 3:
            if (mStepTimer < get3Point3SecondsInTicks()) {
                mStepTimer++;
            } else if (mpWindow->fn_80150EE0() == true) {
                mStep = 4;
                mStepTimer = 0;
                dPad::ex_c::fn_800590F0();
            }
            break;
        case 4:
            if (mpWindow->getField_0xDF7() == true) {
                mStep = 5;
            }
            break;
        case 5: ModeRequestNext(status); break;
    }
}

void dPadManager_c::ModeRequestAttachNunchuk() {
    ModeRequest(AttachNunchuk);
}
void dPadManager_c::ModeProc_AttachNunchuk() {
    PadStatus_e status = getPadStatus();
    if (status == PAD_NEED_CALIBRATION || status == PAD_CONFIGURING_MPLS) {
        status = PAD_NORMAL;
    }

    switch (mStep) {
        case 0:
            // "Connect the Nunchuk to the Wii Remote. Once you have connected the Nunchuk, wait a few moments without
            // touching the Control Stick."
            if (mpWindow->setProperties("SYS_CONT_01_01", false, nullptr) == true) {
                mpWindow->fn_80150F30(1);
                mStep = 1;
                mStepTimer = 0;
            }
            break;
        case 1:
            if (mpWindow->getField_0xDF7() == true) {
                mStep++;
            }
            break;
        case 2:
            if (status == PAD_DISCONNECTED) {
                if (mpWindow->fn_80150EE0() == true) {
                    mStep = 3;
                }
            } else if (status == PAD_NO_MPLS) {
                if (mpWindow->fn_80150EE0() == true) {
                    mStep = 3;
                }
            } else if (mStepTimer < get1Point5SecondsInTicks()) {
                mStepTimer++;
            } else if (dPad::ex_c::isMissingNunchuk() != true && mpWindow->fn_80150EB0() == true) {
                mStep = 3;
                mStepTimer = 0;
            }
            break;
        case 3:
            if (mpWindow->getField_0xDF7() == true) {
                mStep = 4;
            }
            break;
        case 4: ModeRequestNext(status); break;
    }
}

void dPadManager_c::ModeRequestCalibrating() {
    ModeRequest(Calibrating);
    mCalibrationTimer = 0;
    field_0x1D = true;
    mCalibrationFinished = false;
    field_0x24 = false;
}
void dPadManager_c::ModeProc_Calibrating() {
    PadStatus_e status = getPadStatus();
    bool empty = false;
    if (status == PAD_NEED_CALIBRATION) {
        empty = true;
        status = PAD_NORMAL;
    }

    static const char *s_messages[] = {
        "SYS_MPLS_01", // "Now calibrating the Wii MotionPlus. Place the Wii Remote on a flat surface."
        "SYS_MPLS_05", // ""
    };

    switch (mStep) {
        case 0: {
            s32 idx = 0;
            if (empty) {
                idx = 1;
            }
            if (mpWindow->setProperties(s_messages[idx], false, nullptr) == true) {
                mpWindow->fn_80150F30(3);
                mpWindow->fn_80150FE0();
                mpWindow->fn_80151000();
                mStep = 1;
                mStepTimer = 0;
            }
            break;
        }
        case 1:
            if (mpWindow->getField_0xDF7() == true) {
                mStep++;
            }
            break;
        case 2:
            if (status != PAD_NORMAL && status != PAD_CONFIGURING_MPLS) {
                mStep = 20;
            } else if (mStepTimer < get1Point5SecondsInTicks()) {
                mStepTimer++;
            } else if (dPad::ex_c::startCurrentMplsCalibration() == false) {
                mStep = 3;
                mStepTimer = 0;
            } else if (mCalibrationTimer >= get30SecondsInTicks() && mpWindow->fn_80150EE0() == true) {
                mpWindow->fn_80150FF0();
                mStep = 10;
                mCalibrationTimer = 0;
            } else {
                mCalibrationTimer++;
            }
            break;
        case 3:
            if (status != PAD_NORMAL) {
                dPad::ex_c::stopCurrentMplsCalibration();
                mStep = 20;
            } else if (!dPad::ex_c::getCurrentCalibrationWork()) {
                // okay cool everywhere else we do "== true" and so on
                // but here the float is simply converted to bool
                // done calibrating
                if (mpWindow->fn_80150EE0() == true) {
                    mStep = 4;
                }
            } else if (mCalibrationTimer >= get30SecondsInTicks() && mpWindow->fn_80150EE0() == true) {
                mpWindow->fn_80150FF0();
                mStep = 10;
                mCalibrationTimer = 0;
            } else {
                mCalibrationTimer++;
            }
            break;
        case 4:
            if (mpWindow->getField_0xDF7() == true) {
                field_0x1D = false;
                mCalibrationFinished = true;
                ModeRequestPointCenter();
            }
            break;
        case 10:
            if (mpWindow->getField_0xDF7() == true) {
                ModeRequestCalibrationFail();
            }
            break;
        case 20:
            mpWindow->fn_80150FF0();
            if (mpWindow->fn_80150EE0() == true) {
                mStep = 21;
            }
            break;
        case 21:
            if (mpWindow->getField_0xDF7() == true) {
                if (status == PAD_CONFIGURING_MPLS) {
                    mStep = 30;
                } else {
                    ModeRequestNext(status);
                }
            }
            break;
        case 30: ModeRequestCalibrationFail(); break;
    }
}

void dPadManager_c::ModeRequestLowBattery() {
    ModeRequest(LowBattery);
    mCalibrationTimer = 0;
    field_0x20 = false;
}
void dPadManager_c::ModeProc_LowBattery() {
    PadStatus_e status = getPadStatus();
    switch (mStep) {
        case 0:
            // "The Wii Remote battery charge is running low. Please check the HOME Menu."
            if (mpWindow->setProperties("SYS_CONT_09", false, nullptr) == true) {
                mStep = 1;
                mStepTimer = 0;
            }
            break;
        case 1:
            if (mpWindow->getField_0xDF7() == true) {
                mStep++;
            }
            break;
        case 2:
            if (status != PAD_NORMAL) {
                mStep = 10;
            } else if (mStepTimer < get1Point5SecondsInTicks()) {
                mStepTimer++;
            } else if (mpWindow->fn_80150FB0() == true) {
                mStep = 3;
                mStepTimer = 0;
            }
            break;
        case 3:
            if (status != PAD_NORMAL) {
                mStep = 10;
            } else if (mPad::getCore()->downTrigger(mPad::BUTTON_A) && mpWindow->fn_80150EB0() == true) {
                mStep = 4;
            }
            break;
        case 4:
            if (mpWindow->getField_0xDF7() == true) {
                dLytBattery_c::GetInstance()->dismissManually();
                ModeRequestNext(status);
            }
            break;
        case 10:
            if (mpWindow->fn_80150EE0() == true) {
                mStep = 11;
            }
            break;
        case 11:
            if (mpWindow->getField_0xDF7() == true) {
                dLytBattery_c::GetInstance()->dismissManually();
                ModeRequestNext(status);
            }
            break;
    }
}

void dPadManager_c::ModeRequestPointCenter() {
    ModeRequest(PointCenter);
    field_0x25 = true;
    if (field_0x20 == false && dPad::ex_c::getBatteryLevel() >= 2) {
        field_0x20 = true;
    }
}
void dPadManager_c::ModeProc_PointCenter() {
    PadStatus_e status = getPadStatus();
    if (status == PAD_NEED_CALIBRATION || status == PAD_CONFIGURING_MPLS) {
        status = PAD_NORMAL;
    }

    switch (mStep) {
        case 0:
            // "Point the Wii Remote at the screen, and move <pointer icon> to the center."
            if (mpWindow->setProperties("SYS_MPLS_02", false, nullptr) == true) {
                dCsBase_c::GetInstance()->setVisible(true);
                dCsBase_c::GetInstance()->setDrawDirectly(true);
                dCsBase_c::GetInstance()->setCalibrationPointCenterEnabled(true);
                mStep = 1;
            }
            break;
        case 1:
            if (mpWindow->getField_0xDF7() == true) {
                mStep++;
            }
            break;
        case 2:
            if (status != PAD_NORMAL) {
                mStep = 10;
            } else if (mpWindow->isPointerWithinWindow() != false &&
                       // "Press (A)."
                       mpWindow->setProperties("SYS_MPLS_07", false, nullptr) == true) {
                mpWindow->fn_80150FB0();
                mPad::getCore()->startPatternRumble("**-*----", 0, false);
                mStepTimer = 0;
                mStep++;
            }
            break;
        case 3:
            if (mpWindow->getField_0xDF7() == true) {
                mStep = 5;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_POINTER_IN);
            }
            break;
        case 4:
            if (status != PAD_NORMAL) {
                mStep = 10;
            } else if (mpWindow->isPointerWithinWindow() == false &&
                       // "Point the Wii Remote at the screen, and move <pointer icon> to the center."
                       mpWindow->setProperties("SYS_MPLS_02", false, nullptr) == true) {
                mStep = 1;
            } else if (mStepTimer < 8) {
                mStepTimer++;
            } else if (mpWindow->fn_80150FB0() == true) {
                mStepTimer = 0;
                mStep++;
            }
            break;
        case 5:
            if (mpWindow->getField_0xDF8() == true) {
                if (status != PAD_NORMAL) {
                    mStep = 20;
                } else if (mpWindow->isPointerWithinWindow() == false &&
                           // "Point the Wii Remote at the screen, and move <pointer icon> to the center."
                           mpWindow->setProperties("SYS_MPLS_02", false, nullptr) == true) {
                    mpWindow->setField_0xE03(true);
                    mStep = 1;
                } else if (mPad::getCore()->downTrigger(mPad::BUTTON_A) != false && mpWindow->fn_80150EB0() == true) {
                    mStepTimer = 0;
                    mStep++;
                    dPad::ex_c::centerCurrentCursor();
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_POINTER_OK);
                }
            }
            break;
        case 6:
            if (mpWindow->getField_0xDF7() == true) {
                field_0x1E = true;
                field_0x25 = false;
                dCsBase_c::GetInstance()->setVisible(false);
                dCsBase_c::GetInstance()->setDrawDirectly(false);
                dCsBase_c::GetInstance()->setCalibrationPointCenterEnabled(false);
                ModeRequestNormal();
                return;
            }
            break;
        case 10:
            if (mpWindow->fn_80150EE0() == true) {
                mStep = 11;
            }
            break;
        case 11:
            if (mpWindow->getField_0xDF7() == true) {
                ModeRequestNext(status);
                dCsBase_c::GetInstance()->setVisible(false);
                dCsBase_c::GetInstance()->setDrawDirectly(false);
                dCsBase_c::GetInstance()->setCalibrationPointCenterEnabled(false);
                return;
            }
            break;
        case 20:
            if (mpWindow->fn_80150EE0() == true) {
                mpWindow->setField_0xE03(true);
                mStep = 11;
            }
            break;
    }
    dCsBase_c::GetInstance()->update();
}

void dPadManager_c::ModeRequestCalibrationFail() {
    ModeRequest(CalibrationFail);
}
void dPadManager_c::ModeProc_CalibrationFail() {
    PadStatus_e status = getPadStatus();

    switch (mStep) {
        case 0:
            // "Could not successfully complete the calibration check. Run the check again? [1]Check [2]Don't check"
            if (mpWindow->setProperties("SYS_MPLS_03", false, nullptr) == true) {
                mpWindow->fn_80150F30(4);
                mStep = 1;
                mStepTimer = 0;
            }
            break;
        case 1:
            if (status != PAD_NORMAL && status != PAD_CONFIGURING_MPLS && status != PAD_NEED_CALIBRATION) {
                mStep = 14;
            } else if (mStepTimer < get1Point5SecondsInTicks()) {
                mStepTimer++;
            } else if (mPad::getCore()->downTrigger(mPad::BUTTON_A) != false && mpWindow->getField_0xDDC() != 2 &&
                       mpWindow->fn_80150EE0() == true) {
                mStep = 2;
                mStepTimer = 0;
            }
            break;
        case 2:
            if (mpWindow->getField_0xDF7() == true) {
                mStep = 3;
            }
            break;
        case 3:
            if (mpWindow->getField_0xDDC() == 0) {
                if (
                   // "If you proceed without finishing calibration, game controls may not work properly. Continue without finishing calibration? [1]Continue [2]Retry check"
                   mpWindow->setProperties("SYS_MPLS_04", false, nullptr) == true
               ) {
                    mStep = 4;
                    mpWindow->fn_80150F30(4);
                }
            } else {
                mStep = 6;
            }
            break;
        case 4:
            if (status != PAD_NORMAL && status != PAD_CONFIGURING_MPLS && status != PAD_NEED_CALIBRATION) {
                mStep = 14;
            } else if (mPad::getCore()->downTrigger(mPad::BUTTON_A) != false && mpWindow->getField_0xDDC() != 2) {
                if (mpWindow->getField_0xDDC() == 0) {
                    if (mpWindow->fn_80150EE0() == true) {
                        mStep = 5;
                    }
                } else {
                    if (mpWindow->fn_80150EB0() == true) {
                        field_0x1D = false;
                        dPad::ex_c::getInstance()->setNoCalibrationNeeded();
                        field_0x24 = true;
                        field_0x25 = false;
                        mStep = 16;
                    }
                }
            }
            break;
        case 5:
            if (mpWindow->getField_0xDF7() == true) {
                mStep = 6;
            }
            break;
        case 6:
            // "Now calibrating the Wii MotionPlus. Place the Wii Remote on a flat surface."
            if (mpWindow->setProperties("SYS_MPLS_01", false, nullptr) == true) {
                mpWindow->fn_80150F30(3);
                mpWindow->fn_80150FE0();
                mpWindow->fn_80151000();
                mCalibrationTimer = 0;
                mStep = 7;
            }
            break;
        case 7:
            if (mpWindow->getField_0xDF7() == true) {
                mStep = 8;
                mStepTimer = 0;
            }
            break;
        case 8:
            if (status != PAD_NORMAL && status != PAD_NEED_CALIBRATION) {
                mStep = 12;
            } else if (mStepTimer < get1Point5SecondsInTicks()) {
                mStepTimer++;
            } else if (dPad::ex_c::startCurrentMplsCalibration() == false) {
                mStep = 9;
                mStepTimer = 0;
                mCalibrationTimer = 0;
            } else if (mCalibrationTimer >= get30SecondsInTicks() && mpWindow->fn_80150EE0() == true) {
                mpWindow->fn_80150FF0();
                mStep = 11;
                mCalibrationTimer = 0;
            } else {
                mCalibrationTimer++;
            }
            break;
        case 9:
            if (status != PAD_NORMAL && status != PAD_NEED_CALIBRATION) {
                dPad::ex_c::stopCurrentMplsCalibration();
                mStep = 12;
            } else if (!dPad::ex_c::getCurrentCalibrationWork() && mpWindow->fn_80150EE0() == true) {
                // okay cool everywhere else we do "== true" and so on
                // but here the float is simply converted to bool
                // done calibrating
                mStep = 10;
            } else if (mCalibrationTimer >= get30SecondsInTicks() && mpWindow->fn_80150EE0() == true) {
                mpWindow->fn_80150FF0();
                mStep = 11;
                mCalibrationTimer = 0;
            } else {
                mCalibrationTimer++;
            }
            break;
        case 10:
            if (mpWindow->getField_0xDF7() == true) {
                field_0x1D = false;
                mCalibrationFinished = true;
                ModeRequestPointCenter();
            }
            break;
        case 11:
            if (mpWindow->getField_0xDF7() == true) {
                ModeRequestCalibrationFail();
            }
            break;
        case 12:
            mpWindow->fn_80150FF0();
            if (mpWindow->fn_80150EE0() == true) {
                mStep = 13;
            }
            break;
        case 13:
            if (mpWindow->getField_0xDF7() == true) {
                if (status == PAD_CONFIGURING_MPLS) {
                    ModeRequestCalibrationFail();
                } else {
                    ModeRequestNext(status);
                }
            }
            break;
        case 14:
            if (mpWindow->fn_80150EB0() == true) {
                mpWindow->setField_0xE04(true);
                mStep = 15;
            }
            break;
        case 15:
            if (mpWindow->getField_0xDF7() == true) {
                ModeRequestNext(status);
            }
            break;
        case 16:
            if (mpWindow->getField_0xDF7() == true) {
                ModeRequestNext(status);
            }
            break;
    }
}

void dPadManager_c::ModeRequest(Mode_e mode) {
    mMode = mode;
    mStep = 0;
    mStepTimer = 0;
    mCalibrationTimer = 0;
}

dPadManager_c::PadStatus_e dPadManager_c::getPadStatus() {
    PadStatus_e ret = PAD_NORMAL;
    if (dDvdUnk::FontUnk::GetInstance()->padErrorsAllowed() != true) {
        return PAD_NORMAL;
    }

    if (!mPad::getCore()->isConnected()) {
        ret = PAD_DISCONNECTED;
    } else if (dPad::ex_c::isMissingMpls() == true || dPad::ex_c::fn_800590E0()) {
        ret = PAD_NO_MPLS;
    } else if (dPad::ex_c::isMissingNunchuk() == true) {
        ret = PAD_NO_NUNCHUK;
    } else {
        if (mMode == Normal) {
            if (dPad::ex_c::needMplsCalibration()) {
                ret = PAD_NEED_CALIBRATION;
            } else if (field_0x25) {
                ret = PAD_NEED_POINT_CENTER;
            }
        }
        if (dPad::ex_c::fn_800590B0()) {
            ret = PAD_CONFIGURING_MPLS;
        }
    }

    return ret;
}

void dPadManager_c::ModeRequestNext(PadStatus_e status) {
    if (status == PAD_DISCONNECTED) {
        ModeRequestReconnectPad();
    } else if (status == PAD_NO_MPLS) {
        ModeRequestAttachMplsAndNunchuk();
    } else if (status == PAD_NO_NUNCHUK) {
        ModeRequestAttachNunchuk();
    } else if (status == PAD_NEED_CALIBRATION) {
        ModeRequestCalibrating();
    } else if (field_0x1D == true) {
        ModeRequestCalibrating();
    } else if (field_0x25 == true) {
        ModeRequestPointCenter();
    } else if (isOutOfBattery() == true) {
        ModeRequestLowBattery();
    } else {
        ModeRequestNormal();
    }
}

s32 dPadManager_c::get1Point5SecondsInTicks() {
    return 90 / dSys_c::getFrameRate();
}

bool dPadManager_c::isOutOfBattery() {
    if (dDvdUnk::FontUnk::GetInstance()->padErrorsAllowed() != true) {
        return false;
    }

    if (dPad::ex_c::isOutOfBattery() && field_0x20 == true) {
        return true;
    }
    return false;
}
