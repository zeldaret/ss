#include "d/lyt/d_lyt_battery.h"

#include "d/d_d2d.h"
#include "egg/core/eggController.h"
#include "m/m_pad.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

STATE_DEFINE(dLytBattery_c, Off);
STATE_DEFINE(dLytBattery_c, In);
STATE_DEFINE(dLytBattery_c, On);
STATE_DEFINE(dLytBattery_c, None);
STATE_DEFINE(dLytBattery_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {  "denchi_00_in.brlan", "G_inOut_00"},
    {"denchi_00_none.brlan", "G_inOut_00"},
    {"denchi_00_loop.brlan",  "G_loop_00"},
    { "denchi_00_out.brlan", "G_inOut_00"},
};

dLytBattery_c *dLytBattery_c::sInstance;

#define LYT_BATTERY_ANIM_IN 0
#define LYT_BATTERY_ANIM_NONE 1
#define LYT_BATTERY_ANIM_LOOP 2
#define LYT_BATTERY_ANIM_OUT 3

bool dLytBattery_c::create() {
    new dLytBattery_c();
    if (sInstance == nullptr) {
        return false;
    }
    sInstance->init();
    return true;
}

bool dLytBattery_c::draw() {
    if (sInstance != nullptr) {
        sInstance->doDraw();
    }
    return true;
}

bool dLytBattery_c::init() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("System2D");
    mResAcc.attach(data, "");
    mLyt.setResAcc(&mResAcc);
    mLyt.build("denchi_00.brlyt", nullptr);

    for (int i = 0; i < 4; i++) {
        mAnmGroups[i].init(brlanMap[i].mFile, &mResAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnmGroups[i].bind(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    mAnmGroups[LYT_BATTERY_ANIM_LOOP].setAnimEnable(true);
    mStateMgr.changeState(StateID_Off);
    mLyt.calc();
    mDismissManually = false;
    return true;
}

bool dLytBattery_c::doDraw() {
    mStateMgr.executeState();
    if (*mStateMgr.getStateID() != StateID_Off) {
        for (int i = 0; i < 4; i++) {
            if (mAnmGroups[i].isEnabled()) {
                mAnmGroups[i].play();
            }
        }

        mLyt.calc();
        d2d::defaultSet();
        mLyt.draw();
    }

    return true;
}

bool dLytBattery_c::dismissManually() {
    if (*mStateMgr.getStateID() != StateID_On) {
        return false;
    }
    mDismissManually = true;
    return true;
}

extern "C" bool isLowBattery1();
extern "C" bool isLowBattery2();

bool dLytBattery_c::hasEnoughBatteryCharge() {
    if (!mPad::g_currentCore->mFlag.offBit(0) && (isLowBattery2() || isLowBattery1())) {
        return false;
    }
    return true;
}

void dLytBattery_c::initializeState_Off() {}
void dLytBattery_c::executeState_Off() {
    if (!hasEnoughBatteryCharge()) {
        mAnmGroups[LYT_BATTERY_ANIM_IN].setAnimEnable(true);
        mAnmGroups[LYT_BATTERY_ANIM_IN].setFrame(0.0f);
        mStateMgr.changeState(StateID_In);
    }
}
void dLytBattery_c::finalizeState_Off() {}

void dLytBattery_c::initializeState_In() {}
void dLytBattery_c::executeState_In() {
    if (mAnmGroups[LYT_BATTERY_ANIM_IN].isEndReached()) {
        mAnmGroups[LYT_BATTERY_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytBattery_c::finalizeState_In() {}

void dLytBattery_c::initializeState_On() {}
void dLytBattery_c::executeState_On() {
    if (mDismissManually == true) {
        mDismissManually = false;
        mStateMgr.changeState(StateID_None);
    } else {
        if (hasEnoughBatteryCharge()) {
            mAnmGroups[LYT_BATTERY_ANIM_OUT].setAnimEnable(true);
            mAnmGroups[LYT_BATTERY_ANIM_OUT].setFrame(0.0f);
            mStateMgr.changeState(StateID_Out);
        }
    }
}
void dLytBattery_c::finalizeState_On() {}

void dLytBattery_c::initializeState_None() {
    d2d::AnmGroup_c &grp = mAnmGroups[LYT_BATTERY_ANIM_NONE];
    grp.setAnimEnable(true);
    grp.setFrame(0.0f);
}
void dLytBattery_c::executeState_None() {
    d2d::AnmGroup_c &grp = mAnmGroups[LYT_BATTERY_ANIM_NONE];
    if (grp.isEndReached()) {
        grp.setAnimEnable(false);
    }

    if (hasEnoughBatteryCharge()) {
        mAnmGroups[LYT_BATTERY_ANIM_OUT].setAnimEnable(true);
        mAnmGroups[LYT_BATTERY_ANIM_OUT].setFrame(0.0f);
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytBattery_c::finalizeState_None() {}

void dLytBattery_c::initializeState_Out() {}
void dLytBattery_c::executeState_Out() {
    if (mAnmGroups[LYT_BATTERY_ANIM_OUT].isEndReached()) {
        mAnmGroups[LYT_BATTERY_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytBattery_c::finalizeState_Out() {}
