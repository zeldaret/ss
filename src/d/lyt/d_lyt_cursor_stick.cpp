#include "d/lyt/d_lyt_cursor_stick.h"

#include "common.h"
#include "d/d_gfx.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_types.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"


STATE_DEFINE(dLytCursorStick_c, Off);
STATE_DEFINE(dLytCursorStick_c, On);

static const d2d::LytBrlanMapping brlanMap[] = {
    {"cursorStick_00_loop.brlan", "G_loop_00"},
};

#define CURSOR_STICK_ANIM_LOOP 0
#define CURSOR_STICK_NUM_ANIMS 1

static const char *sPaneNames[] = {
    "N_all_00", "N_RT_00", "N_LT_00", "N_LD_00", "N_RD_00",
};

#define CURSOR_STICK_PANE_ALL 0
#define CURSOR_STICK_PANE_RT 1
#define CURSOR_STICK_PANE_LT 2
#define CURSOR_STICK_PANE_LD 3
#define CURSOR_STICK_PANE_RD 4
#define CURSOR_STICK_NUM_PANES 5

dLytCursorStick_c *dLytCursorStick_c::sInstance;

bool dLytCursorStick_c::build() {
    sInstance->build_();
    return true;
}

bool dLytCursorStick_c::remove() {
    if (sInstance != nullptr) {
        sInstance->remove_();
        sInstance = nullptr;
    }
    return true;
}

bool dLytCursorStick_c::execute() {
    if (sInstance != nullptr) {
        sInstance->execute_();
    }
    return true;
}

bool dLytCursorStick_c::draw() {
    if (sInstance != nullptr) {
        sInstance->draw_();
    }
    return true;
}

bool dLytCursorStick_c::drawDirectly() {
    if (sInstance != nullptr) {
        sInstance->drawDirectly_();
    }
    return true;
}

bool dLytCursorStick_c::build_() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("CursorStick");
    mResAcc.attach(data, "");

    mLyt.setResAcc(&mResAcc);
    mLyt.build("cursorStick_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < CURSOR_STICK_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, &mResAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < CURSOR_STICK_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sPaneNames[i]);
    }
    mStateMgr.changeState(StateID_Off);
    mLyt.calc();
    mShouldBeOn = false;
    mpTargetPane = nullptr;

    return true;
}

bool dLytCursorStick_c::remove_() {
    for (int i = 0; i < CURSOR_STICK_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    mResAcc.detach();
    return true;
}

bool dLytCursorStick_c::execute_() {
    mStateMgr.executeState();
    if (mShouldBeOn) {
        mLyt.calc();
    }
    return true;
}

bool dLytCursorStick_c::draw_() {
    if (mShouldBeOn) {
        mLyt.addToDrawList();
    }
    return true;
}

bool dLytCursorStick_c::drawDirectly_() {
    if (mShouldBeOn) {
        mLyt.draw();
    }
    return true;
}

bool dLytCursorStick_c::setPriority(u8 priority) {
    mLyt.setPriority(priority);
}

void dLytCursorStick_c::initializeState_Off() {}
void dLytCursorStick_c::executeState_Off() {
    if (mShouldBeOn) {
        mAnm[CURSOR_STICK_ANIM_LOOP].setAnimEnable(true);
        mAnm[CURSOR_STICK_ANIM_LOOP].setFrame(0.0f);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytCursorStick_c::finalizeState_Off() {}

void dLytCursorStick_c::initializeState_On() {}
void dLytCursorStick_c::executeState_On() {
    if (!mShouldBeOn) {
        mAnm[CURSOR_STICK_ANIM_LOOP].setAnimEnable(false);
        mAnm[CURSOR_STICK_ANIM_LOOP].setFrame(0.0f);
        mStateMgr.changeState(StateID_Off);
    }

    positionToTarget();

    if (mAnm[CURSOR_STICK_ANIM_LOOP].isEnabled()) {
        mAnm[CURSOR_STICK_ANIM_LOOP].play();
    }
}
void dLytCursorStick_c::finalizeState_On() {}

void dLytCursorStick_c::setTargetPane(nw4r::lyt::Pane *pane) {
    mpTargetPane = pane;
    positionToTarget();
}

void dLytCursorStick_c::positionToTarget() {
    if (mpTargetPane == nullptr) {
        return;
    }

    u8 positionV = mpTargetPane->GetPositionV();
    u8 positionH = mpTargetPane->GetPositionH();

    nw4r::math::MTX34 mtx = mpTargetPane->GetGlobalMtx();

    nw4r::lyt::Size size = mpTargetPane->GetSize();
    f32 halfWidthScaled = size.width / 2.0f / dGfx_c::get16x9to4x3WidthScaleF();

    f32 posX;
    f32 posY;
    if (positionH == 0) {
        posX = mtx._03 + halfWidthScaled;
    } else if (positionH == 1) {
        posX = mtx._03;
    } else {
        posX = mtx._03 - halfWidthScaled;
    }

    if (positionV == 0) {
        posY = mtx._13 - size.height / 2.0f;
    } else if (positionV == 1) {
        posY = mtx._13;
    } else {
        posY = mtx._13 + size.height / 2.0f;
    }

    mVec3_c v(posX, posY, 0.0f);
    mpPanes[CURSOR_STICK_PANE_ALL]->SetTranslate(v);

    v.x = -size.width / 2.0f;
    v.y = size.height / 2.0f;
    mpPanes[CURSOR_STICK_PANE_LT]->SetTranslate(v);
    v.x = size.width / 2.0f;
    v.y = size.height / 2.0f;
    mpPanes[CURSOR_STICK_PANE_RT]->SetTranslate(v);
    v.x = -size.width / 2.0f;
    v.y = -size.height / 2.0f;
    mpPanes[CURSOR_STICK_PANE_LD]->SetTranslate(v);
    v.x = size.width / 2.0f;
    v.y = -size.height / 2.0f;
    mpPanes[CURSOR_STICK_PANE_RD]->SetTranslate(v);
    mLyt.calc();
}
