#include "d/lyt/d_lyt_area_caption.h"

#include "d/lyt/d_textbox.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"


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
        for (int i = 0; i < 4; i++) {
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
        for (int i = 0; i < 4; i++) {
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

    field_0x54C = 0;
    field_0x54D = 0;
    mStateMgr.changeState(StateID_Wait);
    mLyt.calc();
    return true;
}

bool dLytAreaCaptionParts_c::remove() {
    for (int i = 0; i < 4; i++) {
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
        for (int i = 0; i < 4; i++) {
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
        mTextBoxes[0]->fn_800E0A60(text, nullptr);
        mTextBoxes[1]->fn_800E0A60(text, nullptr);
        mTextBoxes[2]->fn_800E0A60(text, nullptr);
        field_0x54C = 1;
    }
}

void dLytAreaCaptionParts_c::check() {
    if (*mStateMgr.getStateID() == StateID_Start) {
        field_0x54D = 1;
    }
}

void dLytAreaCaptionParts_c::initializeState_Wait() {}
void dLytAreaCaptionParts_c::executeState_Wait() {
    mTextBoxes[0]->fn_800E0A60("N_inOut_00", nullptr);
}
void dLytAreaCaptionParts_c::finalizeState_Wait() {}

void dLytAreaCaptionParts_c::initializeState_Start() {}
void dLytAreaCaptionParts_c::executeState_Start() {}
void dLytAreaCaptionParts_c::finalizeState_Start() {}

void dLytAreaCaptionParts_c::initializeState_Out() {}
void dLytAreaCaptionParts_c::executeState_Out() {}
void dLytAreaCaptionParts_c::finalizeState_Out() {}

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

void dLytAreaCaption_c::set0xAAC(u8 arg) {
    if (sInstance != nullptr) {
        sInstance->field_0xAAC = arg;
    }
}

u8 dLytAreaCaption_c::get0xAAC() {
    if (sInstance != nullptr) {
        return sInstance->field_0xAAC;
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
    field_0xAAC = 0;
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
