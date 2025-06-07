

#include "d/lyt/d_lyt_common_a_btn.h"

#include "common.h"
#include "d/d_sys.h"
#include "d/lyt/d2d.h"
#include "d/snd/d_snd_small_effect_mgr.h"

static const d2d::LytBrlanMapping brlanMap[] = {
    {    "aBtn_00_in.brlan",   "G_inOut_00"},
    {  "aBtn_00_loop.brlan", "G_loopBtn_00"},
    {"aBtn_00_decide.brlan",  "G_decide_00"},
    {   "aBtn_00_out.brlan",   "G_inOut_00"},
};

#define A_BTN_ANIM_IN 0
#define A_BTN_ANIM_LOOP 1
#define A_BTN_ANIM_DECIDE 2
#define A_BTN_ANIM_OUT 3

#define A_BTN_NUM_ANIMS 4

const dLytCommonABtn_c::executeFunc dLytCommonABtn_c::sExecuteFuncs[4] = {
    &dLytCommonABtn_c::executeStateInvisible,
    &dLytCommonABtn_c::executeStateIn,
    &dLytCommonABtn_c::executeStateVisible,
    &dLytCommonABtn_c::executeStateDecideOut,
};

bool dLytCommonABtn_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("aBtn_00.brlyt", nullptr);

    for (int i = 0; i < A_BTN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
    }

    resetToInvisble();
    setSoundVariant(VARIANT_NORMAL);
    return true;
}

bool dLytCommonABtn_c::remove() {
    for (int i = 0; i < A_BTN_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

bool dLytCommonABtn_c::execute() {
    runExecuteFunc();
    if (mState == STATE_INVISIBLE) {
        return true;
    }
    mLyt.calc();
    return true;
}

bool dLytCommonABtn_c::requestIn() {
    if (mState != STATE_INVISIBLE) {
        return false;
    }
    mInRequested = true;
    return true;
}

bool dLytCommonABtn_c::requestOut(bool b) {
    if (mState != STATE_VISIBLE) {
        return false;
    }
    mOutRequested = true;
    field_0x1AC = b;
    return true;
}

void dLytCommonABtn_c::resetToInvisble() {
    gotoStateInvisible();
}

void dLytCommonABtn_c::setSoundVariant(SoundVariant value) {
    mSoundVariant = value;
}

void dLytCommonABtn_c::runExecuteFunc() {
    executeFunc fn = sExecuteFuncs[mState];
    if (fn != nullptr) {
        (this->*fn)();
    }
}

void dLytCommonABtn_c::gotoStateInvisible() {
    setState(STATE_INVISIBLE);
    mLyt.unbindAnims();

    d2d::AnmGroup_c &inAnim = mAnm[A_BTN_ANIM_IN];
    field_0x1A8 = 0;
    mInRequested = false;
    mOutRequested = false;
    mIsDoneOut = false;
    field_0x1AC = 0;
    mNoDecide = false;

    inAnim.bind(false);
    inAnim.setAnimEnable(true);
    inAnim.setFrame(0.0f);
    if (dSys::getFrameRate() == 1) {
        inAnim.setRate(0.5f);
    } else {
        inAnim.setRate(1.0f);
    }
    mLyt.calc();
}

void dLytCommonABtn_c::executeStateInvisible() {
    if (mInRequested != true) {
        return;
    }

    if (field_0x1AC == 1) {
        gotoStateInvisible();
    }
    mInRequested = false;
    field_0x1A8 = 1;
    gotoStateIn();
}

void dLytCommonABtn_c::gotoStateIn() {
    setState(STATE_IN);
}

void dLytCommonABtn_c::executeStateIn() {
    d2d::AnmGroup_c &anm = mAnm[A_BTN_ANIM_IN];
    if (anm.isEndReached() == true) {
        goToStateVisible();
    } else {
        anm.play();
    }
}

void dLytCommonABtn_c::goToStateVisible() {
    setState(STATE_VISIBLE);
    mAnm[A_BTN_ANIM_IN].unbind();
    d2d::AnmGroup_c &anm = mAnm[A_BTN_ANIM_LOOP];
    anm.bind(false);
    anm.setFrame(0.0f);
    if (dSys::getFrameRate() == 1) {
        anm.setRate(0.5f);
    } else {
        anm.setRate(1.0f);
    }
}

void dLytCommonABtn_c::executeStateVisible() {
    if (mOutRequested == true) {
        goToStateDecideOut();
        mOutRequested = false;
    } else {
        d2d::AnmGroup_c &anm = mAnm[A_BTN_ANIM_LOOP];
        anm.play();
    }
}

void dLytCommonABtn_c::goToStateDecideOut() {
    setState(STATE_OUT);
    mAnm[A_BTN_ANIM_LOOP].unbind();
    if (mNoDecide == true) {
        mNoDecide = false;
        d2d::AnmGroup_c &anm = mAnm[A_BTN_ANIM_OUT];
        anm.bind(false);
        anm.setFrame(0.0f);
        if (dSys::getFrameRate() == 1) {
            anm.setRate(0.5f);
        } else {
            anm.setRate(1.0f);
        }
        mOutState = OUT_STATE_OUT;
    } else {
        d2d::AnmGroup_c &anm = mAnm[A_BTN_ANIM_DECIDE];
        anm.bind(false);
        anm.setAnimEnable(true);
        anm.setFrame(0.0f);
        if (dSys::getFrameRate() == 1) {
            anm.setRate(0.5f);
        } else {
            anm.setRate(1.0f);
        }
        if (mSoundVariant == VARIANT_NORMAL) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_WINDOW_PRESS_A);
        } else {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TITLE_PRESS_START);
        }
    }
}

void dLytCommonABtn_c::executeStateDecideOut() {
    d2d::AnmGroup_c &decideAnm = mAnm[A_BTN_ANIM_DECIDE];
    switch (mOutState) {
        case OUT_STATE_DECIDING: {
            if (decideAnm.isEndReached() == true) {
                mOutState = OUT_STATE_GOTO_OUT;
                if (field_0x1AC == 1) {
                    mIsDoneOut = true;
                    setState(STATE_INVISIBLE);
                    break;
                }
            }
            decideAnm.play();
            break;
        }
        case OUT_STATE_GOTO_OUT: {
            decideAnm.unbind();
            d2d::AnmGroup_c &anmOut = mAnm[A_BTN_ANIM_OUT];
            anmOut.bind(false);
            anmOut.setAnimEnable(true);
            anmOut.setFrame(0.0f);
            if (dSys::getFrameRate() == 1) {
                anmOut.setRate(0.5f);
            } else {
                anmOut.setRate(1.0f);
            }
            mOutState = OUT_STATE_OUT;
            break;
        }
        case OUT_STATE_OUT: {
            d2d::AnmGroup_c &anmOut = mAnm[A_BTN_ANIM_OUT];
            if (anmOut.isEndReached() == true) {
                mOutState = OUT_STATE_GOTO_INVISIBLE;
                mIsDoneOut = true;
            }
            anmOut.play();
            break;
        }
        case OUT_STATE_GOTO_INVISIBLE: {
            gotoStateInvisible();
            break;
        }
    }
}

void dLytCommonABtn_c::setState(ABtnState state) {
    mState = state;
    mOutState = OUT_STATE_DECIDING;
}
