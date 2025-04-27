#include "d/lyt/d_lyt_common_arrow.h"

#include "d/d_cs_base.h"
#include "d/d_cursor_hit_check.h"
#include "rvl/MTX/mtx.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/small_sound_mgr.h"

STATE_DEFINE(dLytCommonArrow_c, None);
STATE_DEFINE(dLytCommonArrow_c, In);
STATE_DEFINE(dLytCommonArrow_c, Wait);
STATE_DEFINE(dLytCommonArrow_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {    "commonArrow_00_in.brlan",  "G_inOut_00"},
    {  "commonArrow_00_loop.brlan",   "G_loop_00"},
    {  "commonArrow_00_type.brlan",   "G_type_00"},
    { "commonArrow_00_onOff.brlan", "G_arrowL_00"},
    { "commonArrow_00_onOff.brlan", "G_arrowR_00"},
    {"commonArrow_00_decide.brlan", "G_arrowL_00"},
    {"commonArrow_00_decide.brlan", "G_arrowR_00"},
    { "commonArrow_00_input.brlan",  "G_input_00"},
    {   "commonArrow_00_out.brlan",  "G_inOut_00"},
};

#define ANIM_IN 0
#define ANIM_LOOP 1
#define ANIM_TYPE 2
#define ANIM_ONOFF_L 3
#define ANIM_ONOFF_R 4

#define ANIM_DECIDE_OFFSET 5

#define ANIM_DECIDE_L 5
#define ANIM_DECIDE_R 6
#define ANIM_INPUT 7
#define ANIM_OUT 8

dLytCommonArrow_c::dLytCommonArrow_c() : mStateMgr(*this, sStateID::null) {}

bool dLytCommonArrow_c::build() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("CommonArrow");
    mResAcc.attach(data, "");
    mLytBase.build("commonArrow_00.brlyt", &mResAcc);
    mLytBase.mPriority = 0x86;

    for (int i = 0; i < 9; i++) {
        mAnmGroups[i].init(brlanMap[i].mFile, &mResAcc, mLytBase.getLayout(), brlanMap[i].mName);
    }
    mCsHitCheck.init(mLytBase.getLayout()->GetRootPane(), 1, 0, 0);
    dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitCheck);
    mBoundingL = mLytBase.findBounding("B_arrowL_00");
    mBoundingR = mLytBase.findBounding("B_arrowR_00");
    mStateMgr.changeState(StateID_None);
    setState(0);
    return true;
}

bool dLytCommonArrow_c::remove() {
    dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitCheck);
    mLytBase.unbindAnims();
    for (int i = 0; i < 9; i++) {
        mAnmGroups[i].remove();
    }
    return true;
}

bool dLytCommonArrow_c::execute() {
    mStateMgr.executeState();
    if (!mStateMgr.getStateID()->isEqual(StateID_None)) {
        mAnmGroups[ANIM_LOOP].play();
    }
    mLytBase.calc();
    mCsHitCheck.resetCachedHitboxes();
    mCsHitCheck.execute();
    field_0x6BC = field_0x6B8;
    return true;
}

bool dLytCommonArrow_c::draw() {
    if (field_0x6CB == 1) {
        mLytBase.addToDrawList();
    }
    return true;
}

void dLytCommonArrow_c::setState(s32 state) {
    mType = state;
    if (state == 0) {
        mLytBase.mPriority = 0x86;
    } else {
        mLytBase.mPriority = 0x80;
    }
}

void dLytCommonArrow_c::setTranslate(const Vec *arg) {
    nw4r::math::VEC3 v(*arg);
    v.z = 0.0f;
    mLytBase.getLayout()->GetRootPane()->SetTranslate(v);
}

bool dLytCommonArrow_c::requestIn() {
    if (!mStateMgr.getStateID()->isEqual(StateID_None)) {
        return false;
    }
    mInRequested = 1;
    return true;
}

bool dLytCommonArrow_c::requestOut() {
    if (!mStateMgr.getStateID()->isEqual(StateID_Wait)) {
        return false;
    }
    mOutRequested = 1;
    return true;
}

bool dLytCommonArrow_c::fn_80168760() {
    if (field_0x6C0 == 2 && field_0x6B8 != 2) {
        field_0x6C0 = field_0x6B8;
        return true;
    }
    return false;
}

void dLytCommonArrow_c::displayElement(s32 idx, f32 frame) {
    d2d::AnmGroup_c *s = &mAnmGroups[idx];
    s->bind(false);
    s->setAnimEnable(true);
    s->setFrame(frame);
}

void dLytCommonArrow_c::unbindAt(s32 idx) {
    mAnmGroups[idx].unbind();
}

void dLytCommonArrow_c::tickDown(d2d::AnmGroup_c *ctrl) {
    if (ctrl->getFrame()) {
        f32 newFrame = ctrl->getFrame() - 1.0f;
        if (newFrame <= 0.0f) {
            newFrame = 0.0f;
        }
        ctrl->setFrame(newFrame);
    }
}

extern "C" u8 lbl_80572D10;

void dLytCommonArrow_c::fn_80168880() {
    int i = -1;
    if (lbl_80572D10 == 0) {
        field_0x6B4 = 2;
        return;
    }

    dCursorHitCheck_c *d = dCsBase_c::GetInstance()->getHitCheck();
    if (d != nullptr && d->getType() == 'lyt ') {
        if (static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == mBoundingL) {
            i = 0;
        } else if (static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == mBoundingR) {
            i = 1;
        }
    }

    if (i >= 0) {
        field_0x6B4 = i;
    } else {
        field_0x6B4 = 2;
    }
}

void dLytCommonArrow_c::initializeState_None() {
    mLytBase.unbindAnims();
    mInRequested = 0;
    mOutRequested = 0;
    field_0x6CA = 0;
    field_0x6CB = 0;
    field_0x6B4 = 2;
    field_0x6B8 = 2;
    field_0x6BC = 2;
    field_0x6C0 = 2;
    mTimer = 0;
    displayElement(ANIM_IN, 0.0f);
    field_0x6CC = 1;
}
void dLytCommonArrow_c::executeState_None() {
    if (mInRequested == 1) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytCommonArrow_c::finalizeState_None() {
    mInRequested = 0;
    displayElement(ANIM_LOOP, 0.0f);
}

void dLytCommonArrow_c::initializeState_In() {
    mTimer = 0;
    field_0x6CB = 1;
    f32 anmType = 0.0f;
    if (mType == 1) {
        anmType = 1.0f;
    }
    displayElement(ANIM_TYPE, anmType);
    displayElement(ANIM_ONOFF_L, 0.0f);
    displayElement(ANIM_ONOFF_R, 0.0f);
    displayElement(ANIM_INPUT, 0.0f);
}
void dLytCommonArrow_c::executeState_In() {
    switch (mTimer) {
        case 0: {
            d2d::AnmGroup_c *s = &mAnmGroups[ANIM_IN];
            if (s->isEndReached() == 1) {
                mTimer += 1;
                field_0x6CA = 1;
            } else {
                s->play();
            }
        } break;
        case 1:
            field_0x6CA = 0;
            mStateMgr.changeState(StateID_Wait);
            break;
    }
}
void dLytCommonArrow_c::finalizeState_In() {
    unbindAt(ANIM_IN);
}

void dLytCommonArrow_c::initializeState_Wait() {
    mTimer = 0;
}

void dLytCommonArrow_c::executeState_Wait() {
    if (mOutRequested == 1) {
        mStateMgr.changeState(StateID_Out);
        return;
    }

    fn_80168880();
    if (field_0x6B8 == 0) {
        d2d::AnmGroup_c &g = mAnmGroups[ANIM_ONOFF_L];
        g.play();
        tickDown(&mAnmGroups[ANIM_ONOFF_R]);
    } else if (field_0x6B8 == 1) {
        d2d::AnmGroup_c &g = mAnmGroups[ANIM_ONOFF_R];
        g.play();
        tickDown(&mAnmGroups[ANIM_ONOFF_L]);
    } else {
        tickDown(&mAnmGroups[ANIM_ONOFF_L]);
        tickDown(&mAnmGroups[ANIM_ONOFF_R]);
    }

    d2d::AnmGroup_c &g2 = mAnmGroups[ANIM_INPUT];
    if (field_0x6CC != 0) {
        tickDown(&g2);
    } else {
        g2.play();
    }

    switch (mTimer) {
        case 0:
            if (field_0x6C0 != 2) {
                displayElement(field_0x6C0 + ANIM_DECIDE_OFFSET, 0.0f);
                if (field_0x6C0 == 0) {
                    SmallSoundManager::GetInstance()->playSound(SE_S_MENU_SELECT_TURN_PAGE_LEFT);
                } else {
                    SmallSoundManager::GetInstance()->playSound(SE_S_MENU_SELECT_TURN_PAGE_RIGHT);
                }
                mTimer++;
            }
            break;
        case 1: {
            d2d::AnmGroup_c &g = mAnmGroups[field_0x6C0 + ANIM_DECIDE_OFFSET];
            if (g.isEndReached() == true) {
                mTimer++;
                field_0x6CA = 1;
            }
            g.play();
        } break;
        case 2:
            unbindAt(field_0x6C0 + ANIM_DECIDE_OFFSET);
            mTimer = 0;
            field_0x6CA = 0;
            field_0x6C0 = 2;
            break;
    }

    if (field_0x6BC != field_0x6B8 && field_0x6B8 != 2) {
        if (field_0x6B8 == 0) {
            SmallSoundManager::GetInstance()->playSound(SE_S_MENU_POINT_TURN_PAGE_LEFT);
        } else {
            SmallSoundManager::GetInstance()->playSound(SE_S_MENU_POINT_TURN_PAGE_RIGHT);
        }
    }
}
void dLytCommonArrow_c::finalizeState_Wait() {}

void dLytCommonArrow_c::initializeState_Out() {
    mTimer = 0;
    displayElement(ANIM_OUT, 0.0f);
    mOutRequested = 0;
}
void dLytCommonArrow_c::executeState_Out() {
    switch (mTimer) {
        case 0: {
            d2d::AnmGroup_c *s = &mAnmGroups[ANIM_OUT];
            if (s->isEndReached() == 1) {
                mTimer = 1;
                field_0x6CA = 1;
            }
            s->play();
        } break;
        case 1: mStateMgr.changeState(StateID_None); break;
    }
}
void dLytCommonArrow_c::finalizeState_Out() {}
