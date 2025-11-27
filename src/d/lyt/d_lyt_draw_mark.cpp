#include "d/lyt/d_lyt_draw_mark.h"

#include "common.h"
#include "d/d_gfx.h"
#include "d/d_pad.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_do_button.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "m/m_angle.h"
#include "m/m_pad.h"
#include "m/m_vec.h"
#include "nw4r/math/math_types.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

class dLytDrawMark_HIO_c {
public:
    dLytDrawMark_HIO_c();
    virtual ~dLytDrawMark_HIO_c();

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ s16 field_0x14;
    /* 0x16 */ u16 field_0x16;
    /* 0x18 */ u16 field_0x18;
    /* 0x1A */ s16 field_0x1A;
    /* 0x1C */ s16 field_0x1C;
    /* 0x1E */ u8 field_0x1E;
};

static dLytDrawMark_HIO_c sHio;

dLytDrawMark_HIO_c::dLytDrawMark_HIO_c() {
    field_0x14 = 10;
    field_0x04 = 620.0f;
    field_0x08 = 800.0f;
    field_0x0C = 35.0f;
    field_0x10 = 30.0f;
    field_0x16 = 5;
    field_0x1A = 10923;
    field_0x18 = 9000;
    field_0x1C = 10;
    field_0x1E = 0;
}
dLytDrawMark_HIO_c::~dLytDrawMark_HIO_c() {}

STATE_DEFINE(dLytDrawMark_c, Wait);
STATE_DEFINE(dLytDrawMark_c, In);
STATE_DEFINE(dLytDrawMark_c, RingIn);
STATE_DEFINE(dLytDrawMark_c, MoveWait);
STATE_DEFINE(dLytDrawMark_c, MoveDraw);
STATE_DEFINE(dLytDrawMark_c, MoveFix);
STATE_DEFINE(dLytDrawMark_c, MoveEnd);
STATE_DEFINE(dLytDrawMark_c, Complete);

#define DRAW_MARK_ANIM_IN 0
#define DRAW_MARK_ANIM_RING_IN 1
#define DRAW_MARK_ANIM_DRAW_LINE_LOOP_OFFSET 2
#define DRAW_MARK_ANIM_BASE_LOOP 8
#define DRAW_MARK_ANIM_LINE_FIX_OFFSET 9
#define DRAW_MARK_ANIM_COMPLETE 15

#define DRAW_MARK_NUM_ANIMS 16

#define DRAW_MARK_PANE_START_OFFSET 0
#define DRAW_MARK_PANE_END_OFFSET 6
#define DRAW_MARK_PANE_RING_ALL 12

#define DRAW_MARK_NUM_PANES 13

bool dLytDrawMark_c::build(UNKWORD) {
    mVariant = dScGame_c::currentSpawnInfo.layer == 3 ? 1 : dScGame_c::currentSpawnInfo.layer == 4 ? 2 : 0;

    if (mVariant == 1) {
        buildVariant02();
    } else if (mVariant == 2) {
        buildVariant03();
    } else {
        buildVariant01();
    }

    field_0x500 = mpPanes[0];

    mLastLine = false;
    mNeedEnableLineLoop = false;
    field_0x940 = false;
    field_0x941 = false;

    field_0x8F4 = -1;
    field_0x8F8 = -1;
    mLineFixIndex = 0;

    field_0x944 = 0;

    field_0x900 = 0;
    field_0x904 = 1;
    field_0x908 = 0;
    mLineLoopIndex = 0;

    field_0x874.x = field_0x874.y = 0.0f;
    field_0x87C.x = field_0x87C.y = 0.0f;

    field_0x910 = 0.0f;
    field_0x914 = 0.0f;

    for (int i = 0; i < 10; i++) {
        field_0x918[i] = 0.0f;
    }

    field_0x955 = 0;
    field_0x956 = 0;
    field_0x957 = 0;

    for (int i = 0; i < 6; i++) {
        mStartPositions[i].x = mStartPositions[i].y = 0.0f;
        mEndPositions[i].x = mEndPositions[i].y = 0.0f;
    }

    mAnm[DRAW_MARK_ANIM_IN].setFrame(0.0f);
    mAnm[DRAW_MARK_ANIM_IN].setAnimEnable(true);
    mAnm[DRAW_MARK_ANIM_RING_IN].setFrame(0.0f);
    mAnm[DRAW_MARK_ANIM_RING_IN].setAnimEnable(true);

    mLyt.calc();

    mAnm[DRAW_MARK_ANIM_IN].setAnimEnable(false);
    mAnm[DRAW_MARK_ANIM_RING_IN].setAnimEnable(false);

    mStateMgr.changeState(StateID_In);

    return true;
}

bool dLytDrawMark_c::remove() {
    dPad::ex_c::getInstance()->setField_0x70();
    mResAcc.detach();
    return true;
}

bool dLytDrawMark_c::execute() {
    dPad::ex_c::fn_80059220();
    mStateMgr.executeState();

    for (int i = 0; i < DRAW_MARK_NUM_ANIMS; i++) {
        if (mAnmCtrlInUse[i]) {
            if (mAnm[i].isEnabled()) {
                mAnm[i].play();
            }
        }
    }

    mLyt.calc();
    return true;
}

bool dLytDrawMark_c::draw() {
    mLyt.addToDrawList();
    return true;
}

void dLytDrawMark_c::initializeState_Wait() {}
void dLytDrawMark_c::executeState_Wait() {}
void dLytDrawMark_c::finalizeState_Wait() {}

void dLytDrawMark_c::initializeState_In() {
    mAnm[DRAW_MARK_ANIM_IN].setForwardOnce();
    mAnm[DRAW_MARK_ANIM_IN].setToStart();
    mAnm[DRAW_MARK_ANIM_IN].setAnimEnable(true);
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SEAL_APPEAR);
    if (mVariant == 1) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SEAL_LINE_APPEAR_2);
    } else if (mVariant == 2) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SEAL_LINE_APPEAR_3);
    } else {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SEAL_LINE_APPEAR_1);
    }
}
void dLytDrawMark_c::executeState_In() {
    if (mAnm[DRAW_MARK_ANIM_IN].isStop2()) {
        mAnm[DRAW_MARK_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_RingIn);
    }
}
void dLytDrawMark_c::finalizeState_In() {}

void dLytDrawMark_c::initializeState_RingIn() {
    mAnm[DRAW_MARK_ANIM_RING_IN].setForwardOnce();
    mAnm[DRAW_MARK_ANIM_RING_IN].setToStart();
    mAnm[DRAW_MARK_ANIM_RING_IN].setAnimEnable(true);
    mAnm[DRAW_MARK_ANIM_BASE_LOOP].setAnimEnable(true);
    mAnm[DRAW_MARK_ANIM_BASE_LOOP].setFrame(0.0f);
}
void dLytDrawMark_c::executeState_RingIn() {
    if (mAnm[DRAW_MARK_ANIM_RING_IN].isStop2()) {
        mAnm[DRAW_MARK_ANIM_RING_IN].setAnimEnable(false);
        mNeedEnableLineLoop = true;
        mStateMgr.changeState(StateID_MoveWait);
    }
}
void dLytDrawMark_c::finalizeState_RingIn() {}

void dLytDrawMark_c::initializeState_MoveWait() {
    loadPanePos();
    if (mNeedEnableLineLoop) {
        mAnm[mLineLoopIndex + DRAW_MARK_ANIM_DRAW_LINE_LOOP_OFFSET].setAnimEnable(true);
        mNeedEnableLineLoop = false;
    }
    field_0x874 = mStartPositions[mLineLoopIndex];
    // unused
    mVec2_c _ = mEndPositions[mLineLoopIndex] - mStartPositions[mLineLoopIndex];
    if (field_0x8F4 == -1) {
        field_0x8F4 = mLineLoopIndex;
    }
}
void dLytDrawMark_c::executeState_MoveWait() {
    if (mAnm[mLineLoopIndex + DRAW_MARK_ANIM_DRAW_LINE_LOOP_OFFSET].getFrame() == 1.0f) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SEAL_LINE_BLINK);
    }
    dPad::ex_c::getInstance()->setField_0x70(sHio.field_0x18);
    fn_800ADC10(mVec2_c(0.0f, 0.0f));
    field_0x884.x = field_0x884.y = 0.0f;
    field_0x87C.set(0.0f, 0.0f);
    mStateMgr.changeState(StateID_MoveDraw);
}
void dLytDrawMark_c::finalizeState_MoveWait() {}

void dLytDrawMark_c::initializeState_MoveDraw() {
    field_0x904 = 1;
    field_0x900 = 0;
    field_0x914 = 0.0f;
    for (int i = 0; i < 10; i++) {
        field_0x918[i] = 0.0f;
    }
    field_0x955 = 0;
    field_0x956 = 0;
    field_0x957 = 0;
}
void dLytDrawMark_c::executeState_MoveDraw() {
    if (mAnm[mLineLoopIndex + DRAW_MARK_ANIM_DRAW_LINE_LOOP_OFFSET].getFrame() == 1.0f) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SEAL_LINE_BLINK);
    }
    if (field_0x941) {
        mStateMgr.changeState(StateID_Complete);
    } else {
        detectSwordSwing();
    }
}
void dLytDrawMark_c::finalizeState_MoveDraw() {}

void dLytDrawMark_c::initializeState_MoveFix() {
    mAnm[mLineLoopIndex + DRAW_MARK_ANIM_DRAW_LINE_LOOP_OFFSET].setFrame(0.0f);
    mLyt.calc();
    mAnm[mLineLoopIndex + DRAW_MARK_ANIM_DRAW_LINE_LOOP_OFFSET].setAnimEnable(false);
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SEAL_LINE);
    dRumble_c::start(dRumble_c::sRumblePreset4, dRumble_c::FLAG_SLOT0);

    mAnm[mLineFixIndex + DRAW_MARK_ANIM_LINE_FIX_OFFSET].setAnimEnable(true);
    mAnm[mLineFixIndex + DRAW_MARK_ANIM_LINE_FIX_OFFSET].setFrame(0.0f);
}
void dLytDrawMark_c::executeState_MoveFix() {
    if (mAnm[mLineFixIndex + DRAW_MARK_ANIM_LINE_FIX_OFFSET].isEndReached()) {
        mAnm[mLineFixIndex + DRAW_MARK_ANIM_LINE_FIX_OFFSET].setAnimEnable(false);
        mLineFixIndex++;
        if (mLineLoopIndex >= getNumLines() - 1) {
            mLineLoopIndex = 0;
            mLastLine = true;
        } else {
            mLineLoopIndex++;
        }
        mNeedEnableLineLoop = true;
        if (field_0x8F4 < getNumLines()) {
            if (mLastLine) {
                dLytControlGame_c::getInstance()->setDrawMarkRelated(0, 0);
                mStateMgr.changeState(StateID_Complete);
            } else {
                field_0x8F4 = -1;
                mStateMgr.changeState(StateID_MoveWait);
            }
        }
    }
}
void dLytDrawMark_c::finalizeState_MoveFix() {}

void dLytDrawMark_c::initializeState_MoveEnd() {
    if (mLineLoopIndex >= getNumLines() - 1) {
        mLineLoopIndex = 0;
        mLastLine = true;
    } else {
        mLineLoopIndex++;
    }
    mNeedEnableLineLoop = true;
}
void dLytDrawMark_c::executeState_MoveEnd() {
    if (field_0x8F4 < getNumLines()) {
        if (mLastLine) {
            dLytControlGame_c::getInstance()->setDrawMarkRelated(0, 0);
            mStateMgr.changeState(StateID_Complete);
        } else {
            field_0x8F4 = -1;
            mStateMgr.changeState(StateID_MoveWait);
        }
    }
}
void dLytDrawMark_c::finalizeState_MoveEnd() {}

void dLytDrawMark_c::initializeState_Complete() {
    static const s32 sFlags[] = {
        STORYFLAG_IMPRISONED_1_DEFEATED,
        STORYFLAG_IMPRISONED_2_DEFEATED,
        STORYFLAG_IMPRISONED_3_DEFEATED,
    };
    mAnm[DRAW_MARK_ANIM_COMPLETE].setForwardOnce();
    mAnm[DRAW_MARK_ANIM_COMPLETE].setToStart();
    mAnm[DRAW_MARK_ANIM_COMPLETE].setAnimEnable(true);
    StoryflagManager::sInstance->setFlag(sFlags[mVariant]);
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SEAL_COMPLETE);
    dSndBgmMgr_c::GetInstance()->stopAllBgm(30);
}
void dLytDrawMark_c::executeState_Complete() {
    if (mAnm[DRAW_MARK_ANIM_COMPLETE].isEndReached()) {
        mAnm[DRAW_MARK_ANIM_COMPLETE].setAnimEnable(false);
        field_0x940 = true;
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytDrawMark_c::finalizeState_Complete() {}

void dLytDrawMark_c::detectSwordSwing() {
    dPad::ex_c::getInstance()->setField_0x70(sHio.field_0x18);
    field_0x884 = dPad::getDpdPosScreen();
    if (field_0x904 > 0) {
        field_0x87C = field_0x884;
        field_0x904--;
    }

    if (field_0x908 > 0) {
        field_0x908--;
        field_0x87C = field_0x884;
    } else {
        mVec2_c swingDir(field_0x884.x - field_0x87C.x, field_0x884.y - field_0x87C.y);
        mAng swingAngle = swingDir.ang2();
        mVec2_c lineDir = mEndPositions[mLineLoopIndex] - mStartPositions[mLineLoopIndex];
        mAng lineAngle = lineDir.ang2();
        mVec2_c v(swingDir);
        v.y *= dGfx_c::getScreenAspectF();
        f32 swingMag = v.length();
        mAng ang2 = mAng::abs(lineAngle - swingAngle);

        if (field_0x910 >= sHio.field_0x08) {
            if (swingMag < sHio.field_0x10) {
                field_0x914 = 0.0f;
            } else {
                field_0x914 += swingMag;
                bool isSwingInRightDirection = false;
                if (ang2 <= sHio.field_0x1A) {
                    isSwingInRightDirection = true;
                }
                field_0x955 = field_0x956;
                field_0x956 = field_0x957;
                field_0x957 = isSwingInRightDirection;
                if (field_0x914 >= sHio.field_0x04) {
                    if (field_0x955 && field_0x956 && field_0x957) {
                        field_0x910 = sHio.field_0x08 / 2.0f;
                    }
                    mStateMgr.changeState(StateID_MoveFix);
                    return;
                }
            }
        } else {
            if (ang2 <= sHio.field_0x1A) {
                field_0x900++;
                field_0x914 += swingMag;
                if (field_0x900 > sHio.field_0x14) {
                    field_0x900 = sHio.field_0x14;
                    field_0x914 -= field_0x918[0];
                    for (int i = 0; i < 9; i++) {
                        field_0x918[i] = field_0x918[i + 1];
                    }
                    field_0x918[9] = swingMag;
                } else {
                    s32 idx = field_0x900 - 1;
                    field_0x918[idx] = swingMag;
                }
                if (field_0x914 >= sHio.field_0x04) {
                    field_0x910 = 0.0f;
                    mStateMgr.changeState(StateID_MoveFix);
                    return;
                }
            } else {
                if (swingMag >= sHio.field_0x0C) {
                    field_0x910 += swingMag - sHio.field_0x0C;
                    if (field_0x910 >= sHio.field_0x08) {
                        field_0x908 = sHio.field_0x1C;
                    }
                }
                field_0x900 = 0;
                field_0x914 = 0.0f;
                for (int i = 0; i < 10; i++) {
                    field_0x918[i] = 0.0f;
                }
            }
        }

        fn_800ADC10(mVec2_c(0.0f, 0.0f));
        field_0x884.x = field_0x884.y = 0.0f;
        field_0x87C.set(0.0f, 0.0f);
        dLytDobutton_c::setActionTextStuff(dLytDobutton_c::ICON_2, dLytDobutton_c::ACT_DO_70, true);
    }
}

// Copied from SelectBtnHelper
void dLytDrawMark_c::convertScreenPosToDpdPos(mVec2_c *pOutDpd, mVec2_c *screenPos) const {
    pOutDpd->x = (screenPos->x - dGfx_c::getWidth4x3LeftF()) / (dGfx_c::getWidth4x3F() * 0.5f) - 1.0f;
    pOutDpd->y = (screenPos->y - dGfx_c::getCurrentScreenTopF()) / (dGfx_c::getCurrentScreenHeightF() * -0.5f) - 1.0f;
}

// Copied from SelectBtnHelper
void dLytDrawMark_c::fn_800ADC10(mVec2_c screenPos) const {
    mVec2_c dpdPos;
    convertScreenPosToDpdPos(&dpdPos, &screenPos);
    dPad::ex_c::getInstance()->fn_80056580(mPad::getCurrentCoreID(), dpdPos);
}

void dLytDrawMark_c::loadPanePos() {
    for (int i = 0; i < getNumLines(); i++) {
        nw4r::math::MTX34 mtx1 = mpPanes[i + DRAW_MARK_PANE_START_OFFSET]->GetGlobalMtx();
        mStartPositions[i].set(mtx1._03, mtx1._13);

        nw4r::math::MTX34 mtx2 = mpPanes[i + DRAW_MARK_PANE_END_OFFSET]->GetGlobalMtx();
        mEndPositions[i].set(mtx2._03, mtx2._13);
    }
}

static const d2d::LytBrlanMapping brlanMapVariant01[] = {
    {          "drawMark_01_in.brlan",    "G_inOut_00"},
    {      "drawMark_01_ringIn.brlan",    "G_inOut_01"},
    {"drawMark_01_drawLineLoop.brlan", "G_triangle_00"},
    {"drawMark_01_drawLineLoop.brlan", "G_triangle_01"},
    {"drawMark_01_drawLineLoop.brlan", "G_triangle_02"},
    {                              "",              ""},
    {                              "",              ""},
    {                              "",              ""},
    {    "drawMark_01_baseLoop.brlan",  "G_baseAll_00"},
    {     "drawMark_01_LineFix.brlan",  "G_lineFix_00"},
    {     "drawMark_01_LineFix.brlan",  "G_lineFix_01"},
    {     "drawMark_01_LineFix.brlan",  "G_lineFix_02"},
    {                              "",              ""},
    {                              "",              ""},
    {                              "",              ""},
    {    "drawMark_01_complete.brlan", "G_complete_00"},
};

static const char *sPaneNamesVariant01[] = {
    "N_start_03", "N_start_04", "N_start_05", nullptr, nullptr, nullptr,        "N_end_01",
    "N_end_02",   "N_end_03",   nullptr,      nullptr, nullptr, "N_ringAll_00",
};

void dLytDrawMark_c::buildVariant01() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("DrawMark_01");
    mResAcc.attach(data, "");
    mLyt.setResAcc(&mResAcc);
    mLyt.build("drawMark_01.brlyt", nullptr);
    for (int i = 0; i < DRAW_MARK_NUM_ANIMS; i++) {
        if (brlanMapVariant01[i].mFile[0] == '\0') {
            mAnmCtrlInUse[i] = false;
        } else {
            mAnmCtrlInUse[i] = true;
            mAnm[i].init(brlanMapVariant01[i].mFile, &mResAcc, mLyt.getLayout(), brlanMapVariant01[i].mName);
            mAnm[i].bind(false);
            mAnm[i].setAnimEnable(false);
        }
    }

    for (int i = 0; i < DRAW_MARK_NUM_PANES; i++) {
        if (sPaneNamesVariant01[i] != nullptr) {
            mpPanes[i] = mLyt.findPane(sPaneNamesVariant01[i]);
        } else {
            mpPanes[i] = nullptr;
        }
    }
}

static const d2d::LytBrlanMapping brlanMapVariant02[] = {
    {          "drawMark_02_in.brlan",    "G_inOut_00"},
    {      "drawMark_02_ringIn.brlan",    "G_inOut_01"},
    {"drawMark_02_drawLineLoop.brlan", "G_triangle_00"},
    {"drawMark_02_drawLineLoop.brlan", "G_triangle_01"},
    {"drawMark_02_drawLineLoop.brlan", "G_triangle_02"},
    {"drawMark_02_drawLineLoop.brlan", "G_triangle_03"},
    {                              "",              ""},
    {                              "",              ""},
    {    "drawMark_02_baseLoop.brlan",  "G_baseAll_00"},
    {     "drawMark_02_LineFix.brlan",  "G_lineFix_00"},
    {     "drawMark_02_LineFix.brlan",  "G_lineFix_01"},
    {     "drawMark_02_LineFix.brlan",  "G_lineFix_02"},
    {     "drawMark_02_LineFix.brlan",  "G_lineFix_03"},
    {                              "",              ""},
    {                              "",              ""},
    {    "drawMark_02_complete.brlan", "G_complete_00"},
};

static const char *sPaneNamesVariant02[] = {
    "N_start_00", "N_start_01", "N_start_02", "N_start_03", nullptr, nullptr,        "N_end_01",
    "N_end_02",   "N_end_03",   "N_end_04",   nullptr,      nullptr, "N_ringAll_00",
};

void dLytDrawMark_c::buildVariant02() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("DrawMark_02");
    mResAcc.attach(data, "");
    mLyt.setResAcc(&mResAcc);
    mLyt.build("drawMark_02.brlyt", nullptr);
    for (int i = 0; i < DRAW_MARK_NUM_ANIMS; i++) {
        if (brlanMapVariant02[i].mFile[0] == '\0') {
            mAnmCtrlInUse[i] = false;
        } else {
            mAnmCtrlInUse[i] = true;
            mAnm[i].init(brlanMapVariant02[i].mFile, &mResAcc, mLyt.getLayout(), brlanMapVariant02[i].mName);
            mAnm[i].bind(false);
            mAnm[i].setAnimEnable(false);
        }
    }

    for (int i = 0; i < DRAW_MARK_NUM_PANES; i++) {
        if (sPaneNamesVariant02[i] != nullptr) {
            mpPanes[i] = mLyt.findPane(sPaneNamesVariant02[i]);
        } else {
            mpPanes[i] = nullptr;
        }
    }
}

static const d2d::LytBrlanMapping brlanMapVariant03[] = {
    {          "drawMark_03_in.brlan",    "G_inOut_00"},
    {      "drawMark_03_ringIn.brlan",    "G_inOut_01"},
    {"drawMark_03_drawLineLoop.brlan", "G_triangle_00"},
    {"drawMark_03_drawLineLoop.brlan", "G_triangle_01"},
    {"drawMark_03_drawLineLoop.brlan", "G_triangle_02"},
    {"drawMark_03_drawLineLoop.brlan", "G_triangle_03"},
    {"drawMark_03_drawLineLoop.brlan", "G_triangle_04"},
    {"drawMark_03_drawLineLoop.brlan", "G_triangle_05"},
    {    "drawMark_03_baseLoop.brlan",  "G_baseAll_00"},
    {     "drawMark_03_LineFix.brlan",  "G_lineFix_00"},
    {     "drawMark_03_LineFix.brlan",  "G_lineFix_01"},
    {     "drawMark_03_LineFix.brlan",  "G_lineFix_02"},
    {     "drawMark_03_LineFix.brlan",  "G_lineFix_03"},
    {     "drawMark_03_LineFix.brlan",  "G_lineFix_04"},
    {     "drawMark_03_LineFix.brlan",  "G_lineFix_05"},
    {    "drawMark_03_complete.brlan", "G_complete_00"},
};

static const char *sPaneNamesVariant03[] = {
    "N_start_00", "N_start_01", "N_start_02", "N_start_03", "N_start_04", "N_start_05",   "N_end_01",
    "N_end_02",   "N_end_03",   "N_end_04",   "N_end_05",   "N_end_06",   "N_ringAll_00",
};

void dLytDrawMark_c::buildVariant03() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("DrawMark_03");
    mResAcc.attach(data, "");
    mLyt.setResAcc(&mResAcc);
    mLyt.build("drawMark_03.brlyt", nullptr);
    for (int i = 0; i < DRAW_MARK_NUM_ANIMS; i++) {
        if (brlanMapVariant03[i].mFile[0] == '\0') {
            mAnmCtrlInUse[i] = false;
        } else {
            mAnmCtrlInUse[i] = true;
            mAnm[i].init(brlanMapVariant03[i].mFile, &mResAcc, mLyt.getLayout(), brlanMapVariant03[i].mName);
            mAnm[i].bind(false);
            mAnm[i].setAnimEnable(false);
        }
    }

    for (int i = 0; i < DRAW_MARK_NUM_PANES; i++) {
        if (sPaneNamesVariant03[i] != nullptr) {
            mpPanes[i] = mLyt.findPane(sPaneNamesVariant03[i]);
        } else {
            mpPanes[i] = nullptr;
        }
    }
}

s32 dLytDrawMark_c::getNumLines() const {
    if (mVariant == 1) {
        return 4;
    } else if (mVariant == 2) {
        return 6;
    } else {
        return 3;
    }
}
