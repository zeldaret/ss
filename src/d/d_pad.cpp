#include "d/d_pad.h"

#include "common.h"
#include "d/d_gfx.h"
#include "d/snd/d_snd_player_mgr.h"
#include "egg/core/eggController.h"
#include "egg/core/eggHeap.h"
#include "m/m_pad.h"
#include "m/m_vec.h"
#include "rvl/KPAD/KPAD.h"
#include "rvl/WPAD/WPAD.h"

#include "rvl/WPAD.h"

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

void beginPad_BR() {
    mPad::beginPad();

    if (mPad::getCore(0)->isConnected()) {
        WPADSetAcceptConnection(0);
    } else {
        WPADSetAcceptConnection(1);
    }

    for (int i = 0; i < 4; ++i) {
        if (mPad::getCore(i)->isConnected()) {
            WPADDisconnect(i);
        }
    }

    ex_c::m_ex[0].fn_800572A0();
    if (mPad::getCore(0)->isConnected()) {
        ex_c::m_ex[0].getUnifiedWpadStatus(0);
        ex_c::m_ex[0].fn_80059300(0);
        ex_c::m_ex[0].fn_80056AF0(0);
        ex_c::m_ex[0].fn_80056B90(0);
        ex_c::m_ex[0].fn_80056CE0(0);
        ex_c::m_ex[0].fn_80056DF0(0);
    }

    ex_c::m_current_ex = &ex_c::m_ex[mPad::getCurrentCoreID()];
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
