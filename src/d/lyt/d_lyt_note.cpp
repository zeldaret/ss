#include "d/lyt/d_lyt_note.h"

#include "common.h"
#include "m/m_mtx.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/math/math_types.h"

STATE_DEFINE(dLytNote_c, Wait);
STATE_DEFINE(dLytNote_c, In);
STATE_DEFINE(dLytNote_c, Move);
STATE_DEFINE(dLytNote_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {     "suiryuScore_00_noteGet.brlan",  "G_mark_00"},
    { "suiryuScore_00_noteGetLoop.brlan",  "G_mark_00"},
    {"suiryuScore_00_noteLostLoop.brlan",  "G_mark_00"},
    {    "suiryuScore_00_noteLost.brlan",  "G_mark_00"},
    {     "suiryuScore_00_noteFix.brlan",  "G_mark_00"},
    {   "suiryuScore_00_noteColor.brlan", "G_color_00"},
};

#define NOTE_ANIM_GET 0
#define NOTE_ANIM_GET_LOOP 1
#define NOTE_ANIM_LOST_LOOP 2
#define NOTE_ANIM_LOST 3
#define NOTE_ANIM_FIX 4
#define NOTE_ANIM_COLOR 5
#define NOTE_ANIM_INVALID 6

void dLytNote_c::initializeState_Wait() {
    field_0x273 = 0;
}

void dLytNote_c::executeState_Wait() {
    if (field_0x270 != 0) {
        mStateMgr.changeState(StateID_In);
    }
}

void dLytNote_c::finalizeState_Wait() {}

void dLytNote_c::initializeState_In() {
    field_0x274 = 0;
    mpPane->SetVisible(true);
    mAnmGroups[NOTE_ANIM_GET].setAnimEnable(true);
    mAnmGroups[NOTE_ANIM_GET].setFrame(0.0f);
}

void dLytNote_c::executeState_In() {
    if (field_0x270 == 0) {
        mAnmGroups[NOTE_ANIM_GET].setToEnd();
        mAnmGroups[NOTE_ANIM_GET].setAnimEnable(false);
        mStateMgr.changeState(StateID_Out);
    } else if (mAnmGroups[NOTE_ANIM_GET].isEndReached()) {
        mAnmGroups[NOTE_ANIM_GET].setAnimEnable(false);
        field_0x260 = 0.0f;
        mInAnim = NOTE_ANIM_GET_LOOP;
        mAnmGroups[mInAnim].setAnimEnable(true);
        mAnmGroups[mInAnim].setFrame(0.0f);
        mStateMgr.changeState(StateID_Move);
    }

    if (mAnmGroups[NOTE_ANIM_GET].isEnabled()) {
        mAnmGroups[NOTE_ANIM_GET].play();
    }
}

void dLytNote_c::finalizeState_In() {}

void dLytNote_c::initializeState_Move() {
    field_0x273 = 1;
}

void dLytNote_c::executeState_Move() {
    if (field_0x270 == 0) {
        mAnmGroups[mInAnim].setAnimEnable(false);
        mStateMgr.changeState(StateID_Out);
    } else if (field_0x272 != 0 && mInAnim == NOTE_ANIM_GET_LOOP) {
        mAnmGroups[mInAnim].setAnimEnable(false);
        field_0x260 = 0.0f;
        mInAnim = NOTE_ANIM_LOST_LOOP;
        // Maybe an inline?
        f32 progress = mAnmGroups[mInAnim].getEndFrameRaw();
        f32 nul = 0.0f;
        if (nul >= progress) {
            field_0x260 = nul - progress;
        }
        mAnmGroups[mInAnim].setFrame(field_0x260);
        mAnmGroups[mInAnim].setAnimEnable(true);
    } else {
        if (field_0x272 == 0 && mInAnim == NOTE_ANIM_LOST_LOOP) {
            mAnmGroups[mInAnim].setAnimEnable(false);
            field_0x260 = 0.0f;
            mInAnim = NOTE_ANIM_GET_LOOP;
            // Maybe an inline?
            f32 progress = mAnmGroups[mInAnim].getEndFrameRaw();
            f32 nul = 0.0f;
            if (nul >= progress) {
                field_0x260 = nul - progress;
            }
            mAnmGroups[mInAnim].setFrame(field_0x260);
            mAnmGroups[mInAnim].setAnimEnable(true);
        }
    }
}

void dLytNote_c::finalizeState_Move() {}

void dLytNote_c::initializeState_Out() {
    mInAnim = NOTE_ANIM_INVALID;
    field_0x273 = 0;
    if (field_0x271) {
        mOutAnim = NOTE_ANIM_FIX;
    } else {
        mOutAnim = NOTE_ANIM_LOST;
    }
    mAnmGroups[mOutAnim].setAnimEnable(true);
    mAnmGroups[mOutAnim].setFrame(0.0f);
}

void dLytNote_c::executeState_Out() {
    if (field_0x274 != 0) {
        mAnmGroups[mOutAnim].setToEnd();
    }

    if (mAnmGroups[mOutAnim].isEndReached()) {
        mAnmGroups[mOutAnim].setAnimEnable(false);
        mpPane->SetVisible(false);
        mStateMgr.changeState(StateID_Wait);
    }

    if (mAnmGroups[mOutAnim].isEnabled()) {
        mAnmGroups[mOutAnim].play();
    }
}

void dLytNote_c::finalizeState_Out() {}

bool dLytNote_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("suiryuScore_00.brlyt", nullptr);

    for (int i = 0; i < 6; i++) {
        mAnmGroups[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnmGroups[i].setDirection(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    mAnmGroups[NOTE_ANIM_COLOR].setAnimEnable(true);
    static const char *sPaneName = "N_inOutAll_00";
    mpPane = mLyt.findPane(sPaneName);
    mpPane->SetVisible(false);
    const nw4r::math::VEC3 &translate = mpPane->GetTranslate();
    mTranslate.copyFrom(translate);
    mInAnim = NOTE_ANIM_INVALID;
    mOutAnim = NOTE_ANIM_FIX;
    field_0x260 = 0.0f;
    field_0x270 = 0;
    field_0x271 = 0;
    field_0x272 = 0;
    field_0x273 = 0;
    field_0x274 = 0;
    field_0x26C = 0;
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytNote_c::remove() {
    for (int i = 0; i < 6; i++) {
        // @bug? .unbind call is missing
        mAnmGroups[i].afterUnbind();
    }
    return true;
}

void dLytNote_c::execute(u8 alpha) {
    mStateMgr.executeState();
    if (*mStateMgr.getStateID() != StateID_Wait) {
        mpPane->SetTranslate(mTranslate);
        mAnmGroups[NOTE_ANIM_COLOR].setFrame(field_0x26C);
        mpPane->SetAlpha(alpha);
        mLyt.calc();
    }

    if (mInAnim != NOTE_ANIM_INVALID) {
        field_0x260 += 1.0f;
        f32 nextFrame = mAnmGroups[mInAnim].getEndFrameRaw();
        if (field_0x260 >= nextFrame) {
            field_0x260 -= nextFrame;
        }
    }
}

void dLytNote_c::draw() {
    mLyt.addToDrawList();
}

void dLytNote_c::setTransform(nw4r::lyt::Pane &p) {
    // Cool, full on struct copy here
    nw4r::math::MTX34 myMtx = p.GetGlobalMtx();
    mTranslate.x = myMtx.m[0][3];
    mTranslate.y = myMtx.m[1][3];
    mTranslate.z = 0.0f;
}

f32 dLytNote_c::getField0x260() const {
    return field_0x260;
}

void dLytNote_c::setFrame(f32 arg) {
    if (mInAnim != 6) {
        field_0x260 = arg;
        // ???
        f32 newArg = (f64)arg;
        mAnmGroups[mInAnim].setFrame(newArg);
    }
}
