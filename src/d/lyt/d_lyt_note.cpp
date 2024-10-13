#include <d/lyt/d_lyt_note.h>

STATE_DEFINE(dLytNote_c, Wait);
STATE_DEFINE(dLytNote_c, In);
STATE_DEFINE(dLytNote_c, Move);
STATE_DEFINE(dLytNote_c, Out);

void dLytNote_c::initializeState_Wait() {
    field_0x273 = 0;
}
void dLytNote_c::executeState_Wait() {
    if (field_0x270 != 0) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytNote_c::finalizeState_Wait() {}

void dLytNote_c::initializeState_In() {}
void dLytNote_c::executeState_In() {}
void dLytNote_c::finalizeState_In() {}

void dLytNote_c::initializeState_Move() {
    field_0x273 = 1;
}
void dLytNote_c::executeState_Move() {}
void dLytNote_c::finalizeState_Move() {}

void dLytNote_c::initializeState_Out() {}
void dLytNote_c::executeState_Out() {}
void dLytNote_c::finalizeState_Out() {}

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
    // FPR Regswap
    const nw4r::math::VEC3 &translate = mpPane->GetTranslate();
    mTranslate.set(translate.x, translate.y, translate.z);
    field_0x268 = 6;
    field_0x264 = 4;
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

void dLytNote_c::execute() {
    // TODO
}

void dLytNote_c::draw() {
    mLyt.addToDrawList();
}

f32 dLytNote_c::getField0x260() const {
    return field_0x260;
}

void dLytNote_c::setFrame(f32 arg) {
    if (field_0x268 != 6) {
        // mysterious frsp here
        field_0x260 = arg;
        mAnmGroups[field_0x268].setFrame(arg);
    }
}
