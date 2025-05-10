#include "d/lyt/d_lyt_bird_gauge.h"

#include "common.h"
#include "toBeSorted/small_sound_mgr.h"

STATE_DEFINE(dLytBirdGaugeMain_c, ModeNone);
STATE_DEFINE(dLytBirdGaugeMain_c, ModeIn);
STATE_DEFINE(dLytBirdGaugeMain_c, ModeDash);
STATE_DEFINE(dLytBirdGaugeMain_c, ModeRecovery);
STATE_DEFINE(dLytBirdGaugeMain_c, ModeOut);

STATE_DEFINE(dLytBirdGauge_c, In);
STATE_DEFINE(dLytBirdGauge_c, None);
STATE_DEFINE(dLytBirdGauge_c, Dash);
STATE_DEFINE(dLytBirdGauge_c, Recovery);
STATE_DEFINE(dLytBirdGauge_c, Out);

dLytBirdGauge_c *dLytBirdGauge_c::sInstance;

static const d2d::LytBrlanMapping brlanMap[] = {
    {      "birdGauge_00_in.brlan", "G_inOut_00"},
    {    "birdGauge_00_dash.brlan",  "G_wing_00"},
    {    "birdGauge_00_dash.brlan",  "G_wing_01"},
    {    "birdGauge_00_dash.brlan",  "G_wing_02"},
    {"birdGauge_00_recovery.brlan",  "G_wing_00"},
    {"birdGauge_00_recovery.brlan",  "G_wing_01"},
    {"birdGauge_00_recovery.brlan",  "G_wing_02"},
    {     "birdGauge_00_out.brlan", "G_inOut_00"},
};

#define LYT_SKY_GAUGE_MAIN_ANIM_IN 0
#define LYT_SKY_GAUGE_MAIN_ANIM_DASH 1
#define LYT_SKY_GAUGE_MAIN_ANIM_RECOVERY 4
#define LYT_SKY_GAUGE_MAIN_ANIM_OUT 7

#define LYT_SKY_GAUGE_MAIN_NUM_WINGS 3
#define LYT_SKY_GAUGE_MAIN_NUM_ANIMS 8

void dLytBirdGaugeMain_c::initializeState_ModeNone() {}

void dLytBirdGaugeMain_c::executeState_ModeNone() {
    mIsAnimating = true;
}

void dLytBirdGaugeMain_c::finalizeState_ModeNone() {}

void dLytBirdGaugeMain_c::initializeState_ModeIn() {}

void dLytBirdGaugeMain_c::executeState_ModeIn() {
    if (mIsAnimating) {
        return;
    }
    if (mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].isEndReached()) {
        mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setAnimEnable(false);
        mIsAnimating = true;
    }
}

void dLytBirdGaugeMain_c::finalizeState_ModeIn() {}

void dLytBirdGaugeMain_c::initializeState_ModeDash() {}

void dLytBirdGaugeMain_c::executeState_ModeDash() {
    if (mIsAnimating) {
        return;
    }
    if (mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_DASH + mNumDisplayedDashes].isEndReached()) {
        mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_DASH + mNumDisplayedDashes].setAnimEnable(false);
        mIsAnimating = true;
    }
}

void dLytBirdGaugeMain_c::finalizeState_ModeDash() {}

void dLytBirdGaugeMain_c::initializeState_ModeRecovery() {}

void dLytBirdGaugeMain_c::executeState_ModeRecovery() {
    if (mIsAnimating) {
        return;
    }
    if (mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_RECOVERY + mNumDisplayedDashes - 1].isEndReached()) {
        mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_RECOVERY + mNumDisplayedDashes - 1].setAnimEnable(false);
        mIsAnimating = true;
    }
}

void dLytBirdGaugeMain_c::finalizeState_ModeRecovery() {}

void dLytBirdGaugeMain_c::initializeState_ModeOut() {}

void dLytBirdGaugeMain_c::executeState_ModeOut() {
    if (mIsAnimating) {
        return;
    }
    if (mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_OUT].isEndReached()) {
        mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_OUT].setAnimEnable(false);
        mIsAnimating = true;
    }
}

void dLytBirdGaugeMain_c::finalizeState_ModeOut() {}

dLytBirdGaugeMain_c::dLytBirdGaugeMain_c() : mStateMgr(*this) {}

bool dLytBirdGaugeMain_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("birdGauge_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < LYT_SKY_GAUGE_MAIN_NUM_ANIMS; i++) {
        mAnmGroups[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnmGroups[i].bind(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setAnimEnable(true);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setFrame(0.0f);

    for (int i = 0; i < LYT_SKY_GAUGE_MAIN_NUM_WINGS; i++) {
        mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_DASH + i].setAnimEnable(true);
        mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_DASH + i].setFrame(0.0f);
    }

    mLyt.calc();

    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setAnimEnable(false);

    for (int i = 0; i < LYT_SKY_GAUGE_MAIN_NUM_WINGS; i++) {
        mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_DASH + i].setAnimEnable(false);
    }

    mNumDisplayedDashes = LYT_SKY_GAUGE_MAIN_NUM_WINGS;
    cancelAnimation();
    return true;
}

bool dLytBirdGaugeMain_c::execute() {
    mStateMgr.executeState();
    for (int i = 0; i < LYT_SKY_GAUGE_MAIN_NUM_ANIMS; i++) {
        if (mAnmGroups[i].isEnabled()) {
            mAnmGroups[i].play();
        }
    }
    mLyt.calc();
    return true;
}

bool dLytBirdGaugeMain_c::remove() {
    mLyt.unbindAnims();
    for (int i = 0; i < LYT_SKY_GAUGE_MAIN_NUM_ANIMS; i++) {
        mAnmGroups[i].remove();
    }
    return true;
}

void dLytBirdGaugeMain_c::cancelAnimation() {
    mStateMgr.changeState(StateID_ModeNone);
    mIsAnimating = false;
}

void dLytBirdGaugeMain_c::changeToIn() {
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setAnimEnable(true);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_IN].setFrame(0.0f);
    mLyt.calc();
    mStateMgr.changeState(StateID_ModeIn);
    mIsAnimating = false;
}

void dLytBirdGaugeMain_c::changeToDash() {
    mNumDisplayedDashes--;
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_RECOVERY + mNumDisplayedDashes].setAnimEnable(false);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_DASH + mNumDisplayedDashes].setAnimEnable(true);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_DASH + mNumDisplayedDashes].setFrame(0.0f);
    mLyt.calc();
    mStateMgr.changeState(StateID_ModeDash);
    mIsAnimating = false;
}

void dLytBirdGaugeMain_c::changeToRecovery() {
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_DASH + mNumDisplayedDashes].setAnimEnable(false);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_RECOVERY + mNumDisplayedDashes].setAnimEnable(true);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_RECOVERY + mNumDisplayedDashes].setFrame(0.0f);
    mLyt.calc();
    SmallSoundManager::GetInstance()->playSound(SE_S_BIRD_RECOVER);
    mStateMgr.changeState(StateID_ModeRecovery);
    mIsAnimating = false;
    mNumDisplayedDashes++;
}

void dLytBirdGaugeMain_c::changeToOut() {
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_OUT].setAnimEnable(true);
    mAnmGroups[LYT_SKY_GAUGE_MAIN_ANIM_OUT].setFrame(0.0f);
    mLyt.calc();
    mStateMgr.changeState(StateID_ModeOut);
    mIsAnimating = false;
}

void dLytBirdGauge_c::initializeState_In() {}

void dLytBirdGauge_c::executeState_In() {
    if (mMain.isAnimating()) {
        mMain.cancelAnimation();
        mStateMgr.changeState(StateID_None);
        field_0x692 = true;
    }
}

void dLytBirdGauge_c::finalizeState_In() {}

void dLytBirdGauge_c::initializeState_None() {}

void dLytBirdGauge_c::executeState_None() {
    s32 displayedDashes = mMain.getDisplayedDashes();
    if (field_0x690) {
        mMain.changeToIn();
        mStateMgr.changeState(StateID_In);
        field_0x690 = false;
        field_0x692 = false;
        field_0x693 = false;
    } else if (displayedDashes != 0 && mGameStateDashes < displayedDashes) {
        mMain.changeToDash();
        mStateMgr.changeState(StateID_Dash);
    } else if (displayedDashes < 3 && mGameStateDashes > displayedDashes) {
        mMain.changeToRecovery();
        mStateMgr.changeState(StateID_Recovery);
    }
}

void dLytBirdGauge_c::finalizeState_None() {}

void dLytBirdGauge_c::initializeState_Dash() {}

void dLytBirdGauge_c::executeState_Dash() {
    if (mMain.isAnimating()) {
        mMain.cancelAnimation();
        mStateMgr.changeState(StateID_None);
    }
}

void dLytBirdGauge_c::finalizeState_Dash() {}

void dLytBirdGauge_c::initializeState_Recovery() {}

void dLytBirdGauge_c::executeState_Recovery() {
    if (mMain.isAnimating()) {
        mMain.cancelAnimation();
        mStateMgr.changeState(StateID_None);
    }
}

void dLytBirdGauge_c::finalizeState_Recovery() {}

void dLytBirdGauge_c::initializeState_Out() {}

void dLytBirdGauge_c::executeState_Out() {
    if (mMain.isAnimating()) {
        field_0x693 = true;
        mMain.cancelAnimation();
        mStateMgr.changeState(StateID_None);
    }
}

void dLytBirdGauge_c::finalizeState_Out() {}

void dLytBirdGauge_c::hide() {
    mMain.changeToOut();
    mStateMgr.changeState(StateID_Out);
    field_0x692 = false;
    field_0x693 = false;
}

bool dLytBirdGauge_c::build(d2d::ResAccIf_c *resAcc) {
    sInstance = this;
    mMain.build(resAcc);
    field_0x690 = false;
    field_0x691 = false;
    field_0x692 = false;
    field_0x693 = true;
    mGameStateDashes = LYT_SKY_GAUGE_MAIN_NUM_WINGS;
    mStateMgr.changeState(StateID_None);
    return true;
}

bool dLytBirdGauge_c::execute() {
    mMain.execute();
    mStateMgr.executeState();
    return true;
}

bool dLytBirdGauge_c::remove() {
    mMain.remove();
    return true;
}
