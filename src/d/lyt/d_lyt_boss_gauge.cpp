#include "d/lyt/d_lyt_boss_gauge.h"

#include "common.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "sized_string.h"

STATE_DEFINE(dLytBossGauge_c, None);
STATE_DEFINE(dLytBossGauge_c, In);
STATE_DEFINE(dLytBossGauge_c, Move);
STATE_DEFINE(dLytBossGauge_c, Out);

#define BOSS_GAUGE_ANIM_IN 0
#define BOSS_GAUGE_ANIM_OUT 1
#define BOSS_GAUGE_ANIM_DANGERLOOP 2
#define BOSS_GAUGE_ANIM_MOVE 3

dLytBossGauge_c *dLytBossGauge_c::sInstance;

void dLytBossGauge_c::initializeState_None() {
    return;
}

void dLytBossGauge_c::executeState_None() {
    if (field_0x544) {
        mAnmGroups[BOSS_GAUGE_ANIM_IN].setAnimEnable(true);
        mAnmGroups[BOSS_GAUGE_ANIM_IN].setFrame(0.0f);

        SizedString<64> name;
        name[0] = 0;

        for (int i = 0; i < 3; i++) {
            name.sprintf("N_bigBoss_%02d", i);

            nw4r::lyt::Pane *pane = mLyt.findPane(name);

            if (i == field_0x548) {
                pane->SetVisible(true);
            } else {
                pane->SetVisible(false);
            }
        }
        mAnmGroups[BOSS_GAUGE_ANIM_DANGERLOOP].setAnimEnable(true);
        mAnmGroups[BOSS_GAUGE_ANIM_DANGERLOOP].setFrame(0.0f);

        mLyt.calc();

        mAnmGroups[BOSS_GAUGE_ANIM_DANGERLOOP].setAnimEnable(false);
        mStateMgr.changeState(dLytBossGauge_c::StateID_In);
        field_0x546 = 0;
        field_0x544 = 0;
    }
}

void dLytBossGauge_c::finalizeState_None() {
    return;
}
void dLytBossGauge_c::initializeState_In() {
    return;
}

void dLytBossGauge_c::executeState_In() {
    if (mAnmGroups[BOSS_GAUGE_ANIM_IN].isEndReached()) {
        mAnmGroups[BOSS_GAUGE_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(dLytBossGauge_c::StateID_Move);
    }
    return;
}

void dLytBossGauge_c::finalizeState_In() {
    return;
}

void dLytBossGauge_c::initializeState_Move() {
    return;
}

void dLytBossGauge_c::executeState_Move() {
    if (field_0x545) {
        mAnmGroups[BOSS_GAUGE_ANIM_OUT].setAnimEnable(true);
        mAnmGroups[BOSS_GAUGE_ANIM_OUT].setFrame(0.0f);
        mStateMgr.changeState(dLytBossGauge_c::StateID_Out);
        field_0x545 = false;
    }
    return;
}

void dLytBossGauge_c::finalizeState_Move() {
    return;
}
void dLytBossGauge_c::initializeState_Out() {
    return;
}

void dLytBossGauge_c::executeState_Out() {
    if (mAnmGroups[BOSS_GAUGE_ANIM_OUT].isEndReached()) {
        mAnmGroups[BOSS_GAUGE_ANIM_OUT].setAnimEnable(false);
        field_0x546 = true;
        mStateMgr.changeState(dLytBossGauge_c::StateID_None);
    }
}

void dLytBossGauge_c::finalizeState_Out() {
    return;
}

static const d2d::LytBrlanMapping brlanMap[] = {
    {        "bossGauge_00_in.brlan",  "G_inOut_00"},
    {       "bossGauge_00_out.brlan",  "G_inOut_00"},
    {"bossGauge_00_dangerLoop.brlan", "G_danger_00"},
    {      "bossGauge_00_move.brlan",   "G_move_00"},
};

bool dLytBossGauge_c::build() {
    sInstance = this;
    field_0x54c = 0.8f;

    void *data = LayoutArcManager::GetInstance()->getLoadedData("BossGauge");

    mResAcc.attach(data, "");
    mLyt.setResAcc(&mResAcc);
    mLyt.build("bossGauge_00.brlyt", nullptr);

    for (int i = 0; i < 4; i++) {
        mAnmGroups[i].init(brlanMap[i].mFile, &mResAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnmGroups[i].bind(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    mAnmGroups[BOSS_GAUGE_ANIM_IN].setAnimEnable(true);
    mAnmGroups[BOSS_GAUGE_ANIM_IN].setFrame(0.0f);

    mAnmGroups[BOSS_GAUGE_ANIM_MOVE].setAnimEnable(true);
    mAnmGroups[BOSS_GAUGE_ANIM_MOVE].setFrame(0.0f);

    mLyt.calc();

    mAnmGroups[BOSS_GAUGE_ANIM_IN].setAnimEnable(false);
    mAnmGroups[BOSS_GAUGE_ANIM_MOVE].setAnimEnable(false);

    mStateMgr.changeState(dLytBossGauge_c::StateID_None);

    field_0x544 = 0;
    field_0x545 = 0;
    field_0x546 = 0;
    field_0x548 = 0;

    return true;
}

bool dLytBossGauge_c::execute() {
    mStateMgr.executeState();

    if (*mStateMgr.getStateID() != dLytBossGauge_c::StateID_None) {
        for (int i = 0; i < 3; i++) {
            if (mAnmGroups[i].isEnabled()) {
                if (i < 2 && mAnmGroups[i].isStop()) {
                    mAnmGroups[i].setAnimEnable(false);
                }
                mAnmGroups[i].play();
            }
        }
        mLyt.calc();
    }
    return true;
}

bool dLytBossGauge_c::draw() {
    if (*mStateMgr.getStateID() != dLytBossGauge_c::StateID_None) {
        mLyt.addToDrawList();
    }
    return true;
}

bool dLytBossGauge_c::remove() {
    mLyt.unbindAnims();

    for (int i = 0; i < 4; i++) {
        mAnmGroups[i].remove();
    }

    return true;
}

void dLytBossGauge_c::setMode(s32 mode) {
    field_0x548 = mode;
    field_0x544 = 1;
    return;
}

void dLytBossGauge_c::fn_80158940(f32 f) {
    int f2 = mAnmGroups[BOSS_GAUGE_ANIM_MOVE].getAnimDuration() * f + 0.5f;

    mAnmGroups[BOSS_GAUGE_ANIM_MOVE].setAnimEnable(true);
    mAnmGroups[BOSS_GAUGE_ANIM_MOVE].setFrame(f2);
    mLyt.calc();
    mAnmGroups[BOSS_GAUGE_ANIM_MOVE].setAnimEnable(false);

    if (f >= field_0x54c) {
        if (!mAnmGroups[BOSS_GAUGE_ANIM_DANGERLOOP].isEnabled()) {
            mAnmGroups[BOSS_GAUGE_ANIM_DANGERLOOP].setAnimEnable(true);
            mAnmGroups[BOSS_GAUGE_ANIM_DANGERLOOP].setFrame(0.0f);
        }
    } else {
        mAnmGroups[BOSS_GAUGE_ANIM_DANGERLOOP].setAnimEnable(false);
    }

    return;
}
