#include "d/lyt/d_lyt_sky_gauge.h"

#include "common.h"
#include "s/s_StateID.hpp"

STATE_DEFINE(dLytSkyGaugeMain_c, ModeNone);
STATE_DEFINE(dLytSkyGaugeMain_c, ModeIn);
STATE_DEFINE(dLytSkyGaugeMain_c, ModeMove);
STATE_DEFINE(dLytSkyGaugeMain_c, ModeOut);

STATE_DEFINE(dLytSkyGauge_c, None);
STATE_DEFINE(dLytSkyGauge_c, In);
STATE_DEFINE(dLytSkyGauge_c, Move);
STATE_DEFINE(dLytSkyGauge_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {  "skyGauge_00_in.brlan", "G_inOut_00"},
    { "skyGauge_00_out.brlan", "G_inOut_00"},
    {"skyGauge_00_move.brlan",  "G_move_00"},
};

#define LYT_SKY_GAUGE_MAIN_ANIM_IN 0
#define LYT_SKY_GAUGE_MAIN_ANIM_OUT 1
#define LYT_SKY_GAUGE_MAIN_ANIM_LOOP 2

dLytSkyGauge_c *dLytSkyGauge_c::sInstance;

void dLytSkyGaugeMain_c::initializeState_ModeNone() {}
void dLytSkyGaugeMain_c::executeState_ModeNone() {
    mWantsModeChange = true;
}
void dLytSkyGaugeMain_c::finalizeState_ModeNone() {}

void dLytSkyGaugeMain_c::initializeState_ModeIn() {}
void dLytSkyGaugeMain_c::executeState_ModeIn() {
    if (!mWantsModeChange) {
        if (mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].isEndReached()) {
            mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setAnimEnable(false);
            mWantsModeChange = true;
        }
    }
}
void dLytSkyGaugeMain_c::finalizeState_ModeIn() {}

void dLytSkyGaugeMain_c::initializeState_ModeMove() {}
void dLytSkyGaugeMain_c::executeState_ModeMove() {
    mWantsModeChange = true;
}
void dLytSkyGaugeMain_c::finalizeState_ModeMove() {}

void dLytSkyGaugeMain_c::initializeState_ModeOut() {}
void dLytSkyGaugeMain_c::executeState_ModeOut() {
    if (!mWantsModeChange) {
        if (mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_OUT].isEndReached()) {
            mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_OUT].setAnimEnable(false);
            mWantsModeChange = true;
        }
    }
}
void dLytSkyGaugeMain_c::finalizeState_ModeOut() {}

dLytSkyGaugeMain_c::dLytSkyGaugeMain_c() : mStateMgr(*this, sStateID::null) {}

bool dLytSkyGaugeMain_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("skyGauge_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < 3; i++) {
        mAnmGroups[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnmGroups[i].bind(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setAnimEnable(true);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setFrame(0.0f);
    mLyt.calc();
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setAnimEnable(false);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_LOOP].setAnimEnable(false);
    field_0x195 = false;
    return true;
}

bool dLytSkyGaugeMain_c::execute() {
    mStateMgr.executeState();

    for (int i = 0; i < 2; i++) {
        if (mAnmGroups[i].isEnabled()) {
            if (mAnmGroups[i].isStop()) {
                mAnmGroups[i].setAnimEnable(false);
                if (i == 0) {
                    field_0x195 = true;
                }
            }
            mAnmGroups[i].play();
        }
    }

    mLyt.calc();

    return true;
}

bool dLytSkyGaugeMain_c::remove() {
    mLyt.unbindAnims();
    for (int i = 0; i < 3; i++) {
        mAnmGroups[i].remove();
    }

    return true;
}

void dLytSkyGaugeMain_c::changeToNone() {
    mStateMgr.changeState(StateID_ModeNone);
    mWantsModeChange = false;
}

void dLytSkyGaugeMain_c::changeToIn() {
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setAnimEnable(true);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setFrame(0.0f);
    mLyt.calc();
    mStateMgr.changeState(StateID_ModeIn);
    mWantsModeChange = false;
}

void dLytSkyGaugeMain_c::changeToMove() {
    mStateMgr.changeState(StateID_ModeMove);
    mWantsModeChange = false;
}

void dLytSkyGaugeMain_c::changeToOut() {
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_OUT].setAnimEnable(true);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_OUT].setFrame(0.0f);
    mLyt.calc();
    mStateMgr.changeState(StateID_ModeOut);
    mWantsModeChange = false;
}

void dLytSkyGaugeMain_c::setHeight(f32 height) {
    int frame = height * 60.0f + 0.5f;
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_LOOP].setAnimEnable(true);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_LOOP].setFrame(frame);
}

void dLytSkyGauge_c::initializeState_None() {}
void dLytSkyGauge_c::executeState_None() {
    if (mWantsIn) {
        mMain.changeToIn();
        mStateMgr.changeState(StateID_In);
        mWantsIn = false;
    }
}
void dLytSkyGauge_c::finalizeState_None() {}

void dLytSkyGauge_c::initializeState_In() {}
void dLytSkyGauge_c::executeState_In() {
    if (mMain.wantsMove()) {
        mMain.changeToMove();
        mStateMgr.changeState(StateID_Move);
    }
}
void dLytSkyGauge_c::finalizeState_In() {}

void dLytSkyGauge_c::initializeState_Move() {}
void dLytSkyGauge_c::executeState_Move() {
    if (mWantsOut) {
        mMain.changeToOut();
        mStateMgr.changeState(StateID_Out);
        mWantsOut = false;
    }
}
void dLytSkyGauge_c::finalizeState_Move() {}

void dLytSkyGauge_c::initializeState_Out() {}
void dLytSkyGauge_c::executeState_Out() {
    if (mMain.wantsMove()) {
        mMain.changeToNone();
        mStateMgr.changeState(StateID_None);
    }
}
void dLytSkyGauge_c::finalizeState_Out() {}

bool dLytSkyGauge_c::build(d2d::ResAccIf_c *resAcc) {
    sInstance = this;
    mMain.build(resAcc);
    mWantsIn = false;
    mWantsOut = false;
    mStateMgr.changeState(StateID_None);
    return true;
}

bool dLytSkyGauge_c::execute() {
    mMain.execute();
    mStateMgr.executeState();
    return true;
}

bool dLytSkyGauge_c::remove() {
    mMain.remove();
    return true;
}

void dLytSkyGauge_c::setHeight(f32 height) {
    mMain.setHeight(height);
}
