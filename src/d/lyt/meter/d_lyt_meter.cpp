#include "d/lyt/d_lyt_meter_configuration.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/lyt/d_lyt_area_caption.h"
#include "d/lyt/d_window.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

static dLytMeterConfiguration_c sConf;

STATE_DEFINE(dLytMeter1Button_c, Wait);
STATE_DEFINE(dLytMeter1Button_c, ToUse);
STATE_DEFINE(dLytMeter1Button_c, ToUnuse);
STATE_DEFINE(dLytMeter1Button_c, Unuse);

STATE_DEFINE(dLytMeter2Button_c, Wait);
STATE_DEFINE(dLytMeter2Button_c, ToUse);
STATE_DEFINE(dLytMeter2Button_c, ToUnuse);
STATE_DEFINE(dLytMeter2Button_c, Unuse);

STATE_DEFINE(dLytMeterParts_c, Invisible);
STATE_DEFINE(dLytMeterParts_c, In);
STATE_DEFINE(dLytMeterParts_c, Visible);
STATE_DEFINE(dLytMeterParts_c, Out);

void dLytMeter1Button_c::initializeState_Wait() {}
void dLytMeter1Button_c::executeState_Wait() {
    if (dLytMeterContainer_c::sInstance->isButtonFlag(0x80)) {
        return;
    }
    mStateMgr.changeState(StateID_ToUnuse);
}
void dLytMeter1Button_c::finalizeState_Wait() {}

void dLytMeter1Button_c::initializeState_ToUse() {
    mAnmGroups[0].playBackwardsOnce();
    mAnmGroups[0].setAnimEnable(true);
}
void dLytMeter1Button_c::executeState_ToUse() {
    if (mAnmGroups[0].isStop2()) {
        mStateMgr.changeState(StateID_Wait);
    }

    if (mAnmGroups[0].isFlag2()) {
        mAnmGroups[0].play();
    }
}
void dLytMeter1Button_c::finalizeState_ToUse() {
    mAnmGroups[0].setAnimEnable(false);
}

void dLytMeter1Button_c::initializeState_ToUnuse() {
    mAnmGroups[0].playLoop();
    mAnmGroups[0].setAnimEnable(true);
}
void dLytMeter1Button_c::executeState_ToUnuse() {
    if (mAnmGroups[0].isStop2()) {
        mStateMgr.changeState(StateID_Unuse);
    }

    if (mAnmGroups[0].isFlag2()) {
        mAnmGroups[0].play();
    }
}
void dLytMeter1Button_c::finalizeState_ToUnuse() {
    mAnmGroups[0].setAnimEnable(false);
}

void dLytMeter1Button_c::initializeState_Unuse() {
    field_0x1A4 = 15;
}
void dLytMeter1Button_c::executeState_Unuse() {
    if (dLytMeterContainer_c::sInstance->isButtonFlag(0x80)) {
        if (!(field_0x1A4 > 0)) {
            mStateMgr.changeState(StateID_ToUse);
            return;
        }
        field_0x1A4 -= 1;
    } else {
        field_0x1A4 = 15;
    }
}
void dLytMeter1Button_c::finalizeState_Unuse() {}

static const d2d::LytBrlanMapping btn1BrlanMap[] = {
    {"remoConBtn_04_input.brlan", "G_input_00"},
    {"remoConBtn_04_call.brlan", "G_call_00"},
    {"remoConBtn_04_loop.brlan", "G_loop_00"},
};

static char *sWindowName1 = "W_bgP_00";

bool dLytMeter1Button_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBtn_04.brlyt", nullptr);

    for (int i = 0; i < 3; i++) {
        mAnmGroups[i].init(btn1BrlanMap[i].mFile, resAcc, mLyt.getLayout(), btn1BrlanMap[i].mName);
        mAnmGroups[i].setDirection(false);
        mAnmGroups[i].setAnimEnable(false);
    }
    // TODO
    mLyt.fn_800AB9A0(mLyt.getTextBox("T_sabBtnS_00"), nullptr);
    mLyt.fn_800AB9A0(mLyt.getTextBox("T_sabBtn_00"), nullptr);
    mpWindow = mLyt.getWindow(sWindowName1);
    mpTextbox = mLyt.getSizeBoxInWindow(sWindowName1);
    mpWindow->UpdateSize(mpTextbox, 32.0f);
    field_0x1A4 = 15;
    field_0x1AC = 0;
    field_0x1AD = 0;
    field_0x1A8 = 0;
    mAnmGroups[1].setToEnd();
    mAnmGroups[1].setAnimEnable(true);
    mAnmGroups[2].setAnimEnable(true);
    mLyt.calc();
    mAnmGroups[1].setAnimEnable(false);
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeter1Button_c::LytMeter0x10() {
    for (int i = 0; i < 3; i++) {
        mAnmGroups[i].unbind();
        mAnmGroups[i].afterUnbind();
    }
    return true;
}

bool dLytMeter1Button_c::LytMeter0x14() {
    if (*mStateMgr.getStateID() == StateID_ToUnuse) {
        // TODO
    } else {
        mStateMgr.executeState();
    }
}

void dLytMeter2Button_c::initializeState_Wait() {}
void dLytMeter2Button_c::executeState_Wait() {
    if (dLytMeterContainer_c::sInstance->isButtonFlag(0x100)) {
        return;
    }
    mStateMgr.changeState(StateID_ToUnuse);
}
void dLytMeter2Button_c::finalizeState_Wait() {}

void dLytMeter2Button_c::initializeState_ToUse() {
    mAnmGroups[0].playBackwardsOnce();
    mAnmGroups[0].setAnimEnable(true);
}
void dLytMeter2Button_c::executeState_ToUse() {
    if (mAnmGroups[0].isStop2()) {
        mStateMgr.changeState(StateID_Wait);
    }

    if (mAnmGroups[0].isFlag2()) {
        mAnmGroups[0].play();
    }
}
void dLytMeter2Button_c::finalizeState_ToUse() {
    mAnmGroups[0].setAnimEnable(false);
}

void dLytMeter2Button_c::initializeState_ToUnuse() {
    mAnmGroups[0].playLoop();
    mAnmGroups[0].setAnimEnable(true);
}
void dLytMeter2Button_c::executeState_ToUnuse() {
    if (mAnmGroups[0].isStop2()) {
        mStateMgr.changeState(StateID_Unuse);
    }

    if (mAnmGroups[0].isFlag2()) {
        mAnmGroups[0].play();
    }
}
void dLytMeter2Button_c::finalizeState_ToUnuse() {
    mAnmGroups[0].setAnimEnable(false);
}

void dLytMeter2Button_c::initializeState_Unuse() {
    field_0x1A4 = 15;
}
void dLytMeter2Button_c::executeState_Unuse() {
    if (dLytMeterContainer_c::sInstance->isButtonFlag(0x100)) {
        if (!(field_0x1A4 > 0)) {
            mStateMgr.changeState(StateID_ToUse);
            return;
        }
        field_0x1A4 -= 1;
    } else {
        field_0x1A4 = 15;
    }
}
void dLytMeter2Button_c::finalizeState_Unuse() {}

static const d2d::LytBrlanMapping btn2BrlanMap[] = {
    {"remoConBtn_05_input.brlan", "G_input_00"},
    {"remoConBtn_05_call.brlan", "G_call_00"},
    {"remoConBtn_05_loop.brlan", "G_loop_00"},
};

static char *sWindowName2 = "W_bgP_00";

bool dLytMeter2Button_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBtn_05.brlyt", nullptr);

    for (int i = 0; i < 3; i++) {
        mAnmGroups[i].init(btn2BrlanMap[i].mFile, resAcc, mLyt.getLayout(), btn2BrlanMap[i].mName);
        mAnmGroups[i].setDirection(false);
        mAnmGroups[i].setAnimEnable(false);
    }
    mpWindow = mLyt.getWindow(sWindowName2);
    mpTextbox = mLyt.getSizeBoxInWindow(sWindowName2);
    mpWindow->UpdateSize(mpTextbox, 32.0f);
    field_0x1A4 = 15;
    field_0x1AC = 0;
    field_0x1AD = 0;
    field_0x1A8 = 0;
    mAnmGroups[1].setToEnd();
    mAnmGroups[1].setAnimEnable(true);
    mAnmGroups[2].setAnimEnable(true);
    mLyt.calc();
    mAnmGroups[1].setAnimEnable(false);
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeter2Button_c::LytMeter0x10() {
    for (int i = 0; i < 3; i++) {
        mAnmGroups[i].unbind();
        mAnmGroups[i].afterUnbind();
    }
    return true;
}

bool dLytMeter2Button_c::LytMeter0x14() {
    if (*mStateMgr.getStateID() == StateID_ToUnuse) {
        // TODO
    } else {
        mStateMgr.executeState();
    }
}

void floats() {
    32.0f;
    1.0f;
    0.0f;
    int x = 0;
    f32 f = x;
}

void floats2() {
    5.0f;
    3.0f;
}

#pragma dont_inline on
dLytMeter_c::dLytMeter_c() {}
#pragma dont_inline off

extern "C" void *GLOBAL_TITLE_RELOADER;
extern "C" void fn_800D97E0(int i);
extern "C" void fn_800D9800(int i);
extern "C" void fn_801B2D10(void *);
extern "C" void *GLOBAL_MESSAGE_RELATED_CONTEXT;

static const d2d::LytBrlanMapping meterBrlanMap[] = {
    {"basicPosition_00_in.brlan", "G_remoConBtn_00"},
    {"basicPosition_00_in.brlan", "G_remoConBtn_01"},
    {"basicPosition_00_in.brlan", "G_remoConBtn_02"},
    {"basicPosition_00_in.brlan", "G_remoConBtn_03"},
    {"basicPosition_00_in.brlan", "G_remoConBtn_04"},
    {"basicPosition_00_in.brlan", "G_remoConBtn_05"},
    {"basicPosition_00_in.brlan", "G_remoConBtn_06"},
    {"basicPosition_00_in.brlan", "G_remoConBg_00"},
    {"basicPosition_00_in.brlan", "G_nunBtn_03"},
    {"basicPosition_00_in.brlan", "G_nunBtn_01"},
    {"basicPosition_00_in.brlan", "G_nunBtn_02"},
    {"basicPosition_00_in.brlan", "G_nunBg_00"},
    {"basicPosition_00_in.brlan", "G_shield_00"},
    {"basicPosition_00_in.brlan", "G_rupy_00"},
    {"basicPosition_00_in.brlan", "G_heart_00"},
    {"basicPosition_00_in.brlan", "G_ganbari_00"},

    {"basicPosition_00_out.brlan", "G_remoConBtn_00"},
    {"basicPosition_00_out.brlan", "G_remoConBtn_01"},
    {"basicPosition_00_out.brlan", "G_remoConBtn_02"},
    {"basicPosition_00_out.brlan", "G_remoConBtn_03"},
    {"basicPosition_00_out.brlan", "G_remoConBtn_04"},
    {"basicPosition_00_out.brlan", "G_remoConBtn_05"},
    {"basicPosition_00_out.brlan", "G_remoConBtn_06"},
    {"basicPosition_00_out.brlan", "G_remoConBg_00"},
    {"basicPosition_00_out.brlan", "G_nunBtn_03"},
    {"basicPosition_00_out.brlan", "G_nunBtn_01"},
    {"basicPosition_00_out.brlan", "G_nunBtn_02"},
    {"basicPosition_00_out.brlan", "G_nunBg_00"},
    {"basicPosition_00_out.brlan", "G_shield_00"},
    {"basicPosition_00_out.brlan", "G_rupy_00"},
    {"basicPosition_00_out.brlan", "G_heart_00"},
    {"basicPosition_00_out.brlan", "G_ganbari_00"},

    {"basicPosition_00_position.brlan", "G_position_00"},
    {"basicPosition_00_rupyPosition.brlan", "G_rupyPosi_00"},
};

bool dLytMeter_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("basicPosition_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);
    for (int i = 0; i < 34; i++) {
        mAnmGroups[i].init(meterBrlanMap[i].mFile, resAcc, mLyt.getLayout(), meterBrlanMap[i].mName);
        mAnmGroups[i].setDirection(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    // in G_remoConBtn_06, out G_remoConBtn_06
    mABtn.setContainerGroups(&mAnmGroups[6], &mAnmGroups[22]);
    // out G_shield_00
    mShield.setContainerGroup(&mAnmGroups[28]);

    if (GLOBAL_TITLE_RELOADER == nullptr) {
        mp1Button = new dLytMeter1Button_c();
        mp2Button = new dLytMeter2Button_c();
    } else {
        mp1Button = nullptr;
        mp2Button = nullptr;
    }

    mNodes[0].mpMeter = &mGanbariGauge;
    mNodes[1].mpMeter = &mRupy;
    mNodes[2].mpMeter = &mItemSelect;
    mNodes[3].mpMeter = &mCrossBtn;
    mNodes[4].mpMeter = &mPlusBtn;
    mNodes[5].mpMeter = &mMinusBtn;
    mNodes[6].mpMeter = &mABtn;
    mNodes[7].mpMeter = &mDowsing;
    mNodes[8].mpMeter = &mZBtn;
    mNodes[9].mpMeter = &mNunStk;
    mNodes[10].mpMeter = &mRemoCon;
    mNodes[11].mpMeter = &mNunBg;
    mNodes[12].mpMeter = &mHeart;
    mNodes[13].mpMeter = &mShield;
    mNodes[14].mpMeter = mp1Button;
    mNodes[15].mpMeter = mp2Button;
    mPosArray1[0] = mLyt.findPane("N_shield_00")->GetTranslate();
    mPosArray1[1] = mLyt.findPane("N_shield_01")->GetTranslate();

    mPos1 = mPosArray1[0];
    mPos2 = mPosArray1[1];
    mLyt.findPane("N_rupyAll_00")->SetVisible(true);
    mAnmGroups[33].setAnimEnable(true);

    // Advance through some anim's keyframes and store positions in mPosArray2
    for (int i = 0; i < 7; i++) {
        mAnmGroups[33].setFrame(i);
        mLyt.calc();
        mPosArray2[i] = mLyt.findPane("N_rupy_00")->GetTranslate();
    }

    mAnmGroups[33].setAnimEnable(false);
    mLyt.findPane("N_rupyAll_00")->SetVisible(false);

    for (int i = 0; i < 16; i++) {
        if (mNodes[i].mpMeter != nullptr) {
            mNodes[i].mpMeter->build(resAcc);
            mMeters.PushBack(&mNodes[i]);
        }
    }
}

dLytMeterContainer_c::dLytMeterContainer_c() {
    sInstance = this;
}


bool dLytMeterContainer_c::build() {
    void *data = LayoutArcManager::sInstance->getLoadedData("DoButton");
    mResAcc.attach(data, "");
    if (GLOBAL_TITLE_RELOADER != nullptr) {
        mpEventSkip = nullptr;
    } else {
        mpEventSkip = new dLytMeterEventSkip_c();
    }

    if (mpEventSkip != nullptr) {
        mpEventSkip->build(&mResAcc);
    }

    dLytAreaCaption_c::create();
    mMeter.build(&mResAcc);
    if (GLOBAL_TITLE_RELOADER != nullptr) {
        mpDoButton = nullptr;
        mpDoButtonRelated = nullptr;
        mpBirdRelated = nullptr;
    } else {
        mpDoButton = new dLytDobutton_c();
        mpDoButtonRelated = new LytDoButtonRelated();
        mpBirdRelated = new LytBirdButtonRelated();
    }

    if (mpDoButton != nullptr) {
        mpDoButton->init(&mResAcc);
    }
    if (mpDoButtonRelated != nullptr) {
        mpDoButtonRelated->build(&mResAcc);
    }
    if (mpBirdRelated != nullptr) {
        mpBirdRelated->build(&mResAcc);
    }

    fn_800D97E0(0xb);
    fn_800D9800(1);
    fn_801B2D10(GLOBAL_MESSAGE_RELATED_CONTEXT);
}
