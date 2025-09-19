#include "d/lyt/d_lyt_save_msg_window.h"

#include "common.h"
#include "d/d_message.h"
#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "nw4r/lyt/lyt_group.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

STATE_DEFINE(dLytSaveMsgWindow_c, Init);
STATE_DEFINE(dLytSaveMsgWindow_c, In);
STATE_DEFINE(dLytSaveMsgWindow_c, Wait);
STATE_DEFINE(dLytSaveMsgWindow_c, Out);

dLytSaveMsgWindow_c::dLytSaveMsgWindow_c() : mStateMgr(*this, sStateID::null) {
    resetProperties();
}

static const d2d::LytBrlanMapping brlanMap[] = {
    {         "save_00_in.brlan",       "G_inOut_00"},
    {         "save_00_in.brlan",       "G_inOut_01"},
    {        "save_00_bgV.brlan",         "G_bgV_00"},
    {      "save_00_textV.brlan",       "G_textV_00"},
    {"save_00_winPosition.brlan", "G_winPosition_00"},
    {        "save_00_out.brlan",       "G_inOut_00"},
    {        "save_00_out.brlan",       "G_inOut_01"},
    {     "save_00_textIn.brlan",   "G_textInOut_00"},
    {    "save_00_textOut.brlan",   "G_textInOut_00"},
};

#define SAVE_ANIM_IN_0 0
#define SAVE_ANIM_IN_1 1
#define SAVE_ANIM_BG_V 2
#define SAVE_ANIM_TEXT_V 3
#define SAVE_ANIM_WIN_POSITION 4
#define SAVE_ANIM_OUT_0 5
#define SAVE_ANIM_OUT_1 6
#define SAVE_ANIM_TEXT_IN 7
#define SAVE_ANIM_TEXT_OUT 8

#define SAVE_NUM_ANIMS 9

static const char *sGroupName = "G_ref_00";

bool dLytSaveMsgWindow_c::build(bool arg) {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("System2D");
    mResAcc.attach(data, "");
    mLyt.build("save_00.brlyt", &mResAcc);

    for (int i = 0; i < SAVE_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, &mResAcc, mLyt.getLayout(), brlanMap[i].mName);
    }

    mLyt.setPriority(0x8A);

    for (int i = 0; i < 1; i++) {
        mSubpanes[i].mpLytPane = &mBtn;
        mBtn.build(&mResAcc);
        mSubpaneList.PushBack(&mSubpanes[i]);
    }

    if (mLyt.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *g = mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mSubpaneList);
        }
    }

    mBtn.setFlipBtnLayout(1);
    if (arg) {
        mBtn.setSkipOutAnim(1);
    }
    mBtn.setInSound(1);

    mStateMgr.changeState(StateID_Init);
    mLineSpace = mLyt.getTextBox("T_text_00")->GetLineSpace();
    field_0x10DC = 1;
    return true;
}

bool dLytSaveMsgWindow_c::remove() {
    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    for (int i = 0; i < 1; i++) {
        mSubpanes[i].mpLytPane->remove();
    }
    mLyt.unbindAnims();
    for (int i = 0; i < SAVE_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    mResAcc.detach();
    return true;
}

bool dLytSaveMsgWindow_c::execute() {
    for (int i = 0; i < 1; i++) {
        mSubpanes[i].mpLytPane->execute();
    }
    mStateMgr.executeState();
    mLyt.calc();
    return true;
}

bool dLytSaveMsgWindow_c::draw() {
    if (field_0x10D8 != 1) {
        return true;
    }
    mLyt.addToDrawList();
    return true;
}

bool dLytSaveMsgWindow_c::drawNow() {
    if (field_0x10D8 != 1) {
        return true;
    }
    mLyt.draw();
    return true;
}

void dLytSaveMsgWindow_c::reset() {
    resetProperties();
    mStateMgr.changeState(StateID_Init);
}

bool dLytSaveMsgWindow_c::setProperties(const char *message, s32 a1, s32 a2, u8 a3) {
    mLabel = message;
    field_0x10B0 = a1;
    field_0x10B4 = a2;
    field_0x10DD = a3;
    field_0x10DA = 1;
    mNumLines = 1;
    field_0x10B8 = 0;
    mSaveobjId = -1;
    return true;
}

bool dLytSaveMsgWindow_c::checkIsWait() {
    if (*mStateMgr.getStateID() == StateID_Wait) {
        field_0x10DB = 1;
        return true;
    }
    return false;
}

void dLytSaveMsgWindow_c::initializeState_Init() {
    resetProperties();
    for (int i = 0; i < SAVE_NUM_ANIMS; i++) {
        mAnm[i].unbind();
    }
}
void dLytSaveMsgWindow_c::executeState_Init() {
    if (field_0x10DA != 1) {
        return;
    }
    field_0x10DA = 0;
    mStateMgr.changeState(StateID_In);
}
void dLytSaveMsgWindow_c::finalizeState_Init() {}

void dLytSaveMsgWindow_c::initializeState_In() {
    field_0x10D8 = 1;
    if (field_0x10B8 == 1) {
        setAnmFrame(SAVE_ANIM_TEXT_IN, 0.0f);
    } else {
        setAnmFrame(SAVE_ANIM_IN_0, 0.0f);
        setAnmFrame(SAVE_ANIM_IN_1, 0.0f);
    }
    setupSaveTextMaybe();
    f32 bgV = 0.0f;
    if (field_0x10DD == 1) {
        bgV = 1.0f;
    }
    setAnmFrame(SAVE_ANIM_BG_V, bgV);
    f32 textV = 0.0f;
    if (mNumLines != 1) {
        textV = 1.0f;
    }
    setAnmFrame(SAVE_ANIM_TEXT_V, textV);
    setAnmFrame(SAVE_ANIM_WIN_POSITION, field_0x10B4);
    field_0x10DC = 0;
}
void dLytSaveMsgWindow_c::executeState_In() {
    if (field_0x10C0 != 0) {
        return;
    }
    s32 group = SAVE_ANIM_IN_0;
    if (field_0x10B8 == 1) {
        group = SAVE_ANIM_TEXT_IN;
    }
    d2d::AnmGroup_c &grp = mAnm[group];
    if (grp.isEndReached() == true) {
        updateSaveTextMaybe();
        mStateMgr.changeState(StateID_Wait);
    }
    grp.play();
    if (field_0x10B8 != 1) {
        mAnm[SAVE_ANIM_IN_1].play();
    }
}
void dLytSaveMsgWindow_c::finalizeState_In() {
    if (field_0x10B8 == 1) {
        unbindAnm(SAVE_ANIM_TEXT_IN);
    } else {
        unbindAnm(SAVE_ANIM_IN_0);
        unbindAnm(SAVE_ANIM_IN_1);
    }
}

void dLytSaveMsgWindow_c::initializeState_Wait() {
    if (field_0x10B0 == 0) {
        field_0x10C0 = 10;
    } else {
        field_0x10C0 = 0;
    }
}
void dLytSaveMsgWindow_c::executeState_Wait() {
    if (field_0x10C0 == 0) {
        field_0x10C4 = mBtn.getDecidedBtnIdx();
        field_0x10C8 = mBtn.getConfirmedBtnIdx();
    }
    if (field_0x10DB == 1) {
        field_0x10DB = 0;
        mStateMgr.changeState(StateID_Out);
    }
    if (field_0x10DA == 1) {
        field_0x10DA = 0;
        field_0x10C4 = -1;
        field_0x10C8 = -1;
        mStateMgr.changeState(StateID_In);
    }
}
void dLytSaveMsgWindow_c::finalizeState_Wait() {}

void dLytSaveMsgWindow_c::initializeState_Out() {
    field_0x10C0 = 0;
    if (field_0x10BC == 1) {
        setAnmFrame(SAVE_ANIM_TEXT_OUT, 0.0f);
    } else if (field_0x10BC == 2) {
        setAnmFrame(SAVE_ANIM_OUT_1, 0.0f);
    } else {
        setAnmFrame(SAVE_ANIM_OUT_0, 0.0f);
        setAnmFrame(SAVE_ANIM_OUT_1, 0.0f);
    }
}
void dLytSaveMsgWindow_c::executeState_Out() {
    s32 group = SAVE_ANIM_OUT_0;
    if (field_0x10BC == 1) {
        group = SAVE_ANIM_TEXT_OUT;
    } else if (field_0x10BC == 2) {
        group = SAVE_ANIM_OUT_1;
    }
    d2d::AnmGroup_c &grp = mAnm[group];
    switch (field_0x10C0) {
        case 0:
            if (grp.isEndReached() == true) {
                mWillFinishOut = 1;
                field_0x10C0 = 1;
            }
            break;
        case 1:
            if (field_0x10BC == 1) {
                mStateMgr.changeState(StateID_Wait);
            } else {
                mStateMgr.changeState(StateID_Init);
            }
            return;
    }

    grp.play();
    if (field_0x10BC == 0) {
        mAnm[SAVE_ANIM_OUT_1].play();
    }
}
void dLytSaveMsgWindow_c::finalizeState_Out() {
    if (!mBtn.isVisible()) {
        mLyt.findPane("N_messageBtn_00")->SetVisible(false);
    }
    mWillFinishOut = 0;
    if (field_0x10BC == 1) {
        unbindAnm(SAVE_ANIM_TEXT_OUT);
    } else if (field_0x10BC == 2) {
        unbindAnm(SAVE_ANIM_OUT_1);
    } else {
        unbindAnm(SAVE_ANIM_OUT_0);
        unbindAnm(SAVE_ANIM_OUT_1);
    }
}

void dLytSaveMsgWindow_c::resetProperties() {
    field_0x10D8 = 0;
    mWillFinishOut = 0;
    field_0x10DA = 0;
    field_0x10DB = 0;
    field_0x10C4 = -1;
    field_0x10C8 = -1;
    field_0x10C0 = 0;
    field_0x10B0 = 4;
    field_0x10B4 = 4;
    field_0x10B8 = 0;
    field_0x10BC = 0;
    mSaveobjId = -1;
}

void dLytSaveMsgWindow_c::setAnmFrame(s32 group, f32 frame) {
    d2d::AnmGroup_c &anm = mAnm[group];
    anm.bind(false);
    anm.setAnimEnable(true);
    anm.setFrame(frame);
}

void dLytSaveMsgWindow_c::unbindAnm(s32 group) {
    d2d::AnmGroup_c &anm = mAnm[group];
    anm.unbind();
}

void dLytSaveMsgWindow_c::updateSaveTextMaybe() {
    // Redundant check, this is also checked internally
    if (field_0x10B0 != 0) {
        updateSaveText();
    }
}

void dLytSaveMsgWindow_c::setupSaveTextMaybe() {
    const wchar_t *text = dMessage_c::getTextMessageByLabel(mLabel, true, nullptr, 0);
    dTextBox_c *box = mLyt.getTextBox("T_text_00");
    dTagProcessor_c *tagProcessor = dMessage_c::getGlobalTagProcessor();
    if (mSaveobjId != -1) {
        SizedString<32> name;
        if (mSaveobjId == 0xFF) {
            name = "SAVEOBJ_NAME_UNKNOWN";
        } else {
            name.sprintf("SAVEOBJ_NAME_%02d", mSaveobjId);
        }

        if (tagProcessor != nullptr) {
            const wchar_t *objMsg = dMessage_c::getTextMessageByLabel(name, true, nullptr, 0);
            if (objMsg != nullptr) {
                tagProcessor->setStringArg(objMsg, 0);
            }
        }
    }
    mNumLines = box->calcTextLines(text, tagProcessor);
    if (mNumLines == 1) {
        box->SetLineSpace(0.0f);
    } else {
        box->SetLineSpace(mLineSpace);
    }
    box->setTextWithGlobalTextProcessor(text);
    dTextBox_c *boxShadow = mLyt.getTextBox("T_textS_00");
    if (mNumLines == 1) {
        boxShadow->SetLineSpace(0.0f);
    } else {
        boxShadow->SetLineSpace(mLineSpace);
    }
    boxShadow->setTextWithGlobalTextProcessor(text);
    mLyt.findPane("N_messageBtn_00")->SetVisible(false);
}

void dLytSaveMsgWindow_c::updateSaveText() {
    if (field_0x10B0 == 0) {
        return;
    }
    mLyt.findPane("N_messageBtn_00")->SetVisible(true);
    s32 numOptions = 2;
    if (field_0x10B0 == 2) {
        numOptions = 3;
    } else if (field_0x10B0 == 3) {
        numOptions = 4;
    }
    for (s32 i = 0; i < numOptions; i++) {
        const wchar_t *text = dMessage_c::getGlobalTagProcessor()->getOptionString(i);
        mBtn.getSelectTextBox(i, 0)->setTextWithGlobalTextProcessor(text);
        mBtn.getSelectTextBox(i, 1)->setTextWithGlobalTextProcessor(text);
    }

    if (field_0x10B0 != 0) {
        mBtn.setTagProcessor(dMessage_c::getGlobalTagProcessor());
        static const u32 sInts[] = {0, 2, 3, 4};
        u32 numOptions = sInts[field_0x10B0];
        s32 f = dMessage_c::getGlobalTagProcessor()->getCancelBtnIdx();
        mBtn.setCancelBtnIdx(f);
        mBtn.setField_0x990(f);
        mBtn.requestIn(numOptions, true);
    }
}
