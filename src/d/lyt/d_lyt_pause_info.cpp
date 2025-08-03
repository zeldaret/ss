#include "d/lyt/d_lyt_pause_info.h"

#include "common.h"
#include "d/a/d_a_itembase.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_title.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_pause.h"
#include "d/lyt/d_lyt_pause_text.h"
#include "d/lyt/d_lyt_util_items.h"
#include "sized_string.h"

#include <cstring>


STATE_DEFINE(dLytPauseInfo_c, None);
STATE_DEFINE(dLytPauseInfo_c, In);
STATE_DEFINE(dLytPauseInfo_c, Wait);
STATE_DEFINE(dLytPauseInfo_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {   "pauseInfo_00_in.brlan", "G_inOut_00"},
    {  "pauseInfo_00_out.brlan", "G_inOut_00"},
    {"pauseInfo_00_input.brlan", "G_input_00"},
};

#define PAUSE_INFO_ANIM_IN 0
#define PAUSE_INFO_ANIM_OUT 1
#define PAUSE_INFO_ANIM_INPUT 2

#define PAUSE_INFO_NUM_ANIMS 3

dLytPauseInfo_c::dLytPauseInfo_c() : mStateMgr(*this) {
    mpTitle = nullptr;
}

bool dLytPauseInfo_c::build() {
    d2d::ResAccIf_c *resAcc = dLytPauseMgr_c::GetInstance()->getResAcc1();
    mLyt.setResAcc(resAcc);
    mLyt.build("pauseInfo_00.brlyt", nullptr);
    mLyt.setPriority(0x86);

    for (int i = 0; i < PAUSE_INFO_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
    }

    mStateMgr.changeState(StateID_None);

    mpTitle = new dLytCommonTitle_c();
    mpTitle->build();
    mpTitle->setField_0x680(1);
    mInputInOut = true;
    return true;
}

bool dLytPauseInfo_c::remove() {
    if (mpTitle != nullptr) {
        mpTitle->remove();
        delete mpTitle;
        mpTitle = nullptr;
    }
    mLyt.unbindAnims();
    for (int i = 0; i < PAUSE_INFO_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

bool dLytPauseInfo_c::execute() {
    mStateMgr.executeState();
    if (!mStateMgr.getStateID()->isEqual(StateID_None)) {
        if (mInputInOut == true) {
            playBackwards(mAnm[PAUSE_INFO_ANIM_INPUT]);
        } else {
            mAnm[PAUSE_INFO_ANIM_INPUT].play();
        }
    }
    mLyt.calc();
    if (mpTitle != nullptr) {
        mpTitle->execute();
    }
    return true;
}

bool dLytPauseInfo_c::draw() {
    if (field_0x19B == true) {
        if (mpTitle != nullptr) {
            mpTitle->draw();
        }
        if (!dLytControlGame_c::getInstance()->getField_0x15C67()) {
            mLyt.addToDrawList();
        }
    }
    return true;
}

void dLytPauseInfo_c::requestIn() {
    mInRequest = true;
}

void dLytPauseInfo_c::requestOut() {
    mOutRequest = true;
}

void dLytPauseInfo_c::updateTitle() {
    if (dLytControlGame_c::getInstance()->getField_0x15C2C()) {
        // "Gear"
        mpTitle->set(dLytCommonTitle_c::SET_01, mpTitle->setSubTitle(1), nullptr);
    } else {
        // "Collection"
        mpTitle->set(dLytCommonTitle_c::SET_01, mpTitle->setSubTitle(2), nullptr);
    }
}

void dLytPauseInfo_c::setInputInOut(bool inOut) {
    if (mpTitle == nullptr) {
        return;
    }
    mpTitle->setInputInOut(inOut);
    mInputInOut = inOut;
}

void dLytPauseInfo_c::initializeState_None() {
    mLyt.unbindAnims();
    field_0x19A = false;
    mInRequest = false;
    mOutRequest = false;
    field_0x19B = false;
    mStep = 0;
}
void dLytPauseInfo_c::executeState_None() {
    if (mInRequest == true) {
        mInRequest = false;
        mStateMgr.changeState(StateID_In);
    }
}
void dLytPauseInfo_c::finalizeState_None() {}

void dLytPauseInfo_c::initializeState_In() {
    startAnim(PAUSE_INFO_ANIM_IN);
    startAnim(PAUSE_INFO_ANIM_INPUT);
    field_0x19B = true;

    s32 msgIdx = 1; // "Gear"
    if (dLytControlGame_c::getInstance()->getField_0x15C2C() != 0) {
        msgIdx = 2; // "Collection"
    }
    if (dLytControlGame_c::getInstance()->getField_0x15C67()) {
        SizedString<32> label;
        if (dLytControlGame_c::getInstance()->getItemForPauseDemo() == ITEM_5_CRYSTALS) {
            getItemLabel(ITEM_1_CRYSTAL, label);
        } else {
            getItemLabel(dLytControlGame_c::getInstance()->getItemForPauseDemo(), label);
        }

        if (std::strlen(label) != 0) {
            const char *msgLabel = mpTitle->setSubTitle(msgIdx);
            mpTitle->set(dLytCommonTitle_c::SET_00, msgLabel, label);
        } else {
            const char *msgLabel = mpTitle->setSubTitle(msgIdx);
            mpTitle->set(dLytCommonTitle_c::SET_00, msgLabel, nullptr);
        }
    } else {
        const char *msgLabel = mpTitle->setSubTitle(msgIdx);
        mpTitle->set(dLytCommonTitle_c::SET_00, msgLabel, nullptr);
    }
}
void dLytPauseInfo_c::executeState_In() {
    d2d::AnmGroup_c &anm = mAnm[PAUSE_INFO_ANIM_IN];
    if (anm.isEndReached() == true) {
        mStateMgr.changeState(StateID_Wait);
    } else {
        anm.play();
    }
}
void dLytPauseInfo_c::finalizeState_In() {}

void dLytPauseInfo_c::initializeState_Wait() {
    field_0x19A = true;
}
void dLytPauseInfo_c::executeState_Wait() {
    updateCaption();
    field_0x19A = false;
    if (mOutRequest == true) {
        mOutRequest = false;
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytPauseInfo_c::finalizeState_Wait() {}

void dLytPauseInfo_c::initializeState_Out() {
    stopAnim(PAUSE_INFO_ANIM_IN);
    startAnim(PAUSE_INFO_ANIM_OUT);
    mpTitle->set(dLytCommonTitle_c::SET_OUT, nullptr, nullptr);
    mStep = 0;
}
void dLytPauseInfo_c::executeState_Out() {
    d2d::AnmGroup_c &anm = mAnm[PAUSE_INFO_ANIM_OUT];
    switch (mStep) {
        case 0: {
            if (anm.isEndReached() == true) {
                mStep = 1;
                field_0x19A = true;
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
void dLytPauseInfo_c::finalizeState_Out() {}

void dLytPauseInfo_c::startAnim(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.bind(false);
    anm.setFrame(0.0f);
}

void dLytPauseInfo_c::stopAnim(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.unbind();
}

void dLytPauseInfo_c::playBackwards(d2d::AnmGroup_c &anm) {
    f32 frame = anm.getFrame();
    if (frame) {
        frame -= 1.0f;
        if (frame <= 0.0f) {
            frame = 0.0f;
        }
        anm.setFrame(frame);
    }
}

void dLytPauseInfo_c::updateCaption() {
    if (dLytControlGame_c::getInstance()->getField_0x15C67() != true &&
        dLytPauseMgr_c::GetInstance()->getField_0x083B() == true) {
        SizedString<32> buf;
        const char *label = dLytPauseText_c::getCurrentPauseLabel(buf);
        mpTitle->set(dLytCommonTitle_c::SET_CAPTION_IMMEDIATELY, nullptr, label);
    }
}
