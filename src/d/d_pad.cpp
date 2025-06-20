#include "d/d_pad.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_cs_game.h"
#include "d/d_gfx.h"
#include "d/d_pause.h"
#include "d/d_reset.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_player_mgr.h"
#include "egg/core/eggController.h"
#include "egg/core/eggHeap.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_pad.h"
#include "m/m_vec.h"
#include "rvl/KPAD/KPAD.h"
#include "rvl/MTX/vec.h"
#include "rvl/WPAD/WPAD.h"

namespace dPad {

ex_c ex_c::m_ex[4];
ex_c *ex_c::m_current_ex = ex_c::m_ex;

WPADInfo ex_c::m_info[2][4];

bool ex_c::m_connected[4];

void control_mpls_callback(s32 idx, s32 code) {
    switch (code) {
        case WPAD_ERR_OK: {
            ex_c::on_0x54(idx);
            break;
        }
        case WPAD_ERR_NO_CONTROLLER: {
            ex_c::on_0x55(idx);
            break;
        }
        case WPAD_ERR_BUSY: {
            ex_c::on_0x56(idx);
            break;
        }
        case 1: {
            ex_c::on_0x57(idx);
            break;
        }
    }
}

bool checkDeviceType(u32 type) {
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
        ex_c::m_ex[i].resetState(i);
    }
    fn_80194080();
}

void setMpls(bool enable, s32 chan) {
    if (enable) {
        if (ex_c::m_ex[chan].mMplsEnabled == false) {
            ex_c::m_ex[chan].mMplsEnabled = true;
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

void centerPos(mVec2_c &in, mVec2_c &out) {
    out.x = dGfx_c::getWidth4x3F() * 0.5f * (1.f + in.x) + dGfx_c::getWidth4x3LeftF();
    out.y = dGfx_c::getCurrentScreenHeightF() * -0.5f * (1.f + in.y) + dGfx_c::getCurrentScreenTopF();
}

extern "C" void fn_801940C0();
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
        ex.fn_80056DF0(0);
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

            VECCrossProduct(ex_c::m_ex[0].mMPLSBasisY, baseZ, baseX);
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

        if ((dCsGame_c::GetInstance() && dCsGame_c::GetInstance()->fn_801BF5E0() && !ex.field_0x22D0) ||
            ((dAcPy_c::GetLink() &&
              dAcPy_c::GetLink()->checkActionFlagsCont(0x400 | 0x100 | 0x80 | 0x40 | 0x10 | 0x4 | 0x2 | 0x1) &&
              !dAcPy_c::GetLink()->vt_0x1C0() && !dLytMeter_c::GetMain()->getField_0x1377F()) ||
             ex.field_0x22CF)) {
            if (dLytControlGame_c::getInstance() && dLytControlGame_c::getInstance()->isStateNormal()) {
                if (!(dPauseManager_c::GetInstance() && dPauseManager_c::GetInstance()->getField_0x25())) {
                    KPADDisableMplsDpdRevise(0);
                    KPADDisableMplsAccRevise(0);
                }
            }
            KPADDisableMplsDirRevise(0);
        }
        if (ex.field_0x47 || ex.field_0x48 != 0) {
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

        ex.fn_800593D0();

        bool isMpls = mPad::isMpls(0) || mPad::isMplsPtFS(0);
        if (isMpls) {
            ex.mMPLSSpeed.copyFrom(&mPad::getCore(0)->getCoreStatus()->mpls_rot);
            ex.mMPLSBasisX.copyFrom(&mPad::getCore(0)->getCoreStatus()->mpls_basis_x);
            ex.mMPLSBasisY.copyFrom(&mPad::getCore(0)->getCoreStatus()->mpls_basis_y);
            ex.mMPLSBasisZ.copyFrom(&mPad::getCore(0)->getCoreStatus()->mpls_basis_z);
        } else {
            ex.mMPLSSpeed.set(0.f, 0.f, 0.f);
            ex.mMPLSBasisX = mVec3_c::Ex;
            ex.mMPLSBasisY = mVec3_c::Ey;
            ex.mMPLSBasisZ = mVec3_c::Ez;
        }

        ex.fn_80056790(0);

        if (ex.field_0x50) {
            ex.mDpdPos = ex.field_0x8;
        } else {
            if (mPad::getCore(0)->getDpdValidFlag() > 0) {
                ex.mDpdPos = mPad::getCore(0)->getDpdRawPos();
                // ex.mDpdPos.set(v.x, v.y);
            } else {
                ex.mDpdPos.set(-2.f, -2.f);
            }
        }
        centerPos(ex.mDpdPos, ex.field_0x8);
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
    fn_801940C0();
}

void endPad_BR() {
    mPad::endPad();
}

void connectCallback(const EGG::CoreControllerConnectArg &rArg) {
    switch (rArg.result) {
        case WPAD_ERR_OK: {
            dSndPlayerMgr_c::GetInstance()->setup(rArg.chan);
            ex_c::m_ex[rArg.chan].mSpeakerSetup = true;
            break;
        }
        case WPAD_ERR_NO_CONTROLLER: {
            dSndPlayerMgr_c::GetInstance()->shutdown(rArg.chan);
            ex_c::m_ex[rArg.chan].mSpeakerShutdown = true;
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

void ex_c::fn_80056AF0(s32 chan) {
    if (mSpeakerSetup) {
        mSpeakerSetup = false;
        field_0x38 = 90;
        field_0x44 = true;

        m_ex[chan].field_0x46 = true;
        m_ex[chan].field_0x3C = 120;
    } else {
        field_0x44 = false;
    }
    if (mSpeakerShutdown) {
        mSpeakerShutdown = false;
        field_0x45 = true;
        m_ex[chan].mWPADDeviceTypeStable = WPAD_DEV_UNKNOWN;
    } else {
        field_0x45 = false;
    }
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
    if (field_0x38 > 0) {
        field_0x38--;
        if (field_0x38 < 0) {
            field_0x38 = 0;
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

    if (!checkDeviceType(mWPADDeviceTypeStable)) {
        mIncorrectDeviceType = true;
        field_0x6C = 0;
    }
    if (checkDeviceType(mWPADDeviceTypeStable) && field_0x6C < 8) {
        field_0x6C++;
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

// . . .

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
    m_info[0][chan].dpd = pInfo->dpd;
    m_info[0][chan].speaker = pInfo->speaker;
    m_info[0][chan].attach = pInfo->attach;
    m_info[0][chan].lowBat = pInfo->lowBat;
    m_info[0][chan].nearempty = pInfo->nearempty;
    m_info[0][chan].battery = pInfo->battery;
    m_info[0][chan].led = pInfo->led;
    m_info[0][chan].protocol = pInfo->protocol;
    m_info[0][chan].firmware = pInfo->firmware;
    m_connected[chan] = true;
}
// . . .

void ex_c::on_0x54(s32 idx) {
    m_ex[idx].field_0x54[0] = true;
}
void ex_c::on_0x55(s32 idx) {
    m_ex[idx].field_0x54[1] = true;
}
void ex_c::on_0x56(s32 idx) {
    m_ex[idx].field_0x54[2] = true;
}
void ex_c::on_0x57(s32 idx) {
    m_ex[idx].field_0x54[3] = true;
}

void ex_c::fn_800592D0(s32 chan) {
    m_ex[chan].field_0x54[0] = 0;
    m_ex[chan].field_0x54[1] = 0;
    m_ex[chan].field_0x54[2] = 0;
    m_ex[chan].field_0x54[3] = 0;
}

void ex_c::fn_80059300(s32 chan) {
    field_0x58[0] = field_0x54[0];
    field_0x58[1] = field_0x54[1];
    field_0x58[2] = field_0x54[2];
    field_0x58[3] = field_0x54[3];
    fn_800592D0(chan);
}

void ex_c::getUnifiedWpadStatus(s32 chan) {
    KPADGetUnifiedWpadStatus(chan, &mStatus, 1);
}

void ex_c::fn_800593D0() {
    field_0x22D1 = 0;
    if (mFSStickDistance < 0.8f) {
        field_0x22D4 = 0;
        return;
    }
    u32 prev22D4 = field_0x22D4;
    s32 ang = mFSStickAngle;
    ang = ((ang + 0x11000) / 0x2000) % 8;
    field_0x22D4 = 1 << ang;
    if (field_0x22D4 != 0 && field_0x22D4 != prev22D4) {
        field_0x22D1 = 1;
    }
}

void setNoSleep() {
    WPADSetAutoSleepTime(0);
}

void setAutoSleepTime() {
    WPADSetAutoSleepTime(5);
    WPADSetControllerLastDataUpdateTime(0);
}

} // namespace dPad
