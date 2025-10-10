#include "d/lyt/d_lyt_area_caption.h"

#include "d/d_message.h"
#include "d/d_pad.h"
#include "d/d_sc_game.h"
#include "d/d_stage_mgr.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/event_manager.h"

STATE_DEFINE(dLytAreaCaptionParts_c, Wait);
STATE_DEFINE(dLytAreaCaptionParts_c, Start);
STATE_DEFINE(dLytAreaCaptionParts_c, Out);

static const d2d::LytBrlanMapping brlanMap1[] = {
    {   "areaCaption_00_start.brlan",   "G__start_00"},
    {    "areaCaption_00_type.brlan",     "G_type_00"},
    {"areaCaption_00_textLine.brlan", "G_textLine_00"},
    {                             "",              ""},
};

static const d2d::LytBrlanMapping brlanMap2[] = {
    {   "areaCaption_01_start.brlan",   "G__start_00"},
    {    "areaCaption_01_type.brlan",     "G_type_00"},
    {"areaCaption_01_textLine.brlan", "G_textLine_00"},
    {     "areaCaption_01_out.brlan",    "G_inOut_00"},
};

#define AREA_CAPTION_ANIM_START 0
#define AREA_CAPTION_ANIM_TYPE 1
#define AREA_CAPTION_ANIM_TEXTLINE 2
#define AREA_CAPTION_ANIM_OUT 3

#define AREA_CAPTION_NUM_ANIMS 4

static const char *captionBoxes[] = {
    "T_caption_00",
    "T_captionS_00",
    "T_captionS_01",
};

dLytAreaCaption_c *dLytAreaCaption_c::sInstance;

bool dLytAreaCaptionParts_c::init(int first) {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("DoButton");
    mResAcc.attach(data, "");
    mLyt.setResAcc(&mResAcc);

    mFirst = first;
    if (mFirst != 0) {
        mLyt.build("areaCaption_00.brlyt", nullptr);
        for (int i = 0; i < AREA_CAPTION_NUM_ANIMS; i++) {
            if (brlanMap1[i].mFile[0] == '\0') {
                mAnmCtrlInUse[i] = false;
            } else {
                mAnmGroups[i].init(brlanMap1[i].mFile, &mResAcc, mLyt.getLayout(), brlanMap1[i].mName);
                mAnmGroups[i].bind(false);
                mAnmGroups[i].setAnimEnable(false);
                mAnmCtrlInUse[i] = true;
            }
        }
    } else {
        mLyt.build("areaCaption_01.brlyt", nullptr);
        mLyt.setPriority(0x8A);
        for (int i = 0; i < AREA_CAPTION_NUM_ANIMS; i++) {
            if (brlanMap2[i].mFile[0] == '\0') {
                mAnmCtrlInUse[i] = false;
            } else {
                mAnmGroups[i].init(brlanMap2[i].mFile, &mResAcc, mLyt.getLayout(), brlanMap2[i].mName);
                mAnmGroups[i].bind(false);
                mAnmGroups[i].setAnimEnable(false);
                mAnmCtrlInUse[i] = true;
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        mTextBoxes[i] = mLyt.getTextBox(captionBoxes[i]);
    }

    field_0x54C = false;
    field_0x54D = false;
    mStateMgr.changeState(StateID_Wait);
    mLyt.calc();
    return true;
}

bool dLytAreaCaptionParts_c::remove() {
    for (int i = 0; i < AREA_CAPTION_NUM_ANIMS; i++) {
        if (mAnmCtrlInUse[i]) {
            mAnmGroups[i].unbind();
            mAnmGroups[i].remove();
        }
    }
    mResAcc.detach();
    return true;
}

bool dLytAreaCaptionParts_c::update() {
    mStateMgr.executeState();
    if (*mStateMgr.getStateID() != StateID_Wait) {
        for (int i = 0; i < AREA_CAPTION_NUM_ANIMS; i++) {
            if (mAnmCtrlInUse[i] && mAnmGroups[i].isEnabled()) {
                mAnmGroups[i].play();
            }
        }
        mLyt.calc();
    }
    return true;
}

bool dLytAreaCaptionParts_c::draw() {
    mLyt.addToDrawList();
    return true;
}

void dLytAreaCaptionParts_c::setTextIfWaiting(const char *text) {
    if (*mStateMgr.getStateID() == StateID_Wait) {
        mTextBoxes[0]->setMessageWithGlobalTextProcessor2(text, nullptr);
        mTextBoxes[1]->setMessageWithGlobalTextProcessor2(text, nullptr);
        mTextBoxes[2]->setMessageWithGlobalTextProcessor2(text, nullptr);
        field_0x54C = true;
    }
}

void dLytAreaCaptionParts_c::check() {
    if (*mStateMgr.getStateID() == StateID_Start) {
        field_0x54D = true;
    }
}

void dLytAreaCaptionParts_c::initializeState_Wait() {}
void dLytAreaCaptionParts_c::executeState_Wait() {
    if (field_0x54C) {
        field_0x54C = false;
        s32 typeFrame = 0;
        if (dStageMgr_c::GetInstance()->isSTIFAreaFaron()) {
            typeFrame = 1;
        } else if (dStageMgr_c::GetInstance()->isSTIFAreaEldin()) {
            typeFrame = 2;
        } else if (dStageMgr_c::GetInstance()->isSTIFAreaLanayru()) {
            typeFrame = 3;
        } else if (dStageMgr_c::GetInstance()->isSTIFAreaSealedGrounds()) {
            typeFrame = 4;
        } else if (dStageMgr_c::GetInstance()->isSTIFAreaSky()) {
            typeFrame = 5;
        }

        if (!mFirst && dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL) {
            typeFrame = 6;
        }

        s32 numLines = mTextBoxes[0]->calcTextLines(mTextBoxes[0]->GetString(), dMessage_c::getGlobalTagProcessor());
        f32 textFrame = 0.0f;
        if (numLines > 1) {
            textFrame = 1.0f;
        }

        if (!mFirst) {
            mLyt.findPane("N_inOut_00")->SetVisible(true);
        }

        mAnmGroups[AREA_CAPTION_ANIM_TYPE].setFrame(typeFrame);
        mAnmGroups[AREA_CAPTION_ANIM_TYPE].setAnimEnable(true);
        mAnmGroups[AREA_CAPTION_ANIM_TEXTLINE].setAnimEnable(true);
        mAnmGroups[AREA_CAPTION_ANIM_TEXTLINE].setFrame(textFrame);
        mLyt.calc();
        mAnmGroups[AREA_CAPTION_ANIM_TYPE].setAnimEnable(false);
        mAnmGroups[AREA_CAPTION_ANIM_TEXTLINE].setAnimEnable(false);
        mAnmGroups[AREA_CAPTION_ANIM_START].setAnimEnable(true);
        mAnmGroups[AREA_CAPTION_ANIM_START].setFrame(0.0f);
        mStateMgr.changeState(StateID_Start);
    }
}
void dLytAreaCaptionParts_c::finalizeState_Wait() {}

void dLytAreaCaptionParts_c::initializeState_Start() {
    if (mFirst == 0) {
        dLytAreaCaption_c::setVisible(true);
        dLytMeter_c::GetInstance()->setAreaCaptionOverrideVisibility(false);
        dLytMeter_c::GetMain()->setGanbariGaugeHiddenByAreaCaption(true);
    }
}
void dLytAreaCaptionParts_c::executeState_Start() {
    if (mAnmGroups[AREA_CAPTION_ANIM_START].isEndReached()) {
        mAnmGroups[AREA_CAPTION_ANIM_START].setAnimEnable(false);
        if (!mFirst) {
            dLytAreaCaption_c::setVisible(false);
            dLytMeter_c::GetInstance()->setAreaCaptionOverrideVisibility(true);
        }
        mStateMgr.changeState(StateID_Wait);
    } else {
        if (shouldImmediatelyDismiss()) {
            field_0x54D = false;
            if (mFirst) {
                mAnmGroups[AREA_CAPTION_ANIM_START].setFrame(0.0f);
                mLyt.calc();
                mAnmGroups[AREA_CAPTION_ANIM_START].setAnimEnable(false);
                mStateMgr.changeState(StateID_Wait);
            } else {
                mAnmGroups[AREA_CAPTION_ANIM_OUT].setAnimEnable(true);
                mAnmGroups[AREA_CAPTION_ANIM_OUT].setFrame(0.0f);
                mStateMgr.changeState(StateID_Out);
                dLytAreaCaption_c::setVisible(false);
                dLytMeter_c::GetInstance()->setAreaCaptionOverrideVisibility(true);
            }
        }
    }
}
void dLytAreaCaptionParts_c::finalizeState_Start() {}

void dLytAreaCaptionParts_c::initializeState_Out() {}
void dLytAreaCaptionParts_c::executeState_Out() {
    if (mAnmGroups[AREA_CAPTION_ANIM_OUT].isEndReached()) {
        mAnmGroups[AREA_CAPTION_ANIM_OUT].setAnimEnable(false);
        if (!mFirst) {
            dLytAreaCaption_c::setVisible(false);
            dLytMeter_c::GetInstance()->setAreaCaptionOverrideVisibility(true);
        }
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytAreaCaptionParts_c::finalizeState_Out() {}

bool dLytAreaCaptionParts_c::shouldImmediatelyDismiss() const {
    if (mFirst) {
        return field_0x54D;
    }

    if (field_0x54D) {
        return true;
    }

    if (dPad::getDownTrigB()) {
        return true;
    }

    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_C) && dPad::getDownTrigC()) {
        return true;
    }

    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_PLUS) && dPad::getDownTrigPlus()) {
        return true;
    }

    if (dPad::getDownTrigMinus()) {
        return true;
    }

    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_1) && dPad::getDownTrig1()) {
        return true;
    }

    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_2) && dPad::getDownTrig2()) {
        return true;
    }

    if (dPad::getDownTrigUp()) {
        return true;
    }

    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_CROSS_DOWN) && dPad::getDownTrigDown()) {
        return true;
    }

    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_CROSS_LEFT) && dPad::getDownTrigLeft()) {
        return true;
    }

    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_CROSS_RIGHT) && dPad::getDownTrigRight()) {
        return true;
    }

    if (EventManager::eventRelatedStateFlags_shift0x11_1()) {
        return true;
    }

    return false;
}

bool dLytAreaCaption_c::create() {
    new dLytAreaCaption_c();
    if (dLytAreaCaption_c::sInstance == nullptr) {
        return false;
    }
    sInstance->init();
    return true;
}

bool dLytAreaCaption_c::remove() {
    if (sInstance != nullptr) {
        sInstance->deinit();
        delete sInstance;
        sInstance = nullptr;
    }
    return true;
}

bool dLytAreaCaption_c::update() {
    if (sInstance != nullptr) {
        sInstance->doUpdate();
    }
    return true;
}

bool dLytAreaCaption_c::draw() {
    if (sInstance != nullptr) {
        sInstance->doDraw();
    }
    return true;
}

void dLytAreaCaption_c::setTextIfWaiting(const char *text) {
    if (sInstance != nullptr) {
        sInstance->doSetTextIfWaiting(text);
    }
}

void dLytAreaCaption_c::setArea(int id) {
    if (sInstance != nullptr) {
        sInstance->doSetArea(id);
    }
}

void dLytAreaCaption_c::check() {
    if (sInstance != nullptr) {
        sInstance->doCheck();
    }
}

void dLytAreaCaption_c::setVisible(bool arg) {
    if (sInstance != nullptr) {
        sInstance->mVisible = arg;
    }
}

bool dLytAreaCaption_c::getVisible() {
    if (sInstance != nullptr) {
        return sInstance->mVisible;
    }
    return 0;
}

bool dLytAreaCaption_c::init() {
    int first = 1;
    for (int i = 0; i < 2; i++) {
        mParts[i].init(first);
        first = 0;
    }
    mPartToDraw = 0;
    mVisible = false;
    return true;
}

bool dLytAreaCaption_c::deinit() {
    for (int i = 0; i < 2; i++) {
        mParts[i].remove();
    }
    return true;
}

bool dLytAreaCaption_c::doUpdate() {
    for (int i = 0; i < 2; i++) {
        mParts[i].update();
    }
    return true;
}

bool dLytAreaCaption_c::doDraw() {
    for (int i = 0; i < 2; i++) {
        if (i == mPartToDraw) {
            mParts[i].draw();
        }
    }
    return true;
}

void dLytAreaCaption_c::doSetTextIfWaiting(const char *text) {
    mPartToDraw = 0;
    for (int i = 0; i < 2; i++) {
        mParts[i].setTextIfWaiting(text);
    }
}

void dLytAreaCaption_c::doSetArea(int id) {
    mPartToDraw = 1;
    SizedString<16> str;
    str.sprintf("MAP_%02d", id);
    for (int i = 0; i < 2; i++) {
        mParts[i].setTextIfWaiting(str);
    }
}

void dLytAreaCaption_c::doCheck() {
    for (int i = 0; i < 2; i++) {
        mParts[i].check();
    }
}
