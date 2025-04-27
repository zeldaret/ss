#include "d/lyt/d_lyt_wipe.h"

#include "common.h"
#include "d/d_stage.h"
#include "d/lyt/d2d.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

STATE_DEFINE(dLytWipeMain_c, ModeNone);
STATE_DEFINE(dLytWipeMain_c, ModeIn);
STATE_DEFINE(dLytWipeMain_c, ModeWait);
STATE_DEFINE(dLytWipeMain_c, ModeOut);

STATE_DEFINE(dLytWipe_c, None);
STATE_DEFINE(dLytWipe_c, In);
STATE_DEFINE(dLytWipe_c, Wait);
STATE_DEFINE(dLytWipe_c, Out);

dLytWipe_c *dLytWipe_c::sInstance;

#define WIPE_ANIM_INOUT 0
#define WIPE_NUM_ANIMS 1

void dLytWipeMain_c::initializeState_ModeNone() {}
void dLytWipeMain_c::executeState_ModeNone() {
    mStateEndReached = true;
}
void dLytWipeMain_c::finalizeState_ModeNone() {}

void dLytWipeMain_c::initializeState_ModeIn() {}
void dLytWipeMain_c::executeState_ModeIn() {
    if (mAnm[WIPE_ANIM_INOUT].isStop2()) {
        mStateEndReached = true;
    }
}
void dLytWipeMain_c::finalizeState_ModeIn() {}

void dLytWipeMain_c::initializeState_ModeWait() {}
void dLytWipeMain_c::executeState_ModeWait() {
    mStateEndReached = true;
}
void dLytWipeMain_c::finalizeState_ModeWait() {}

void dLytWipeMain_c::initializeState_ModeOut() {}
void dLytWipeMain_c::executeState_ModeOut() {
    if (mAnm[WIPE_ANIM_INOUT].isStop2()) {
        mStateEndReached = true;
    }
}
void dLytWipeMain_c::finalizeState_ModeOut() {}

dLytWipeMain_c::dLytWipeMain_c() : mStateMgr(*this, sStateID::null) {}

static const d2d::LytBrlanMapping brlanMap[] = {
    {"wipe_00_inOut.brlan", "G_inOut_00"},
};

bool dLytWipeMain_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("wipe_00.brlyt", nullptr);
    mLyt.setPriority(0);

    for (int i = 0; i < WIPE_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[WIPE_ANIM_INOUT].setAnimEnable(true);
    mAnm[WIPE_ANIM_INOUT].setFrame(0.0f);
    mLyt.calc();
    mAnm[WIPE_ANIM_INOUT].setAnimEnable(false);
    mIsIn = false;
    return true;
}

bool dLytWipeMain_c::remove() {
    mLyt.unbindAnims();
    for (int i = 0; i < WIPE_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

void dLytWipeMain_c::gotoIn() {
    if (!mIsIn) {
        mAnm[WIPE_ANIM_INOUT].setForwardOnce();
        mAnm[WIPE_ANIM_INOUT].setAnimEnable(true);
        if (mStateEndReached) {
            mAnm[WIPE_ANIM_INOUT].setToStart();
        } else {
            mStateMgr.finalizeState(); // ???????
        }
        mStateMgr.changeState(StateID_ModeIn);
        mIsIn = true;
        mStateEndReached = false;
    }
}

void dLytWipeMain_c::gotoWait() {
    mAnm[WIPE_ANIM_INOUT].setAnimEnable(false);
    mStateMgr.changeState(StateID_ModeWait);
    mStateEndReached = false;
}

void dLytWipeMain_c::gotoOut() {
    if (mIsIn) {
        mAnm[WIPE_ANIM_INOUT].setBackwardsOnce();
        mAnm[WIPE_ANIM_INOUT].setAnimEnable(true);
        if (mStateEndReached) {
            // Nothing to do - Anim already at end
        } else {
            mStateMgr.finalizeState(); // ???????
        }
        mStateMgr.changeState(StateID_ModeOut);
        mIsIn = false;
        mStateEndReached = false;
    }
}

void dLytWipeMain_c::gotoNone() {
    mStateMgr.changeState(StateID_ModeNone);
    mStateEndReached = false;
}

bool dLytWipeMain_c::execute() {
    mStateMgr.executeState();
    for (int i = 0; i < WIPE_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    return true;
}

bool dLytWipeMain_c::draw() {
    mLyt.addToDrawList();
    return true;
}

void dLytWipe_c::initializeState_None() {
    mWantsIn = false;
}
void dLytWipe_c::executeState_None() {
    mWantsOut = false;
    if (mWantsIn) {
        mTransitionStage = 0;
        mIsSettled = false;
        // State IN fades OUT, because I guess the fader fades the scene out,
        // which means Wipe is In
        if (dStage_c::GetInstance()->fadeOut(2, 16)) {
            mStateMgr.changeState(StateID_In);
            mWantsIn = false;
            mVisible = true;
        }
    }
}
void dLytWipe_c::finalizeState_None() {}

void dLytWipe_c::initializeState_In() {}
void dLytWipe_c::executeState_In() {
    if (mTransitionStage == 0) {
        if (dStage_c::GetInstance()->isFadedOut()) {
            mFadeDelay = 15;
            mTransitionStage = 1;
        }
    } else if (mTransitionStage == 1) {
        mFadeDelay--;
        if (mFadeDelay == 0) {
            dStage_c::GetInstance()->forceFadeIn();
            mMain.gotoIn();
            mTransitionStage = 2;
        }
    } else {
        mTransitionStage = 3;
        if (mMain.isStateEndReached()) {
            mMain.gotoWait();
            mStateMgr.changeState(StateID_Wait);
            mIsSettled = true;
        }
    }
}
void dLytWipe_c::finalizeState_In() {}

void dLytWipe_c::initializeState_Wait() {}
void dLytWipe_c::executeState_Wait() {
    mWantsIn = false;
    if (mWantsOut) {
        mTransitionStage = 0;
        mMain.gotoOut();
        mStateMgr.changeState(StateID_Out);
        mWantsOut = false;
        mIsSettled = false;
    }
}
void dLytWipe_c::finalizeState_Wait() {}

void dLytWipe_c::initializeState_Out() {}
void dLytWipe_c::executeState_Out() {
    if (mTransitionStage == 0) {
        if (mMain.isStateEndReached()) {
            dStage_c::GetInstance()->forceFadeOut();
            mFadeDelay = 15;
            mTransitionStage = 1;
        }
    } else if (mTransitionStage == 1) {
        dStage_c::GetInstance()->forceFadeOut();
        mFadeDelay--;
        if (mFadeDelay == 0) {
            dStage_c::GetInstance()->fadeIn(2, 16);
            mMain.gotoNone();
            mTransitionStage = 2;
        }
    } else {
        mTransitionStage = 3;
        if (dStage_c::GetInstance()->isFadedIn()) {
            mStateMgr.changeState(StateID_None);
            mIsSettled = true;
            mVisible = false;
        }
    }
}
void dLytWipe_c::finalizeState_Out() {}

bool dLytWipe_c::build() {
    sInstance = this;
    mTransitionStage = 0;
    mFadeDelay = 0;

    mWantsIn = false;
    mWantsOut = false;
    mIsSettled = true;
    mVisible = false;

    void *data = LayoutArcManager::GetInstance()->getLoadedData("DoButton");
    mResAcc.attach(data, "");
    mMain.build(&mResAcc);

    mStateMgr.changeState(StateID_None);
    return true;
}

bool dLytWipe_c::remove() {
    sInstance = nullptr;
    mMain.remove();
    mResAcc.detach();
    return true;
}

bool dLytWipe_c::execute() {
    mMain.execute();
    mStateMgr.executeState();
    return true;
}

bool dLytWipe_c::draw() {
    if (mVisible &&
        ((mTransitionStage && (*mStateMgr.getStateID() == StateID_In || *mStateMgr.getStateID() == StateID_Wait)) ||
         (*mStateMgr.getStateID() == StateID_Out && mTransitionStage < 2))) {
        mMain.draw();
    }
    return true;
}
