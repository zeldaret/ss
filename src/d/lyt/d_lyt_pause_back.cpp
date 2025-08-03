#include "d/lyt/d_lyt_pause_back.h"

#include "common.h"
#include "d/d_stage_mgr.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_pause.h"
#include "rvl/GX/GXTexture.h"

STATE_DEFINE(dLytPauseBack_c, None);
STATE_DEFINE(dLytPauseBack_c, In);
STATE_DEFINE(dLytPauseBack_c, Wait);
STATE_DEFINE(dLytPauseBack_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    { "pauseBace_00_in.brlan", "G_inOut_00"},
    {"pauseBace_00_out.brlan", "G_inOut_00"},
};

#define PAUSE_BACK_ANIM_IN 0
#define PAUSE_BACK_ANIM_OUT 1

#define PAUSE_BACK_NUM_ANIMS 2

dLytPauseBack_c::dLytPauseBack_c() : mStateMgr(*this) {}

bool dLytPauseBack_c::build() {
    d2d::ResAccIf_c *resAcc = dLytPauseMgr_c::GetInstance()->getResAcc1();
    mLyt.setResAcc(resAcc);
    mLyt.build("pauseBace_00.brlyt", nullptr);
    mLyt.setPriority(0x86);

    for (int i = 0; i < PAUSE_BACK_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
    }

    mStateMgr.changeState(StateID_None);
    return true;
}

bool dLytPauseBack_c::remove() {
    mLyt.unbindAnims();
    for (int i = 0; i < PAUSE_BACK_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

bool dLytPauseBack_c::execute() {
    mStateMgr.executeState();
    mLyt.calc();
    return true;
}

bool dLytPauseBack_c::draw() {
    if (mVisible == true) {
        mLyt.addToDrawList();
    }
    return true;
}

void dLytPauseBack_c::requestIn() {
    mInRequest = true;
}

void dLytPauseBack_c::requestOut() {
    mOutRequest = true;
}

void dLytPauseBack_c::initializeState_None() {
    mLyt.unbindAnims();
    field_0x156 = false;
    mInRequest = false;
    mOutRequest = false;
    mVisible = false;
    mOutStep = 0;
}
void dLytPauseBack_c::executeState_None() {
    if (mInRequest == true) {
        mInRequest = false;
        mStateMgr.changeState(StateID_In);
    }
}
void dLytPauseBack_c::finalizeState_None() {}

void dLytPauseBack_c::initializeState_In() {
    startAnim(PAUSE_BACK_ANIM_IN);
    mVisible = true;
    if (dStageMgr_c::GetInstance()->getField_0x8898() != nullptr) {
        GXTexObj texObj;
        dStageMgr_c::GetInstance()->getField_0x8898()->getTexObj(&texObj);
        mLyt.findPicture("P_capture_00")->GetMaterial()->GetTexMapAry()->ReplaceImage(texObj);
        mLyt.findPicture("P_capture_01")->GetMaterial()->GetTexMapAry()->ReplaceImage(texObj);
    }
}
void dLytPauseBack_c::executeState_In() {
    d2d::AnmGroup_c &anm = mAnm[PAUSE_BACK_ANIM_IN];
    if (anm.isEndReached() == true) {
        mStateMgr.changeState(StateID_Wait);
    } else {
        anm.play();
    }
}
void dLytPauseBack_c::finalizeState_In() {}

void dLytPauseBack_c::initializeState_Wait() {
    field_0x156 = true;
}
void dLytPauseBack_c::executeState_Wait() {
    field_0x156 = false;
    if (mOutRequest == true) {
        mOutRequest = false;
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytPauseBack_c::finalizeState_Wait() {}

void dLytPauseBack_c::initializeState_Out() {
    stopAnim(PAUSE_BACK_ANIM_IN);
    startAnim(PAUSE_BACK_ANIM_OUT);
    mOutStep = 0;
}
void dLytPauseBack_c::executeState_Out() {
    d2d::AnmGroup_c &anm = mAnm[PAUSE_BACK_ANIM_OUT];
    switch (mOutStep) {
        case 0: {
            if (anm.isEndReached() == true) {
                mOutStep = 1;
                field_0x156 = true;
            }
            break;
        }
        case 1: {
            mStateMgr.changeState(StateID_None);
            return;
        }
    }
    anm.play();
}
void dLytPauseBack_c::finalizeState_Out() {}

void dLytPauseBack_c::startAnim(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.bind(false);
    anm.setFrame(0.0f);
}

void dLytPauseBack_c::stopAnim(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.unbind();
}

void dLytPauseBack_c::loadBgTexture() {
    if (dLytPauseMgr_c::GetInstance()->getBgTexture() != nullptr) {
        GXTexObj texObj;
        dLytPauseMgr_c::GetInstance()->getBgTexture()->getTexObj(&texObj);
        mLyt.findPicture("P_capture_00")->GetMaterial()->GetTexMapAry()->ReplaceImage(texObj);
        mLyt.findPicture("P_capture_01")->GetMaterial()->GetTexMapAry()->ReplaceImage(texObj);
    }
}
