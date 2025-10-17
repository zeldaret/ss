#include "d/d_pad.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "d/d_cs_game.h"
#include "d/d_gfx.h"
#include "d/d_hbm.h"
#include "d/d_pad_manager.h"
#include "d/d_pad_nav.h"
#include "d/d_reset.h"
#include "d/d_sc_game.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_map.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/core/eggController.h"
#include "egg/core/eggHeap.h"
#include "egg/math/eggMath.h"
#include "egg/math/eggVector.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_pad.h"
#include "m/m_vec.h"
#include "nw4r/math/math_types.h"
#include "rvl/KPAD/KPAD.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/mtxvec.h"
#include "rvl/MTX/vec.h"
#include "rvl/WPAD/WPAD.h"

#include <cmath>

namespace dPad {

ex_c ex_c::m_ex[4];
ex_c *ex_c::m_current_ex = ex_c::m_ex;

WPADInfo ex_c::m_info[2][4];

bool ex_c::m_connected[4];

// TODO - the SDK decomp uses WPAD error codes for this
// callback, but I think they have different semantics.
// Refer to https://github.com/doldecomp/sdk_2009-12-11/blob/main/source/kpad/KPAD.c#L3607-L3639
// for logic.
void control_mpls_callback(s32 idx, s32 code) {
    switch (code) {
        // Called first
        case 0: {
            ex_c::on_0x54(idx);
            break;
        }
        // Called when Mpls configuration failed - not actually reached?
        case -1: {
            ex_c::on_0x55(idx);
            break;
        }
        // Called when Mpls + Nunchuk configuration failed -- called a bunch of times
        case -2: {
            ex_c::on_0x56(idx);
            break;
        }
        // not sure - called last
        case 1: {
            ex_c::on_0x57(idx);
            break;
        }
    }
}

bool isDeviceTypeMpls(u32 type) {
    switch (type) {
        case WPAD_DEV_MOTION_PLUS:
        case WPAD_DEV_MPLS_PT_FS:
        case WPAD_DEV_MPLS_PT_CLASSIC:
        case WPAD_DEV_MPLS_PT_UNKNOWN: return true;
        default:                       return false;
    }
}

void initMpls(s32 chan) {
    KPADResetMpls(chan);
    KPADDisableMplsZeroPlay(chan);
    KPADSetMplsZeroPlayParam(chan, 0.005f);
    KPADSetMplsZeroDriftMode(chan, 2);
    disableMplsDirRevise(chan);
    KPADSetMplsDirReviseParam(chan, 0.35f);
    KPADEnableMplsAccRevise(chan);
    KPADSetMplsAccReviseParam(chan, 0.03f, 0.4f);
    KPADEnableMplsDpdRevise(chan);
    KPADSetMplsDpdReviseParam(chan, 0.5f);
}

extern "C" void fn_80194080();
void create() {
    mPad::create();

    KPADSetBtnRepeat(mPad::getCore(0)->getChannelID(), 0.5f, 0.1f);
    ex_c::resetInfo();

    KPADSetMplsWorkarea(new (0x20) u8[KPADGetMplsWorkSize()]);

    for (int i = 0; i < 4; ++i) {
        ex_c::m_ex[i].mMotion.init();
        KPADSetReviseMode(i, 1);
        KPADSetControlMplsCallback(i, control_mpls_callback);
        ex_c::m_ex[i].gotoStateWaitForConnect(i);
    }
    fn_80194080();
}

void setMpls(bool enable, s32 chan) {
    if (enable) {
        if (ex_c::m_ex[chan].mMplsEnabled == false) {
            ex_c::m_ex[chan].mMplsEnabled = true;
            // WPAD_DEV_MODE_MPLS_PT_FS
            // i.e. Skyward Sword wants both MotionPlus and Nunchuk
            KPADEnableMpls(chan, 5);
            initMpls(chan);
        }
    } else {
        if (ex_c::m_ex[chan].mMplsEnabled) {
            ex_c::m_ex[chan].mMplsEnabled = false;
            KPADDisableMpls(chan);
        }
    }
}

void convertDpdPosToScreenPos(mVec2_c &in, mVec2_c &out) {
    out.x = dGfx_c::getWidth4x3F() * 0.5f * (1.f + in.x) + dGfx_c::getWidth4x3LeftF();
    out.y = dGfx_c::getCurrentScreenHeightF() * -0.5f * (1.f + in.y) + dGfx_c::getCurrentScreenTopF();
}

void beginPad_BR() {
    mPad::beginPad();

    if (mPad::getCore(0)->isConnected()) {
        WPADSetAcceptConnection(0);
    } else {
        WPADSetAcceptConnection(1);
    }

    // Forces only first controller to be active
    for (int i = 1; i < 4; ++i) {
        if (mPad::getCore(i)->isConnected()) {
            WPADDisconnect(i);
        }
    }

    dPad::ex_c &ex = ex_c::m_ex[0];
    ex.fn_800572A0(0);
    if (mPad::getCore(0)->isConnected()) {
        ex.getUnifiedWpadStatus(0);
        ex.fn_80059300(0);
        ex.fn_80056AF0(0);
        ex.fn_80056B90(0);
        ex.fn_80056CE0(0);
        ex.workMplsCalibration(0);
        bool isDpdReviseEnabled = KPADIsEnableMplsDpdRevise(0) >= 0.f;
        if (ex.field_0x22CE) {
            if (isDpdReviseEnabled) {
                KPADDisableMplsDpdRevise(0);
            }
        } else {
            if (!isDpdReviseEnabled) {
                KPADEnableMplsDpdRevise(0);
            }
        }
        ex.field_0x22CE = false;

        const dAcPy_c *player = dAcPy_c::GetLink();
        if (player && player->isAttacking()) {
            mMtx_c m;
            mVec3_c baseX;
            mVec3_c baseY;
            mVec3_c baseZ;
            KPADVec kpadvec;
            m.XrotS(mAng::deg2short(-30.f));
            baseZ.z = m.m[2][2];
            baseZ.y = m.m[1][2];
            baseZ.x = m.m[0][2];
            kpadvec[2].x = baseZ.x;
            kpadvec[2].y = baseZ.y;
            kpadvec[2].z = baseZ.z;

            VECCrossProduct(ex_c::m_ex[0].mMPLS.mY, baseZ, baseX);
            if (VECMag(baseX) > 0.05f) {
                baseX.normalize();
                kpadvec[0].z = baseX.z;
                kpadvec[0].y = baseX.y;
                kpadvec[0].x = baseX.x;
            } else {
                baseX = baseZ;
                kpadvec[0].z = baseX.z;
                kpadvec[0].y = baseX.y;
                kpadvec[0].x = baseX.x;
            }

            VECCrossProduct(baseZ, baseX, baseY);
            if (VECMag(baseY) > 0.05f) {
                baseY.normalize();
                kpadvec[1].z = baseY.z;
                kpadvec[1].y = baseY.y;
                kpadvec[1].x = baseY.x;
            } else {
                baseY = baseZ;
                kpadvec[1].z = baseY.z;
                kpadvec[1].y = baseY.y;
                kpadvec[1].x = baseY.x;
            }

            KPADSetMplsDirReviseBase(0, kpadvec);
            enableMplsDirRevise(0);
        } else {
            disableMplsDirRevise(0);
        }

        KPADEnableMplsAccRevise(0);
        KPADSetMplsAccReviseParam(0, 0.03f, 0.4f);

        if ((dCsGame_c::GetInstance() != nullptr && dCsGame_c::GetInstance()->shouldDraw() && !ex.field_0x22D0) ||
            (dAcPy_c::GetLink() != nullptr &&
             dAcPy_c::GetLink()->checkActionFlagsCont(0x400 | 0x100 | 0x80 | 0x40 | 0x10 | 0x4 | 0x2 | 0x1) &&
             !dAcPy_c::GetLink()->vt_0x1C0() && !dLytMeter_c::GetMain()->getField_0x1377F()) ||
            ex.field_0x22CF) {
            if (dLytControlGame_c::getInstance() && dLytControlGame_c::getInstance()->isStateNormal()) {
                if (!(dPadManager_c::GetInstance() && dPadManager_c::GetInstance()->getField_0x25())) {
                    KPADDisableMplsDpdRevise(0);
                    KPADDisableMplsAccRevise(0);
                }
            }
            KPADDisableMplsDirRevise(0);
        }
        if (ex.mIsCalibrating || ex.field_0x48 != 0) {
            KPADEnableMplsAccRevise(0);
            KPADSetMplsAccReviseParam(0, 1.f, 0.6f);
            if (--ex.field_0x48 < 0) {
                ex.field_0x48 = 0;
            }
        }

        ex.field_0x22CF = false;
        ex.mFSStick.x = mPad::getCore(0)->getFreeStickX();
        ex.mFSStick.y = mPad::getCore(0)->getFreeStickY();
        ex.mFSStickDistance = ex.mFSStick.length();
        ex.mFSStickAngle = -ex.mFSStick.ang();

        ex.calcFSStickDirMask();

        bool isMpls = mPad::isMpls(0) || mPad::isMplsPtFS(0);
        if (isMpls) {
            ex.mMPLSVelocity.copyFrom(&mPad::getCore(0)->getCoreStatus()->mpls_rot);
            ex.mMPLS.mX.copyFrom(&mPad::getCore(0)->getCoreStatus()->mpls_basis_x);
            ex.mMPLS.mY.copyFrom(&mPad::getCore(0)->getCoreStatus()->mpls_basis_y);
            ex.mMPLS.mZ.copyFrom(&mPad::getCore(0)->getCoreStatus()->mpls_basis_z);
        } else {
            ex.mMPLSVelocity.set(0.f, 0.f, 0.f);
            ex.mMPLS.mX = mVec3_c::Ex;
            ex.mMPLS.mY = mVec3_c::Ey;
            ex.mMPLS.mZ = mVec3_c::Ez;
        }

        ex.fn_80056790(0);

        if (ex.field_0x50) {
            ex.mDpdPos = ex.field_0x5C;
        } else {
            if (mPad::getCore(0)->getDpdValidFlag() > 0) {
                ex.mDpdPos = mPad::getDpdRawPos(0);
            } else {
                ex.mDpdPos.set(-2.f, -2.f);
            }
        }
        convertDpdPosToScreenPos(ex.mDpdPos, ex.mDpdPosScreen);
        ex.updateStatus(0);
    }

    ex_c::m_current_ex = &ex_c::m_ex[mPad::getCurrentCoreID()];

    if (dReset::Manage_c::GetInstance()->isSoftResetOrSafetyWait()) {
        for (int i = 0; i < 4; ++i) {
            EGG::CoreController *core = mPad::g_core[i];
            core->getCoreStatus()->hold &= EGG::cCORE_BUTTON_HOME;
            core->getCoreStatus()->trig &= EGG::cCORE_BUTTON_HOME;
            core->getCoreStatus()->release &= EGG::cCORE_BUTTON_HOME;
        }
    }
    dPadNav::calc();
}

void endPad_BR() {
    mPad::endPad();
}

void connectCallback(const EGG::CoreControllerConnectArg &rArg) {
    switch (rArg.result) {
        case WPAD_ERR_OK: {
            dSndPlayerMgr_c::GetInstance()->setupRmtSpeaker(rArg.chan);
            ex_c::m_ex[rArg.chan].mDidConnect = true;
            break;
        }
        case WPAD_ERR_NO_CONTROLLER: {
            dSndPlayerMgr_c::GetInstance()->shutdownRmtSpeaker(rArg.chan);
            ex_c::m_ex[rArg.chan].mDidDisconnect = true;
            break;
        }
    }
}

void setConnectCallback() {
    EGG::CoreControllerMgr::setConnectCallback(connectCallback);
}

void enableMplsDirRevise(s32 chan) {
    KPADEnableMplsDirRevise(chan);
}

void disableMplsDirRevise(s32 chan) {
    KPADDisableMplsDirRevise(chan);
}

ex_c::ex_c()
    : mDpdPos(0.f, 0.f),
      mDpdPosScreen(0.f, 0.f),
      mFSStick(0.f, 0.f),
      mFSStickDistance(0.f),
      mFSStickAngle(0),
      mWPADProbeResult(0),
      mWPADProbeResultStable(0),
      mWPADProbeStableTimer(0),
      mWPADDeviceType(0),
      mWPADDeviceTypeStable(0xFF),
      mWPADDeviceTypeStableTimer(0),
      mConnectedStableTimer(0),
      field_0x3C(0),
      mCalibrationWork(0.f),
      field_0x44(false),
      field_0x45(false),
      field_0x46(false),
      mIsCalibrating(false),
      field_0x48(0),
      mDidConnect(false),
      mDidDisconnect(false),
      mNeedMplsCalibration(true),
      field_0x4F(false),
      field_0x50(true),
      field_0x51(0),
      mMplsEnabled(false),
      field_0x53(false),
      field_0x54(false),
      field_0x55(false),
      field_0x56(false),
      field_0x57(false),
      field_0x58(false),
      field_0x59(false),
      field_0x5A(false),
      field_0x5B(false),
      field_0x5C(0.f, 0.f),
      field_0x64(0.f, 0.f),
      mIsWPADDeviceTypeMplsStableTimer(0),
      field_0x70(0x1200),
      field_0x74(1.f, 0.f, 0.f),
      field_0x80(0.f, 1.f, 0.f),
      field_0x8C(0.f, 0.f, 1.f),
      mState(0),
      mOutOfHbmStableTimer(0),
      field_0x2288(0),
      field_0x22CE(0),
      field_0x22CF(0),
      field_0x22D0(0),
      mFSStickMaskChanged(false),
      mFSStickMask(0),
      field_0x22D8(0) {
    memset(&mStatus, 0, sizeof(mStatus));
}

void ex_c::fn_80055EF0(s32 chan) {
    const s32 readLen = mPad::getCore(chan)->getReadLength();

    // Shift all old acceleration values back
    for (int i = 120 - 1; i >= readLen; i--) {
        mMotion.mAccVecs[i] = mMotion.mAccVecs[i - readLen];
        mMotion.mAccValues[i] = mMotion.mAccValues[i - readLen];
        mFSMotion.mAccVecs[i] = mFSMotion.mAccVecs[i - readLen];
        mFSMotion.mAccValues[i] = mFSMotion.mAccValues[i - readLen];
    }

    // Write newly read Wiimote acceleration values
    // TODO - Missing an implicit != 0 check before the loop
    for (int i = 0; i < readLen; i++) {
        EGG::CoreStatus status = *mPad::getCore(chan)->getCoreStatus(i);
        mMotion.mAccVecs[i].copyFrom(&status.acc);
        mMotion.mAccValues[i] = status.acc_value;
    }

    mMotion.fn_80058AE0(chan, true);

    // Write newly read Nunchuk acceleration values
    for (int i = 0; i < readLen; ++i) {
        if (mPad::getCore(chan)->getDevType() == EGG::cDEV_FREESTYLE ||
            (mPad::isMplsPtFS(chan) && (u8)KPADGetMplsStatus(chan) == 5)) {
            EGG::CoreStatus status = *mPad::getCore(chan)->getCoreStatus(i);
            mFSMotion.mAccVecs[i].copyFrom(&status.acc);
            mFSMotion.mAccValues[i] = status.acc_value;
        } else {
            mFSMotion.mAccVecs[i].set(0.f, 0.f, 0.f);
            mFSMotion.mAccValues[i] = 0.f;
        }
    }
    if (mPad::getCore(chan)->getDevType() == EGG::cDEV_FREESTYLE ||
        (mPad::isMplsPtFS(chan) && (u8)KPADGetMplsStatus(chan) == 5)) {
        mFSMotion.fn_80058AE0(chan, false);
    }
}

void float_order_d_pad() {
    EGG::Math<f32>::epsilon();
}

void ex_c::fn_800562B0(s32 chan, mVec3_c &mpls) {
    if (field_0x53) {
        mpls = m_ex[chan].mMPLS.mY;
        mpls *= -1.f;
    } else {
        mpls = m_ex[chan].mMPLS.mZ;
    }
}

void ex_c::fn_80056330(s32 chan) {
    f32 mult = cM::minMaxLimit(1.f - field_0x8C.y * field_0x8C.y, 0.f, 1.f);

    mVec3_c basis;
    if (field_0x53) {
        basis = m_ex[chan].mMPLS.mZ;
    } else {
        basis = m_ex[chan].mMPLS.mY;
    }
    // ??
    field_0x74 = (mult * (mVec3_c::Ey - basis) + basis).cross(field_0x8C);

    f32 mag = field_0x74.mag();
    bool b = std::abs(mag) <= EGG::Math<f32>::epsilon();
    if (b) {
        field_0x74 = mVec3_c::Ex;
    } else {
        field_0x74 *= 1.f / mag;
    }

    field_0x80 = field_0x8C.cross(field_0x74);
    field_0x80.normalize();
}

void ex_c::fn_80056580(s32 chan, const mVec2_c &vec) {
    field_0x5C = vec;
    field_0x53 = false;

    mMtx_c m;
    m.ZXYrotS(-field_0x5C.y * field_0x70, field_0x5C.x * field_0x70, 0);
    MTXMultVecSR(m, m_ex[chan].mMPLS.mZ, field_0x8C);

    fn_80056330(chan);
}

void ex_c::centerCursor(s32 chan, bool b) {
    if (field_0x51 != 0 && b) {
        return;
    }
    field_0x64 = field_0x5C;

    // NON-MATCHING
    bool tmp = false;
    const EGG::Vector3f acc = mPad::getCore()->getAccelBad();
    if (dAcPy_c::GetLink() && dAcPy_c::GetLink()->checkActionFlagsCont(0x2) && acc.z < -0.9f) {
        tmp = true;
    }
    field_0x53 = tmp;

    fn_800562B0(chan, field_0x8C);
    fn_80056330(chan);

    if (b) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_POINTER_RESET);
        field_0x51 = 3;
    } else {
        field_0x51 = 0;
        field_0x5C.set(0.f, 0.f);
        mDpdPos.set(0.f, 0.f);
        convertDpdPosToScreenPos(mDpdPos, mDpdPosScreen);
    }
}

void ex_c::fn_80056790(s32 chan) {
    mVec2_c dpdRawPos = mPad::getDpdRawPos(chan);

    f32 f;
    if (dScGame_c::GetInstance() != nullptr && dLytMap_c::GetInstance() != nullptr &&
        !dLytMap_c::GetInstance()->isVisible()) {
        f = dScGame_c::GetInstance()->targetingScreenFn_801BBEC0();
    } else {
        f = 1.0f;
    }
    field_0x22D8 = 0;
    EGG::CoreStatus coreStatus = *mPad::getCore(chan)->getCoreStatus(0);
    if (coreStatus.dpd_valid_fg == 2) {
        field_0x22D8 = 1;
    }
    mVec3_c v;
    fn_800562B0(chan, v);
    mVec3_c v2(v.dot(field_0x74), v.dot(field_0x80), v.dot(field_0x8C));
    v2.z = nw4r::ut::Max(v2.z, field_0x70.cos());

    s16 b1 = cM::atan2s(-v2.x, v2.z);
    field_0x5C.x = cM::minMaxLimit(b1 * (1.0f / field_0x70), -1.0f, 1.0f);
    s16 b2 = cM::atan2s(-v2.y, v2.z);
    field_0x5C.y = cM::minMaxLimit(b2 * (1.0f / field_0x70), -f, f);

    if (field_0x51 != 0) {
        field_0x51--;
    }
    f32 f4 = field_0x51 * (1 / 3.0f);
    // lerp
    field_0x5C = field_0x5C * (1.0f - f4) + field_0x64 * f4;
}

void ex_c::setField_0x70(mAng ang) {
    field_0x70 = ang;
}

void ex_c::setField_0x70() {
    field_0x70.set(0x1200);
}

void ex_c::fn_80056AF0(s32 chan) {
    if (mDidConnect) {
        mDidConnect = false;
        mConnectedStableTimer = 90;
        field_0x44 = true;

        m_ex[chan].field_0x46 = true;
        m_ex[chan].field_0x3C = 120;
    } else {
        field_0x44 = false;
    }
    if (mDidDisconnect) {
        mDidDisconnect = false;
        field_0x45 = true;
        m_ex[chan].mWPADDeviceTypeStable = WPAD_DEV_UNKNOWN;
    } else {
        field_0x45 = false;
    }
}

bool ex_c::checkWPADProbeStable() {
    return mWPADProbeResultStable == WPAD_ERR_OK;
}

void ex_c::fn_80056B90(s32 chan) {
    u32 prevType = mWPADDeviceType;
    WPADResult prevResult = mWPADProbeResult;

    mWPADProbeResult = WPADProbe(chan, &mWPADDeviceType);

    if (prevResult == mWPADProbeResult) {
        mWPADProbeStableTimer++;
        if (mWPADProbeStableTimer >= 5) {
            mWPADProbeStableTimer = 5;
        }
    } else {
        mWPADProbeStableTimer = 0;
    }
    if (mWPADProbeStableTimer >= 5) {
        mWPADProbeResultStable = mWPADProbeResult;
    }
    if (mConnectedStableTimer > 0) {
        mConnectedStableTimer--;
        if (mConnectedStableTimer < 0) {
            mConnectedStableTimer = 0;
        }
    }

    if (prevType != mWPADDeviceType) {
        mWPADDeviceTypeStableTimer = 0;
    } else {
        mWPADDeviceTypeStableTimer++;
        if (mWPADDeviceTypeStableTimer > 20) {
            mWPADDeviceTypeStableTimer = 20;
        }
    }

    if (mWPADDeviceType != mWPADDeviceTypeStable && mWPADDeviceTypeStableTimer >= 20) {
        mWPADDeviceTypeStable = mWPADDeviceType;
    }

    if (!isDeviceTypeMpls(mWPADDeviceTypeStable)) {
        mNeedMplsCalibration = true;
        mIsWPADDeviceTypeMplsStableTimer = 0;
    }
    if (isDeviceTypeMpls(mWPADDeviceTypeStable) && mIsWPADDeviceTypeMplsStableTimer < 8) {
        mIsWPADDeviceTypeMplsStableTimer++;
    }
}

void ex_c::fn_80056CE0(s32 chan) {
    if (field_0x46 == false) {
        return;
    }

    if (field_0x3C == 1) {
        if (WPADCanSendStreamData(chan) && WPADGetRadioSensitivity(chan) > 50) {
            WPADResult res = getInfo(chan);
            if (res == WPAD_ERR_OK || res == WPAD_ERR_NO_CONTROLLER) {
                field_0x46 = false;
            } else {
                field_0x3C = 120;
            }
        } else {
            field_0x3C = 120;
        }
    }
    field_0x3C--;
    if (field_0x3C < 0) {
        field_0x3C = 0;
    }
}

void ex_c::startMplsCalibration(s32 chan) {
    KPADStartMplsCalibration(chan);
    mIsCalibrating = true;
    mCalibrationWork = 1.0f;
    field_0x48 = 0;
}

void ex_c::workMplsCalibration(s32 chan) {
    if (!mIsCalibrating) {
        return;
    }

    mCalibrationWork = KPADWorkMplsCalibration(chan);
    if (mCalibrationWork == 0.0f) {
        mIsCalibrating = false;
        field_0x48 = 60;
    }
}

f32 ex_c::getCalibrationWork() {
    return mCalibrationWork;
}

void ex_c::stopMplsCalibration(s32 chan) {
    mIsCalibrating = false;
    mCalibrationWork = 0.0f;
    KPADStopMplsCalibration(chan);
    field_0x48 = 0;
}

void ex_c::centerCursor(s32 chan) {
    centerCursor(chan, false);
    field_0x48 = 0;
}

void ex_c::gotoStateWaitForConnect(s32 chan) {
    mState = EX_STATE_WAITING_FOR_CONNECT;
}

void ex_c::executeStateWaitForConnect(s32 chan) {
    if (mPad::getCore(chan)->isConnected()) {
        gotoStatePostConnect(chan);
    }
}

void ex_c::gotoStatePostConnect(s32 chan) {
    mState = EX_STATE_POST_CONNECT;
}

void ex_c::executeStatePostConnect(s32 chan) {
    if (dHbm::Manage_c::GetInstance()->getState() == dHbm::Manage_c::HBM_MANAGE_ACTIVE) {
        // If we are in HBM, set some sort of cooldown
        mOutOfHbmStableTimer = 110;
        field_0x4F = true;
    }

    if (mPad::getCore(chan)->isConnected()) {
        if (ex_c::checkWPADProbeStable() &&
            dHbm::Manage_c::GetInstance()->getState() != dHbm::Manage_c::HBM_MANAGE_ACTIVE) {
            // Enable Mpls after we leave HBM
            gotoStateEnableMpls(chan);
        }
    }

    if (!mPad::getCore(chan)->isConnected()) {
        gotoStateWaitForConnect(chan);
    }
}

void ex_c::gotoStateEnableMpls(s32 chan) {
    mState = EX_STATE_ENABLE_MPLS;
    setMpls(true, chan);
}

void ex_c::executeStateEnableMpls(s32 chan) {
    if (fn_80059350(chan) || fn_80059370(chan)) {
        gotoStateMplsConfigError(chan);
    } else if (fn_80059390(chan)) {
        gotoStateMplsConfigured(chan);
    }
}

void ex_c::gotoStateDisconnectWpad(s32 chan) {
    mState = EX_STATE_DISCONECT_WPAD;
    WPADDisconnect(chan);
}

void ex_c::executeStateDisconnectWpad(s32 chan) {
    gotoStatePostConnect(chan);
}

void ex_c::gotoStateMplsConfigError(s32 chan) {
    mState = EX_STATE_MPLS_CONFIG_ERROR;
    setMpls(false, chan);
    field_0x2288 = 0;
}

void ex_c::executeStateMplsConfigError(s32 chan) {
    if (fn_80059390(chan)) {
        if (dHbm::Manage_c::GetInstance()->getState() == dHbm::Manage_c::HBM_MANAGE_ACTIVE) {
            gotoStatePostConnect(chan);
        } else {
            gotoStateEnableMpls(chan);
        }
    } else {
        if (dHbm::Manage_c::GetInstance()->getState() != dHbm::Manage_c::HBM_MANAGE_ACTIVE) {
            if (field_0x2288 > 120) {
                gotoStateDisconnectWpad(chan);
            } else {
                field_0x2288++;
            }
        }
    }
}

void ex_c::gotoStateMplsConfigured(s32 chan) {
    mState = EX_STATE_MPLS_CONFIGURED;
}

void ex_c::executeStateMplsConfigured(s32 chan) {
    if (!mPad::getCore(chan)->isConnected()) {
        setMpls(false, chan);
        gotoStateWaitForConnect(chan);
    } else {
        if (fn_80059330(chan) || fn_80059350(chan) || fn_80059370(chan)) {
            setMpls(false, chan);
        }
    }
    if (!mMplsEnabled) {
        gotoStatePostConnect(chan);
    }

    if (dHbm::Manage_c::GetInstance()->getState() == dHbm::Manage_c::HBM_MANAGE_ACTIVE) {
        field_0x4F = false;
    }
}

void ex_c::fn_800572A0(s32 chan) {
    switch (mState) {
        case EX_STATE_WAITING_FOR_CONNECT: executeStateWaitForConnect(chan); break;
        case EX_STATE_POST_CONNECT:        executeStatePostConnect(chan); break;
        case EX_STATE_ENABLE_MPLS:         executeStateEnableMpls(chan); break;
        case EX_STATE_DISCONECT_WPAD:      executeStateDisconnectWpad(chan); break;
        case EX_STATE_MPLS_CONFIG_ERROR:   executeStateMplsConfigError(chan); break;
        case EX_STATE_MPLS_CONFIGURED:     executeStateMplsConfigured(chan); break;
    }

    if (mState != EX_STATE_POST_CONNECT &&
        dHbm::Manage_c::GetInstance()->getState() != dHbm::Manage_c::HBM_MANAGE_ACTIVE) {
        if (mOutOfHbmStableTimer > 0) {
            mOutOfHbmStableTimer--;
        } else {
            mOutOfHbmStableTimer = 0;
        }
    }
}

void ex_c::acc_c::init() {
    for (int i = 0; i < 120; ++i) {
        mAccVecs[i].set(0.f, 0.f, 0.f);
        field_0x5A0[i].set(0.f, 0.f, 0.f);
        mAccValues[i] = 0.f;
    }
    for (int i = 0; i < 39; ++i) {
        field_0xD20[i].set(0.f, 0.f, 0.f);
    }
    for (int i = 0; i < 33; ++i) {
        field_0xEF4[i].set(0.f, 0.f, 0.f);
    }
    field_0x1080.set(0.f, 0.f, 0.f);
    field_0x108C.set(0.f, 0.f, 0.f);

    MTXIdentity(field_0x1098);
    field_0x10C8 = 0;
    field_0x10CC = 0;
    field_0x10D0 = 0;
    field_0x10D4 = 0;
    field_0x10D8 = 31;
}

f32 ex_c::acc_c::getMaxAccValue(s32 chan) {
    s32 readLen = mPad::getCore(chan)->getReadLength();
    if (readLen == 0) {
        return 0.0f;
    }

    f32 max = 0.0f;
    for (int i = 0; i < readLen; i++) {
        if (max < mAccValues[i]) {
            max = mAccValues[i];
        }
    }
    return max;
}

void ex_c::acc_c::fn_800576D0(s32 chan) {
    // unused
    (void)nw4r::math::FSqrt(mAccVecs[0].squaredLength());

    f32 maxAccValue = getMaxAccValue(chan);
    if (maxAccValue <= 1.1f && mAccVecs[0].y < 0.35f) {
        field_0x10D0++;
        if (field_0x10D0 >= 6) {
            field_0x10D0 = 6;
            field_0x108C = mAccVecs[0];
            mVec3_c dir(0.0f, -1.0f, 0.0f);
            f32 len = mAccVecs[0].squaredLength();
            if (!cM3d_IsZero(len)) {
                // TODO - bad part start
                mVec3_c acc = mAccVecs[0];
                acc.normalize();
                mVec3_c cross = acc.cross(dir);
                f32 f = acc.dot(dir);
                if (!cM3d_IsZero(cross.squaredLength())) {
                    cross.normalize();
                    f32 acos = std::acosf(f);
                    MTXRotAxisRad(field_0x1098, cross, acos);
                    // TODO - bad part end
                } else {
                    MTXIdentity(field_0x1098);
                }
            } else {
                MTXIdentity(field_0x1098);
            }
        }
    } else {
        field_0x10D0 = 0;
    }
}

void ex_c::acc_c::fn_800578E0(s32 chan) {
    const s32 readLen = mPad::getCore(chan)->getReadLength();

    // Shift old values back
    for (int i = readLen; i < 120; i++) {
        field_0x5A0[i] = field_0x5A0[i - readLen];
    }

    // TODO - loop strength reduction too strong
    f32 max = -8.0f;
    bool b = false;
    for (int i = 0; i < readLen; i++) {
        if (max < mAccVecs[i].y) {
            max = mAccVecs[i].y;
            b = true;
        }
        MTXMultVec(field_0x1098, mAccVecs[i], field_0x5A0[i]);
        field_0x5A0[i].y += 1.0f;
    }

    if (b) {
        field_0x10D4 = max;
    }
}

void ex_c::acc_c::fn_80057AC0(s32 chan, bool b) {
    const s32 readLen = mPad::getCore(chan)->getReadLength();

    // Shift field_0xD20 back by one
    for (int i = 38; i > 0; i--) {
        field_0xD20[i] = field_0xD20[i - 1];
    }

    for (int i = 0; i < readLen; i++) {
        field_0x1080.x += mAccVecs[i].x;
        field_0x1080.y += mAccVecs[i].y;
        field_0x1080.z += mAccVecs[i].z;
    }

    if (b) {
        field_0x1080 *= 0.96f;
    } else {
        field_0x1080 *= 0.98f;
    }

    // TODO - problematic part start

    field_0xD20[0] = field_0x1080;

    for (int i = 3; i < 36;) {
        int k = i++ - 3;
        f32 x = 0.0f;
        f32 y = 0.0f;
        f32 z = 0.0f;
        for (int j = 0; j < 7; j++) {
            x += field_0xD20[k + j].x / 7.0f;
            y += field_0xD20[k + j].y / 7.0f;
            z += field_0xD20[k + j].z / 7.0f;
        }
        field_0xEF4[k].set(x, y, z);
    }

    // TODO - problematic part end
}

f32 ex_c::acc_c::fn_80057F00(s32 chan) {
    s32 offs = chan + 3;
    return field_0xD20[offs].x - field_0xEF4[chan].x;
}

f32 ex_c::acc_c::fn_80057F30(s32 chan) {
    s32 offs = chan + 3;
    return field_0xD20[offs].y - field_0xEF4[chan].y;
}

f32 ex_c::acc_c::fn_80057F60(s32 chan) {
    s32 offs = chan + 3;
    return field_0xD20[offs].z - field_0xEF4[chan].z;
}

bool ex_c::acc_c::fn_80057F90(s32 idx, bool b) {
    if ((b && ((fn_80057F00(idx) < -13.5f) || fn_80057F00(idx + 1) < -13.5f) &&
         std::fabsf(fn_80057F00(idx + 1) - fn_80057F00(idx)) > 3.75f) ||
        (!b && ((fn_80057F00(idx) < -5.0f) || fn_80057F00(idx + 1) < -5.0f) &&
         std::fabsf(fn_80057F00(idx + 1) - fn_80057F00(idx)) > 2.75f)) {
        return true;
    }
    return false;
}
bool ex_c::acc_c::fn_800580C0(s32 idx, bool b) {
    if ((b && ((fn_80057F00(idx) > 13.5f) || fn_80057F00(idx + 1) > 13.5f) &&
         std::fabsf(fn_80057F00(idx + 1) - fn_80057F00(idx)) > 3.75f) ||
        (!b && ((fn_80057F00(idx) > 5.0f) || fn_80057F00(idx + 1) > 5.0f) &&
         std::fabsf(fn_80057F00(idx + 1) - fn_80057F00(idx)) > 2.75f)) {
        return true;
    }
    return false;
}

bool ex_c::acc_c::fn_800581F0(s32 idx, bool b) {
    if ((b && ((fn_80057F30(idx) < -13.5f) || fn_80057F30(idx + 1) < -13.5f) &&
         std::fabsf(fn_80057F30(idx + 1) - fn_80057F30(idx)) > 3.75f) ||
        (!b && ((fn_80057F30(idx) < -5.0f) || fn_80057F30(idx + 1) < -5.0f) &&
         std::fabsf(fn_80057F30(idx + 1) - fn_80057F30(idx)) > 2.75f)) {
        return true;
    }
    return false;
}
bool ex_c::acc_c::fn_80058320(s32 idx, bool b) {
    if ((b && ((fn_80057F30(idx) > 13.5f) || fn_80057F30(idx + 1) > 13.5f) &&
         std::fabsf(fn_80057F30(idx + 1) - fn_80057F30(idx)) > 3.75f) ||
        (!b && ((fn_80057F30(idx) > 5.0f) || fn_80057F30(idx + 1) > 5.0f) &&
         std::fabsf(fn_80057F30(idx + 1) - fn_80057F30(idx)) > 2.75f)) {
        return true;
    }
    return false;
}

bool ex_c::acc_c::fn_80058450(s32 idx, bool b) {
    // Note - same behavior, whether b is set or not
    if ((b && (fn_80057F60(idx) > 2.0f) && (fn_80057F60(idx + 1) - fn_80057F60(idx)) < -2.0f) ||
        (!b && (fn_80057F60(idx) > 2.0f) && (fn_80057F60(idx + 1) - fn_80057F60(idx)) < -2.0f)) {
        return true;
    }
    return false;
}

void ex_c::acc_c::fn_80058540(s32 chan, bool b) {
    bool b1 = false;
    bool b2 = false;
    bool b3 = false;
    bool b4 = false;
    // TODO condition
    if (b && (mPad::isMpls(chan) || mPad::isMplsPtFS(chan))) {
        if (m_ex[chan].mMPLSVelocity.x > 2.0f) {
            b4 = true;
        } else if (m_ex[chan].mMPLSVelocity.x < -2.0f) {
            b3 = true;
        } else if (m_ex[chan].mMPLSVelocity.y > 2.0f) {
            b1 = true;
        } else if (m_ex[chan].mMPLSVelocity.y < -2.0f) {
            b2 = true;
        }
    } else {
        int i1 = 0;
        int i2 = 0;
        int i3 = 0;
        field_0x10D8++;
        if (field_0x10D8 > 31) {
            field_0x10D8 = 31;
        }

        bool b1_ = false;
        bool b2_ = false;
        bool b3_ = false;
        bool b4_ = false;

        int start = field_0x10D8;
        for (int i = field_0x10D8; i >= 0; i--) {
            if (fn_80057F90(i, b)) {
                b1_ = true;
                b2_ = false;
            }

            if (fn_800580C0(i, b)) {
                b2_ = true;
                b1_ = false;
            }

            if (fn_80057F90(i, b) || fn_800580C0(i, b)) {
                i1 = 2;
            } else {
                i1--;
                if (i1 < 0) {
                    i1 = 0;
                    b1_ = false;
                    b2_ = false;
                }
            }

            if (fn_800581F0(i, b)) {
                b3_ = true;
                b4_ = false;
            }

            if (fn_80058320(i, b)) {
                b4_ = true;
                b3_ = false;
            }

            if (fn_800581F0(i, b) || fn_80058320(i, b)) {
                i2 = 2;
            } else {
                i2--;
                if (i2 < 0) {
                    i2 = 0;
                    b3_ = false;
                    b4_ = false;
                }
            }

            if (fn_80058450(i, b)) {
                i3 = 2;
            } else {
                i3--;
                if (i3 < 0) {
                    i3 = 0;
                }
            }
        }

        if (start != 0 && i3 > 0) {
            if (b1_) {
                field_0x10D8 = 0;
                b1 = true;
            }
            if (b2_) {
                field_0x10D8 = 0;
                b2 = true;
            }
            if (b3_) {
                field_0x10D8 = 0;
                b3 = true;
            }
            if (b4_) {
                field_0x10D8 = 0;
                b4 = true;
            }
        }
    }

    fn_80058990(0x1, b1);
    fn_80058990(0x2, b2);
    fn_80058990(0x4, b3);
    fn_80058990(0x8, b4);
    fn_80058990(0x10, b1 || b2 || b3 || b4);

    bool b6;
    if (b) {
        b6 = getMaxAccValue(chan) > (field_0x10CC ? 2.0f : 3.0f);
    } else {
        b6 = getMaxAccValue(chan) > (field_0x10CC ? 1.5f : 2.0f);
    }
    fn_80058990(0x20, b6);
}

void ex_c::acc_c::fn_80058990(u32 mask, bool b) {
    if (b) {
        if ((field_0x10CC & mask) != 0) {
            field_0x10C8 &= ~mask;
        } else {
            field_0x10C8 |= mask;
        }
        field_0x10CC |= mask;
    } else {
        field_0x10CC &= ~mask;
        field_0x10C8 &= ~mask;
    }
}

bool ex_c::acc_c::fn_800589F0() {
    return true;
}

f32 ex_c::acc_c::fn_80058A00() {
    cM3dGAab aab;
    aab.ClearForMinMax();
    for (int i = 0; i < 120; i++) {
        aab.SetMinMax(mAccVecs[i]);
    }

    f32 lenSq = nw4r::math::VEC3LenSq(aab.GetMax() - aab.GetMin()) - 0.017f;
    if (lenSq < 0.0f) {
        lenSq = 0.0f;
    }
    return lenSq;
}

void ex_c::acc_c::fn_80058AE0(s32 chan, bool b) {
    fn_800576D0(chan);
    fn_800578E0(chan);
    fn_80057AC0(chan, b);
    fn_80058540(chan, b);
}

mMtx_c ex_c::mpls_c::getMtx() const {
    mMtx_c m;
    m.setBase(0, mX);
    m.setBase(1, mY);
    m.setBase(2, mZ);
    m.setBase(3, mVec3_c::Zero);
    return m;
}

bool ex_c::isMissingMpls() {
    if (m_current_ex->mConnectedStableTimer != 0) {
        return false;
    }

    if (m_current_ex->mOutOfHbmStableTimer != 0) {
        return false;
    }

    return !isDeviceTypeMpls(m_current_ex->mWPADDeviceTypeStable);
}

bool ex_c::isMissingNunchuk() {
    if (m_current_ex->mConnectedStableTimer != 0) {
        return false;
    }

    if (m_current_ex->mOutOfHbmStableTimer != 0) {
        return false;
    }

    if (m_current_ex->mState != EX_STATE_MPLS_CONFIGURED) {
        return false;
    }

    if (m_current_ex->mWPADDeviceTypeStable == WPAD_DEV_FREESTYLE) {
        return false;
    }

    if (m_current_ex->mWPADDeviceTypeStable == WPAD_DEV_MPLS_PT_FS) {
        return false;
    }

    return true;
}

void ex_c::fn_80058C90(s32 chan) {
    m_ex[chan].field_0x46 = true;
    m_ex[chan].field_0x3C = 1;
}

bool ex_c::isLowBattery() {
    return getBatteryLevel() == 1;
}
bool ex_c::isOutOfBattery() {
    return getBatteryLevel() == 0;
}

void ex_c::resetInfo() {
    for (int i = 0; i < 4; ++i) {
        clearInfo(i);
    }
}
void ex_c::clearInfo(s32 chan) {
    m_connected[chan] = false;
    m_info[0][chan].dpd = FALSE;
    m_info[0][chan].speaker = FALSE;
    m_info[0][chan].attach = FALSE;
    m_info[0][chan].lowBat = FALSE;
    m_info[0][chan].nearempty = FALSE;
    m_info[0][chan].battery = 0;
    m_info[0][chan].led = 0;
    m_info[0][chan].protocol = 0;
    m_info[0][chan].firmware = 0;
}
extern "C" void async_info_callback(WPADChannel chan, WPADResult result) {
    switch (result) {
        case WPAD_ERR_OK: {
            ex_c::setInfo(chan, &ex_c::m_info[0][chan]);
            break;
        }
        case WPAD_ERR_NO_CONTROLLER: {
            ex_c::clearInfo(chan);
            break;
        }
        case WPAD_ERR_BUSY: {
            ex_c::m_ex[chan].field_0x46 = true;
            ex_c::m_ex[chan].field_0x3C = 120;
            break;
        }
        case WPAD_ERR_TRANSFER: {
            ex_c::m_ex[chan].field_0x46 = true;
            ex_c::m_ex[chan].field_0x3C = 120;
            break;
        }
    }
}
s32 ex_c::getInfo(s32 chan) {
    WPADResult res = WPADGetInfoAsync(chan, &ex_c::m_info[0][chan], async_info_callback);
    if (res == WPAD_ERR_NO_CONTROLLER) {
        clearInfo(chan);
    }
    return res;
}

s32 ex_c::getBatteryLevel() {
    return getBatteryLevel(mPad::g_currentCoreId);
}
s32 ex_c::getBatteryLevel(s32 chan) {
    if (m_connected[chan]) {
        return m_info[0][chan].battery;
    }
    return -1;
}

void ex_c::setInfo(s32 chan, const WPADInfo *pInfo) {
    m_info[0][chan] = *pInfo;
    m_connected[chan] = true;
}

f32 ex_c::fn_80058F50() {
    return m_current_ex->mMotion.fn_80058A00();
}

bool ex_c::startCurrentMplsCalibration() {
    if (m_current_ex->mIsWPADDeviceTypeMplsStableTimer < 8) {
        return true;
    }

    if (!m_current_ex->mMotion.fn_800589F0()) {
        return true;
    }

    m_current_ex->mNeedMplsCalibration = false;
    m_current_ex->startMplsCalibration(mPad::getCurrentCoreID());
    m_current_ex->fn_80058C90(mPad::getCurrentCoreID());
    initMpls(mPad::getCurrentCoreID());

    return false;
}

f32 ex_c::getCurrentCalibrationWork() {
    return m_current_ex->getCalibrationWork();
}

void ex_c::stopCurrentMplsCalibration() {
    m_current_ex->stopMplsCalibration(mPad::getCurrentCoreID());
}

void ex_c::centerCurrentCursor() {
    m_current_ex->centerCursor(mPad::getCurrentCoreID());
}

bool ex_c::needMplsCalibration() {
    if (!mPad::getCore()->isConnected()) {
        return false;
    }

    if (isMissingMpls()) {
        return false;
    }

    if (m_current_ex->mOutOfHbmStableTimer != 0) {
        return false;
    }

    if (isMissingNunchuk()) {
        return false;
    }

    if (m_current_ex->mIsWPADDeviceTypeMplsStableTimer >= 8) {
        return m_current_ex->mNeedMplsCalibration;
    }

    return false;
}

void ex_c::setCalibrateMpls() {
    m_current_ex->mNeedMplsCalibration = true;
}

bool ex_c::fn_800590B0() {
    bool ret = false;
    switch (m_current_ex->mState) {
        case EX_STATE_POST_CONNECT:
        case EX_STATE_ENABLE_MPLS:
        case EX_STATE_DISCONECT_WPAD:
        case EX_STATE_MPLS_CONFIG_ERROR:
            // TODO - one uneliminated dead branch in here
            ret = m_current_ex->mState == EX_STATE_MPLS_CONFIG_ERROR || m_current_ex->mState == EX_STATE_ENABLE_MPLS ||
                  m_current_ex->mState == EX_STATE_POST_CONNECT;
    }
    return ret;
}

bool ex_c::fn_800590E0() {
    return m_current_ex->field_0x4F;
}

void ex_c::fn_800590F0() {
    m_current_ex->field_0x4F = false;
}

bool ex_c::fn_80059100() {
    return fn_80059110(mPad::g_currentCoreId);
}

bool ex_c::fn_80059110(s32 chan) {
    return nw4r::math::VEC3LenSq(m_ex[chan].mMPLSVelocity) >= 0.01f || m_ex[chan].mFSStickDistance >= 0.0001f ||
           mPad::g_core[chan]->getCoreStatus()->getHold() || mPad::g_core[chan]->getCoreStatus()->getTrigger() ||
           m_ex[chan].mMotion.field_0x10C8 != 0;
}

void ex_c::fn_80059210() {
    m_current_ex->field_0x22CE = 1;
}

void ex_c::fn_80059220() {
    m_current_ex->field_0x22CF = 1;
}

void ex_c::fn_80059230() {
    m_current_ex->field_0x22D0 = 1;
}

void ex_c::fn_80059240() {
    m_current_ex->field_0x22D0 = 0;
}

void ex_c::on_0x54(s32 idx) {
    m_ex[idx].field_0x54 = true;
}
void ex_c::on_0x55(s32 idx) {
    m_ex[idx].field_0x55 = true;
}
void ex_c::on_0x56(s32 idx) {
    m_ex[idx].field_0x56 = true;
}
void ex_c::on_0x57(s32 idx) {
    m_ex[idx].field_0x57 = true;
}

void ex_c::fn_800592D0(s32 chan) {
    m_ex[chan].field_0x54 = false;
    m_ex[chan].field_0x55 = false;
    m_ex[chan].field_0x56 = false;
    m_ex[chan].field_0x57 = false;
}

void ex_c::fn_80059300(s32 chan) {
    field_0x58 = field_0x54;
    field_0x59 = field_0x55;
    field_0x5A = field_0x56;
    field_0x5B = field_0x57;
    fn_800592D0(chan);
}

bool ex_c::fn_80059330(s32 chan) {
    return m_ex[chan].field_0x58;
}

bool ex_c::fn_80059350(s32 chan) {
    return m_ex[chan].field_0x59;
}

bool ex_c::fn_80059370(s32 chan) {
    return m_ex[chan].field_0x5A;
}

bool ex_c::fn_80059390(s32 chan) {
    return m_ex[chan].field_0x5B;
}

void ex_c::getUnifiedWpadStatus(s32 chan) {
    KPADGetUnifiedWpadStatus(chan, &mStatus, 1);
}

void ex_c::calcFSStickDirMask() {
    mFSStickMaskChanged = false;
    if (mFSStickDistance < 0.8f) {
        mFSStickMask = 0;
        return;
    }
    u32 prevFSStickMask = mFSStickMask;
    s32 ang = mFSStickAngle;
    ang = ((ang + 0x11000) / 0x2000) % 8;
    mFSStickMask = 1 << ang;
    if (mFSStickMask != 0 && mFSStickMask != prevFSStickMask) {
        mFSStickMaskChanged = true;
    }
}

void ex_c::setNoSleep() {
    WPADSetAutoSleepTime(0);
}

void ex_c::setAutoSleepTime() {
    WPADSetAutoSleepTime(5);
    WPADResetAutoSleepTimeCount(0);
}

} // namespace dPad
