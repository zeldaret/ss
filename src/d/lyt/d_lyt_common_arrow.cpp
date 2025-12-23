#include "d/lyt/d_lyt_common_arrow.h"

#include "d/d_cs_base.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_pad_nav.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "m/m_vec.h"
#include "rvl/MTX/mtx.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

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

#define NUM_ANIMS 9

dLytCommonArrow_c::dLytCommonArrow_c() : mStateMgr(*this, sStateID::null) {}

bool dLytCommonArrow_c::build() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("CommonArrow");
    mResAcc.attach(data, "");
    mLytBase.build("commonArrow_00.brlyt", &mResAcc);
    mLytBase.setPriority(0x86);

    for (int i = 0; i < NUM_ANIMS; i++) {
        mAnmGroups[i].init(brlanMap[i].mFile, &mResAcc, mLytBase.getLayout(), brlanMap[i].mName);
    }
    mCsHitCheck.init(mLytBase.getLayout()->GetRootPane(), 1, 0, 0);
    dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitCheck);
    mpBoundings[ARROW_LEFT] = mLytBase.findBounding("B_arrowL_00");
    mpBoundings[ARROW_RIGHT] = mLytBase.findBounding("B_arrowR_00");
    mStateMgr.changeState(StateID_None);
    setState(0);
    return true;
}

bool dLytCommonArrow_c::remove() {
    dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitCheck);
    mLytBase.unbindAnims();
    for (int i = 0; i < NUM_ANIMS; i++) {
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
    if (mVisible == true) {
        mLytBase.addToDrawList();
    }
    return true;
}

void dLytCommonArrow_c::setState(s32 state) {
    mType = state;
    if (state == 0) {
        mLytBase.setPriority(0x86);
    } else {
        mLytBase.setPriority(0x80);
    }
}

void dLytCommonArrow_c::setTranslate(const mVec2_c *arg) {
    nw4r::math::VEC3 v;
    v.x = arg->x;
    v.y = arg->y;
    v.z = 0.0f;
    mLytBase.getLayout()->GetRootPane()->SetTranslate(v);
}

bool dLytCommonArrow_c::requestIn() {
    if (!mStateMgr.getStateID()->isEqual(StateID_None)) {
        return false;
    }
    mInRequested = true;
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
    if (field_0x6C0 == ARROW_NONE && field_0x6B8 != ARROW_NONE) {
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

void dLytCommonArrow_c::checkPointAtPane() {
    int i = -1;
    if (!dPadNav::isPointerVisible()) {
        mPointedAtArrow = ARROW_NONE;
        return;
    }

    dCursorHitCheck_c *d = dCsBase_c::GetInstance()->getHitCheck();
    if (d != nullptr && d->getType() == 'lyt ') {
        if (static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == mpBoundings[ARROW_LEFT]) {
            i = ARROW_LEFT;
        } else if (static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == mpBoundings[ARROW_RIGHT]) {
            i = ARROW_RIGHT;
        }
    }

    if (i >= 0) {
        mPointedAtArrow = i;
    } else {
        mPointedAtArrow = ARROW_NONE;
    }
}

void dLytCommonArrow_c::initializeState_None() {
    mLytBase.unbindAnims();
    mInRequested = false;
    mOutRequested = false;
    mIsChangingState = false;
    mVisible = false;
    mPointedAtArrow = ARROW_NONE;
    field_0x6B8 = ARROW_NONE;
    field_0x6BC = ARROW_NONE;
    field_0x6C0 = ARROW_NONE;
    mTimer = 0;
    displayElement(ANIM_IN, 0.0f);
    mBackwards = true;
}
void dLytCommonArrow_c::executeState_None() {
    if (mInRequested == true) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytCommonArrow_c::finalizeState_None() {
    mInRequested = false;
    displayElement(ANIM_LOOP, 0.0f);
}

void dLytCommonArrow_c::initializeState_In() {
    mTimer = 0;
    mVisible = true;
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
            if (s->isEndReached() == true) {
                mTimer += 1;
                mIsChangingState = true;
            } else {
                s->play();
            }
        } break;
        case 1:
            mIsChangingState = false;
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
    if (mOutRequested == true) {
        mStateMgr.changeState(StateID_Out);
        return;
    }

    checkPointAtPane();
    if (field_0x6B8 == ARROW_LEFT) {
        d2d::AnmGroup_c &g = mAnmGroups[ANIM_ONOFF_L];
        g.play();
        tickDown(&mAnmGroups[ANIM_ONOFF_R]);
    } else if (field_0x6B8 == ARROW_RIGHT) {
        d2d::AnmGroup_c &g = mAnmGroups[ANIM_ONOFF_R];
        g.play();
        tickDown(&mAnmGroups[ANIM_ONOFF_L]);
    } else {
        tickDown(&mAnmGroups[ANIM_ONOFF_L]);
        tickDown(&mAnmGroups[ANIM_ONOFF_R]);
    }

    d2d::AnmGroup_c &g2 = mAnmGroups[ANIM_INPUT];
    if (mBackwards) {
        tickDown(&g2);
    } else {
        g2.play();
    }

    switch (mTimer) {
        case 0:
            if (field_0x6C0 != ARROW_NONE) {
                displayElement(field_0x6C0 + ANIM_DECIDE_OFFSET, 0.0f);
                if (field_0x6C0 == 0) {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_SELECT_TURN_PAGE_LEFT);
                } else {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_SELECT_TURN_PAGE_RIGHT);
                }
                mTimer++;
            }
            break;
        case 1: {
            d2d::AnmGroup_c &g = mAnmGroups[field_0x6C0 + ANIM_DECIDE_OFFSET];
            if (g.isEndReached() == true) {
                mTimer++;
                mIsChangingState = true;
            }
            g.play();
        } break;
        case 2:
            unbindAt(field_0x6C0 + ANIM_DECIDE_OFFSET);
            mTimer = 0;
            mIsChangingState = false;
            field_0x6C0 = ARROW_NONE;
            break;
    }

    if (field_0x6BC != field_0x6B8 && field_0x6B8 != ARROW_NONE) {
        if (field_0x6B8 == ARROW_LEFT) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_POINT_TURN_PAGE_LEFT);
        } else {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_POINT_TURN_PAGE_RIGHT);
        }
    }
}
void dLytCommonArrow_c::finalizeState_Wait() {}

void dLytCommonArrow_c::initializeState_Out() {
    mTimer = 0;
    displayElement(ANIM_OUT, 0.0f);
    mOutRequested = false;
}
void dLytCommonArrow_c::executeState_Out() {
    switch (mTimer) {
        case 0: {
            d2d::AnmGroup_c *s = &mAnmGroups[ANIM_OUT];
            if (s->isEndReached() == true) {
                mTimer = 1;
                mIsChangingState = 1;
            }
            s->play();
        } break;
        case 1: mStateMgr.changeState(StateID_None); break;
    }
}
void dLytCommonArrow_c::finalizeState_Out() {}
