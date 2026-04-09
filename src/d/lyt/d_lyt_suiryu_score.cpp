#include "d/lyt/d_lyt_suiryu_score.h"

#include "common.h"
#include "d/d_pad.h"
#include "d/d_sc_game.h"
#include "d/d_vec.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_force_line.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "m/m_vec.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/misc_actor.h"

STATE_DEFINE(dLytMeterSuiryuScoreBase_c, None);
STATE_DEFINE(dLytMeterSuiryuScoreBase_c, In);
STATE_DEFINE(dLytMeterSuiryuScoreBase_c, Wait);
STATE_DEFINE(dLytMeterSuiryuScoreBase_c, Out);

#define SUIRYU_SCORE_BASE_ANIM_IN 0
#define SUIRYU_SCORE_BASE_ANIM_ALPHA_OFFSET 1
#define SUIRYU_SCORE_BASE_ANIM_MOVE_1 4
#define SUIRYU_SCORE_BASE_ANIM_MOVE_2 5
#define SUIRYU_SCORE_BASE_ANIM_MOVE_3 6
#define SUIRYU_SCORE_BASE_ANIM_MOVE_123 7
#define SUIRYU_SCORE_BASE_ANIM_MOVE_ALL 8
#define SUIRYU_SCORE_BASE_ANIM_FIX_COLOR_OFFSET 9
#define SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET 26
#define SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET 43
#define SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET 60
#define SUIRYU_SCORE_BASE_ANIM_FIX_N_OFFSET 77
#define SUIRYU_SCORE_BASE_ANIM_FIX_N_LOOP_OFFSET 94
#define SUIRYU_SCORE_BASE_ANIM_FIX_P_OFFSET 111
#define SUIRYU_SCORE_BASE_ANIM_OUT 128

#define SUIRYU_SCORE_BASE_NUM_NOTE_ANIMS 17
#define SUIRYU_SCORE_BASE_NUM_ALPHA_ANIMS 3
#define SUIRYU_SCORE_BASE_NUM_ANIMS 129

#define SUIRYU_SCORE_BASE_NUM_PANES 77

void dLytMeterSuiryuScoreBase_c::initializeState_None() {}
void dLytMeterSuiryuScoreBase_c::executeState_None() {}
void dLytMeterSuiryuScoreBase_c::finalizeState_None() {}

void dLytMeterSuiryuScoreBase_c::initializeState_In() {}
void dLytMeterSuiryuScoreBase_c::executeState_In() {}
void dLytMeterSuiryuScoreBase_c::finalizeState_In() {}

void dLytMeterSuiryuScoreBase_c::initializeState_Wait() {}
void dLytMeterSuiryuScoreBase_c::executeState_Wait() {
    if (mAnm[SUIRYU_SCORE_BASE_ANIM_IN].isEnabled() && mAnm[SUIRYU_SCORE_BASE_ANIM_IN].isEndReached()) {
        mAnm[SUIRYU_SCORE_BASE_ANIM_IN].setAnimEnable(false);
        field_0x2264 = 2;
    }

    if (mAnm[SUIRYU_SCORE_BASE_ANIM_OUT].isEnabled() && mAnm[SUIRYU_SCORE_BASE_ANIM_OUT].isEndReached()) {
        mAnm[SUIRYU_SCORE_BASE_ANIM_OUT].setAnimEnable(false);
        field_0x2264 = 0;
        dLytMeter_c::setGanbariClefRelated(false);
    }

    for (int i = 0; i < SUIRYU_SCORE_BASE_NUM_ALPHA_ANIMS; i++) {
        if (mAnm[i + SUIRYU_SCORE_BASE_ANIM_ALPHA_OFFSET].isEnabled() &&
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_ALPHA_OFFSET].isStop2()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_ALPHA_OFFSET].setAnimEnable(false);
        }
    }

    int i;
    for (int i = 0; i < SUIRYU_SCORE_BASE_NUM_NOTE_ANIMS; i++) {
        if (field_0x2251[i] == 0 && mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].isEnabled()) {
            field_0x2262 = i;
            break;
        }
    }

    for (int i = 0; i < SUIRYU_SCORE_BASE_NUM_NOTE_ANIMS; i++) {
        if (mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].isEnabled() &&
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].isEndReached()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].setFrame(0.0f);
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].setAnimEnable(false);
        }

        if (mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET].isEnabled() &&
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET].isEndReached()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET].setFrame(0.0f);
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET].setAnimEnable(false);
        }

        if (mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].isEnabled() &&
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].isEndReached()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].setFrame(0.0f);
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].setAnimEnable(false);
        }

        if (field_0x2251[i] == 0 && mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].isEnabled() &&
            30.0f < mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].getFrame()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].setFrame(0.0f);
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].setAnimEnable(false);
        }

        if (mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_OFFSET].isEnabled() &&
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_OFFSET].isEndReached()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_OFFSET].setAnimEnable(false);
            field_0x2251[i] = 3;
        }

        if (mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_P_OFFSET].isEnabled() &&
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_P_OFFSET].isEndReached()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_P_OFFSET].setAnimEnable(false);
            field_0x2251[i] = 3;
        }

        if (mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_LOOP_OFFSET].isEnabled() &&
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_LOOP_OFFSET].isEndReached()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_LOOP_OFFSET].setAnimEnable(false);
            field_0x2251[i] = 3;
        }
    }

    for (int i = 0; i < SUIRYU_SCORE_BASE_NUM_NOTE_ANIMS; i++) {
        if (cM::isZero(mAnm[field_0x2262 + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].getFrame()) == true) {
            if (field_0x2251[i] == 0 && !mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].isEnabled()) {
                mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].setAnimEnable(true);
            }

            if (field_0x2251[i] == 0 && !mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET].isEnabled()) {
                mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET].setAnimEnable(true);
            }
        }

        if (field_0x2251[i] == 1 && !mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].isEnabled()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].setAnimEnable(true);
        }

        if (field_0x2251[i] == 2 && !mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_OFFSET].isEnabled()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_OFFSET].setAnimEnable(true);
        }

        if (field_0x2251[i] == 2 && !mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_P_OFFSET].isEnabled()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_P_OFFSET].setAnimEnable(true);
        }

        if (field_0x2251[i] == 2 && !mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_LOOP_OFFSET].isEnabled()) {
            mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_LOOP_OFFSET].setAnimEnable(true);
        }
    }
}
void dLytMeterSuiryuScoreBase_c::finalizeState_Wait() {}

void dLytMeterSuiryuScoreBase_c::initializeState_Out() {}
void dLytMeterSuiryuScoreBase_c::executeState_Out() {}
void dLytMeterSuiryuScoreBase_c::finalizeState_Out() {}

static const d2d::LytBrlanMapping scoreBaseBrlanMap[] = {
    {           "suiryuScoreBase_00_in.brlan",  "G_inOutAll_00"},
    {    "suiryuScoreBase_00_baseAlpha.brlan", "G_baseAlpha_00"},
    {    "suiryuScoreBase_00_baseAlpha.brlan", "G_baseAlpha_01"},
    {    "suiryuScoreBase_00_baseAlpha.brlan", "G_baseAlpha_02"},
    {        "suiryuScoreBase_00_move1.brlan",    "G_scroll_00"},
    {        "suiryuScoreBase_00_move2.brlan",    "G_scroll_00"},
    {        "suiryuScoreBase_00_move3.brlan",    "G_scroll_00"},
    {      "suiryuScoreBase_00_move123.brlan",    "G_scroll_00"},
    {      "suiryuScoreBase_00_moveAll.brlan",    "G_scroll_00"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_00"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_01"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_02"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_03"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_04"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_05"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_06"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_07"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_08"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_09"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_10"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_11"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_12"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_13"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_14"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_15"},
    { "suiryuScoreBase_00_noteFixColor.brlan", "G_scoreFixP_16"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_00"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_01"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_02"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_03"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_04"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_05"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_06"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_07"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_08"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_09"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_10"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_11"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_12"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_13"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_14"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_15"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreLoop_16"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_00"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_01"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_02"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_03"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_04"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_05"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_06"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_07"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_08"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_09"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_10"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_11"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_12"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_13"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_14"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_15"},
    {     "suiryuScoreBase_00_noteLoop.brlan", "G_scoreFixN_16"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_00"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_01"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_02"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_03"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_04"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_05"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_06"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_07"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_08"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_09"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_10"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_11"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_12"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_13"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_14"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_15"},
    {"suiryuScoreBase_00_noteLostLoopB.brlan", "G_scoreLoop_16"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_00"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_01"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_02"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_03"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_04"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_05"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_06"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_07"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_08"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_09"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_10"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_11"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_12"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_13"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_14"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_15"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreFixN_16"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_00"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_01"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_02"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_03"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_04"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_05"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_06"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_07"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_08"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_09"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_10"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_11"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_12"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_13"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_14"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_15"},
    {     "suiryuScoreBase_00_noteFixN.brlan", "G_scoreLoop_16"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_00"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_01"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_02"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_03"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_04"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_05"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_06"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_07"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_08"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_09"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_10"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_11"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_12"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_13"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_14"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_15"},
    {     "suiryuScoreBase_00_noteFixP.brlan", "G_scoreFixP_16"},
    {          "suiryuScoreBase_00_out.brlan",  "G_inOutAll_00"},
};

static const char *scoreBasePanes[] = {
    "N_scoreA_00", "N_scoreB_00", "N_scoreB_01", "N_scoreB_02", "N_scoreB_03", "N_scoreC_00", "N_scoreC_01",
    "N_scoreD_00", "N_scoreD_01", "N_scoreD_02", "N_scoreD_03", "N_scoreD_04", "N_scoreD_05", "N_scoreD_06",
    "N_scoreD_07", "N_scoreE_00", "N_scoreF_00", "N_scoreF_01", "N_scoreF_02", "N_scoreF_03", "N_scoreF_04",
    "N_scoreF_05", "N_scoreF_06", "N_scoreF_07", "N_scoreG_00", "N_scoreH_00", "N_scoreH_01", "N_scoreH_02",
    "N_scoreH_03", "N_scoreI_00", "N_scoreI_01", "N_scoreI_02", "N_scoreI_03", "N_scoreI_04", "N_scoreI_05",
    "N_scoreI_06", "N_scoreI_07", "N_scoreJ_00", "N_scoreK_00", "N_scoreK_01", "N_scoreK_02", "N_scoreK_03",
    "N_scoreL_00", "N_scoreL_01", "N_scoreL_02", "N_scoreL_03", "N_scoreL_04", "N_scoreL_05", "N_scoreL_06",
    "N_scoreL_07", "N_scoreM_00", "N_scoreN_00", "N_scoreN_01", "N_scoreN_02", "N_scoreN_03", "N_scoreO_00",
    "N_scoreO_01", "N_scoreP_00", "N_scoreP_01", "N_scoreP_02", "N_scoreP_03", "N_scoreQ_00", "N_scoreQ_01",
    "N_scoreQ_02", "N_scoreQ_03", "N_scoreQ_04", "N_scoreQ_05", "N_scoreQ_06", "N_scoreQ_07", "N_scoreQ_08",
    "N_scoreQ_09", "N_scoreQ_10", "N_scoreQ_11", "N_scoreQ_12", "N_scoreQ_13", "N_scoreQ_14", "N_scoreQ_15",
};

static const char *scoreBasePaneInOutAll = "N_inOutAll_00";

bool dLytMeterSuiryuScoreBase_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("suiryuScoreBase_00.brlyt", nullptr);

    for (int i = 0; i < SUIRYU_SCORE_BASE_NUM_ANIMS; i++) {
        mAnm[i].init(scoreBaseBrlanMap[i].mFile, resAcc, mLyt.getLayout(), scoreBaseBrlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < SUIRYU_SCORE_BASE_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(scoreBasePanes[i]);
    }

    mpPaneInOutAll = mLyt.findPane(scoreBasePaneInOutAll);
    mMainTranslate.x = mpPaneInOutAll->GetTranslate().x;
    mMainTranslate.y = mpPaneInOutAll->GetTranslate().y;

    for (int i = SUIRYU_SCORE_BASE_ANIM_MOVE_1; i <= SUIRYU_SCORE_BASE_ANIM_MOVE_ALL; i++) {
        mAnm[i].setAnimEnable(false);
    }

    dLytForceLine_c::initPos(&mLyt, &mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123]);

    initAnm();

    mStateMgr.changeState(StateID_Wait);
    field_0x2264 = 0;

    return true;
}

bool dLytMeterSuiryuScoreBase_c::remove() {
    for (int i = 0; i < SUIRYU_SCORE_BASE_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    return true;
}

bool dLytMeterSuiryuScoreBase_c::execute() {
    if (fn_80298600()) {
        stopMove();
    }

    if (dScGame_c::isCurrentStage("F103") && dScGame_c::currentSpawnInfo.layer == 13) {
        mVec2_c t(mMainTranslate.x, mMainTranslate.y + 110.0f);
        mpPaneInOutAll->SetTranslate(vec2ToVec3XY(t));
    } else {
        mpPaneInOutAll->SetTranslate(vec2ToVec3XY(mMainTranslate));
    }

    mStateMgr.executeState();

    for (int i = 0; i < SUIRYU_SCORE_BASE_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    mLyt.calc();

    return true;
}

bool dLytMeterSuiryuScoreBase_c::draw() {
    mLyt.addToDrawList();
    return true;
}

void dLytMeterSuiryuScoreBase_c::initAnm() {
    mAnm[SUIRYU_SCORE_BASE_ANIM_OUT].setForwardOnce();
    mAnm[SUIRYU_SCORE_BASE_ANIM_OUT].setToStart();
    mAnm[SUIRYU_SCORE_BASE_ANIM_OUT].setAnimEnable(false);

    for (int i = 0; i < SUIRYU_SCORE_BASE_NUM_NOTE_ANIMS; i++) {
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_OFFSET].setForwardOnce();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_OFFSET].setToStart();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_OFFSET].setAnimEnable(false);

        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_P_OFFSET].setForwardOnce();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_P_OFFSET].setToStart();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_P_OFFSET].setAnimEnable(false);

        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_LOOP_OFFSET].setForwardOnce();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_LOOP_OFFSET].setToStart();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_N_LOOP_OFFSET].setAnimEnable(false);

        mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].setForwardOnce();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].setToStart();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET].setAnimEnable(false);

        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET].setForwardOnce();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET].setToStart();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET].setAnimEnable(true);
        mLyt.calc();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET].setAnimEnable(false);

        mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].setForwardOnce();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].setToStart();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].setAnimEnable(true);
        mLyt.calc();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET].setAnimEnable(false);
        setColor0(i);
    }

    mAnm[SUIRYU_SCORE_BASE_ANIM_IN].setForwardOnce();
    mAnm[SUIRYU_SCORE_BASE_ANIM_IN].setToStart();
    mAnm[SUIRYU_SCORE_BASE_ANIM_IN].setAnimEnable(true);
    mLyt.calc();
    mAnm[SUIRYU_SCORE_BASE_ANIM_IN].setAnimEnable(false);

    fn_80297C80();
    fn_80297EA0();

    for (int i = 0; i < SUIRYU_SCORE_BASE_NUM_NOTE_ANIMS; i++) {
        field_0x2251[i] = 0;
    }

    field_0x2262 = 0;
}

void dLytMeterSuiryuScoreBase_c::startIn() {
    if (field_0x2264 == 0) {
        if (dScGame_c::isCurrentStage("F103") && dScGame_c::currentSpawnInfo.layer == 13) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_IN].setRate(1.0f / 3.0f);
        } else {
            mAnm[SUIRYU_SCORE_BASE_ANIM_IN].setRate(1.0f);
        }
        mAnm[SUIRYU_SCORE_BASE_ANIM_IN].setForwardOnce();
        mAnm[SUIRYU_SCORE_BASE_ANIM_IN].setFrame(0.0f);
        mAnm[SUIRYU_SCORE_BASE_ANIM_IN].setAnimEnable(true);
        field_0x2264 = 1;
        dLytMeter_c::setGanbariClefRelated(true);
    }
}

void dLytMeterSuiryuScoreBase_c::startOut() {
    if (field_0x2264 == 2) {
        if (dScGame_c::isCurrentStage("F103") && dScGame_c::currentSpawnInfo.layer == 13) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_OUT].setRate(1.0f / 3.0f);
        } else {
            mAnm[SUIRYU_SCORE_BASE_ANIM_OUT].setRate(1.0f);
        }
        mAnm[SUIRYU_SCORE_BASE_ANIM_OUT].setForwardOnce();
        mAnm[SUIRYU_SCORE_BASE_ANIM_OUT].setFrame(0.0f);
        mAnm[SUIRYU_SCORE_BASE_ANIM_OUT].setAnimEnable(true);
        field_0x2264 = 3;
    }
}

void dLytMeterSuiryuScoreBase_c::setColor0(u8 idx) {
    s32 actualIdx = idx + SUIRYU_SCORE_BASE_ANIM_FIX_COLOR_OFFSET;
    mAnm[actualIdx].setFrame(0.0f);
    mAnm[actualIdx].setAnimEnable(true);
    mLyt.calc();
    mAnm[actualIdx].setAnimEnable(false);
}

void dLytMeterSuiryuScoreBase_c::setColor1(u8 idx) {
    s32 actualIdx = idx + SUIRYU_SCORE_BASE_ANIM_FIX_COLOR_OFFSET;
    mAnm[actualIdx].setFrame(1.0f);
    mAnm[actualIdx].setAnimEnable(true);
    mLyt.calc();
    mAnm[actualIdx].setAnimEnable(false);
}

void dLytMeterSuiryuScoreBase_c::fn_802978F0(s32 idx) {
    s32 i1 = idx + SUIRYU_SCORE_BASE_ANIM_LOST_LOOP_OFFSET;
    s32 i2 = idx + SUIRYU_SCORE_BASE_ANIM_FIX_OFFSET;
    s32 i3 = idx + SUIRYU_SCORE_BASE_ANIM_LOOP_OFFSET;
    s32 i4 = idx + SUIRYU_SCORE_BASE_ANIM_FIX_N_OFFSET;
    s32 i5 = idx + SUIRYU_SCORE_BASE_ANIM_FIX_P_OFFSET;
    s32 i6 = idx + SUIRYU_SCORE_BASE_ANIM_FIX_N_LOOP_OFFSET;

    mAnm[i1].setToEnd2();
    mAnm[i2].setToEnd2();
    mAnm[i3].setToEnd2();

    mAnm[i1].setAnimEnable(true);
    mAnm[i2].setAnimEnable(true);
    mAnm[i3].setAnimEnable(true);

    mAnm[i4].setToEnd2();
    mAnm[i5].setToEnd2();
    mAnm[i6].setToEnd2();

    mAnm[i4].setAnimEnable(true);
    mAnm[i5].setAnimEnable(true);
    mAnm[i6].setAnimEnable(true);

    mLyt.calc();

    mAnm[i4].setAnimEnable(false);
    mAnm[i5].setAnimEnable(false);
    mAnm[i6].setAnimEnable(false);

    mAnm[i1].setAnimEnable(false);
    mAnm[i2].setAnimEnable(false);
    mAnm[i3].setAnimEnable(false);

    field_0x2251[idx] = 3;
}

void dLytMeterSuiryuScoreBase_c::fn_80297AE0() {
    alphaBackwards(0, 1.0f);
    alphaBackwards(1, 1.0f);
    alphaBackwards(2, 1.0f);
}

void dLytMeterSuiryuScoreBase_c::fn_80297B40() {
    alphaForwards(0, 1.0f);
    alphaForwards(1, 1.0f);
    alphaForwards(2, 1.0f);
}

void dLytMeterSuiryuScoreBase_c::fn_80297BA0() {
    switch (field_0x2250) {
        case 0: {
            alphaBackwards(0, 1.0f);
            alphaForwards(1, 1.0f);
            alphaForwards(2, 1.0f);
            break;
        }
        case 1: {
            alphaForwards(0, 1.0f);
            alphaBackwards(1, 1.0f);
            alphaForwards(2, 1.0f);
            break;
        }
        case 2: {
            alphaForwards(0, 1.0f);
            alphaForwards(1, 1.0f);
            alphaBackwards(2, 1.0f);
            break;
        }
    }
}

void dLytMeterSuiryuScoreBase_c::fn_80297C80() {
    for (int i = 0; i < 3; i++) {
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_ALPHA_OFFSET].setBackwardsOnce();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_ALPHA_OFFSET].setToEnd2();
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_ALPHA_OFFSET].setAnimEnable(true);
    }
    mLyt.calc();
    for (int i = 0; i < 3; i++) {
        mAnm[i + SUIRYU_SCORE_BASE_ANIM_ALPHA_OFFSET].setAnimEnable(false);
    }
}

void dLytMeterSuiryuScoreBase_c::alphaBackwards(s32 idx, f32 rate) {
    s32 actualIdx = idx + SUIRYU_SCORE_BASE_ANIM_ALPHA_OFFSET;
    mAnm[actualIdx].setBackwardsOnce();
    mAnm[actualIdx].setRate(rate);
    mAnm[actualIdx].setAnimEnable(true);
}

void dLytMeterSuiryuScoreBase_c::alphaForwards(s32 idx, f32 rate) {
    s32 actualIdx = idx + SUIRYU_SCORE_BASE_ANIM_ALPHA_OFFSET;
    mAnm[actualIdx].setForwardOnce();
    mAnm[actualIdx].setRate(rate);
    mAnm[actualIdx].setAnimEnable(true);
}

bool dLytMeterSuiryuScoreBase_c::fn_80297E10(u8 arg) const {
    if (field_0x2250 == 0) {
        if (arg == 1) {
            return true;
        }
        if (arg == 2) {
            return true;
        }
    } else if (field_0x2250 == 1) {
        if (arg == 0) {
            return true;
        }
        if (arg == 2) {
            return true;
        }
    } else if (field_0x2250 == 2) {
        if (arg == 0) {
            return true;
        }
        if (arg == 1) {
            return true;
        }
    }
    return false;
}

void dLytMeterSuiryuScoreBase_c::fn_80297EA0() {
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setForwardOnce();
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setToStart();
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setAnimEnable(false);

    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setForwardOnce();
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setToStart();
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setAnimEnable(false);

    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setForwardOnce();
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setToStart();
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setAnimEnable(false);

    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setForwardOnce();
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setToStart();
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setAnimEnable(false);

    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setForwardOnce();
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setToEnd2();
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setAnimEnable(true);

    mLyt.calc();

    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setAnimEnable(false);

    field_0x2250 = 4;
}

void dLytMeterSuiryuScoreBase_c::fn_80297FE0(u8 arg) {
    if (field_0x2250 == 4) {
        if (arg == 0) {
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setAnimEnable(false);
            }
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setBackwardsOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setToStart();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setAnimEnable(true);
        } else if (arg == 1) {
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setAnimEnable(false);
            }
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setBackwardsOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setToStart();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setAnimEnable(true);
        } else if (arg == 2) {
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setAnimEnable(false);
            }
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setBackwardsOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setToStart();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setAnimEnable(true);
        } else if (arg == 3) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setBackwardsOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setAnimEnable(true);
        }
        field_0x2263 = 0;
    } else if (field_0x2250 == 3) {
        if (arg == 0) {
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setAnimEnable(false);
            }
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setAnimEnable(false);
            }
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setAnimEnable(false);
            }
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setForwardOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setAnimEnable(true);
        } else if (arg == 1) {
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setAnimEnable(false);
            }
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setAnimEnable(false);
            }
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setAnimEnable(false);
            }
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setForwardOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setAnimEnable(true);
        } else if (arg == 2) {
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setAnimEnable(false);
            }
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setAnimEnable(false);
            }
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setToEnd2();
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setAnimEnable(false);
            }
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setForwardOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setAnimEnable(true);
        } else if (arg == 4) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setForwardOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setAnimEnable(true);
        }
        field_0x2263 = 0;
    } else if (field_0x2250 == 0) {
        if (arg == 1) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setForwardOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setFrame(0.0f);
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setAnimEnable(true);
            field_0x2263 = 1;
        } else if (arg == 2) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setBackwardsOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setToStart();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setAnimEnable(true);
            field_0x2263 = 1;
        } else if (arg == 3) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setBackwardsOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setAnimEnable(true);
            field_0x2263 = 0;
        }
    } else if (field_0x2250 == 1) {
        if (arg == 0) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setBackwardsOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setFrame(10.0f);
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setAnimEnable(true);
            field_0x2263 = 1;
        } else if (arg == 2) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setForwardOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setFrame(10.0f);
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setAnimEnable(true);
            field_0x2263 = 1;
        } else if (arg == 3) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setBackwardsOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setAnimEnable(true);
            field_0x2263 = 0;
        }
    } else if (field_0x2250 == 2) {
        if (arg == 0) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setForwardOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setFrame(20.0f);
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setAnimEnable(true);
            field_0x2263 = 1;
        } else if (arg == 1) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setBackwardsOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setFrame(20.0f);
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setAnimEnable(true);
            field_0x2263 = 1;
        } else if (arg == 3) {
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setBackwardsOnce();
            mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setAnimEnable(true);
            field_0x2263 = 0;
        }
    }

    field_0x2250 = arg;
}

bool dLytMeterSuiryuScoreBase_c::fn_80298600() {
    switch (field_0x2250) {
        case 0: {
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].isPlayingForwardsOnce() &&
                10.0f <= mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].getFrame() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setFrame(0.0f);
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].isPlayingBackwardsOnce() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].getFrame() <= 10.0f &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setFrame(0.0f);
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingBackwardsOnce() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isStop2() && mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setFrame(10.0f);
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setFrame(0.0f);
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingForwardsOnce() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isStop2() && mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setFrame(10.0f);
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setFrame(0.0f);
                return true;
            }
            break;
        }

        case 1: {
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].isPlayingForwardsOnce() &&
                10.0f <= mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].getFrame() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setFrame(10.0f);
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].isPlayingBackwardsOnce() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].getFrame() <= 10.0f &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setFrame(10.0f);
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingForwardsOnce() &&
                10.0f <= mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].getFrame() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setFrame(10.0f);
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setFrame(0.0f);
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingBackwardsOnce() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].getFrame() <= 10.0f &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setFrame(10.0f);
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setFrame(0.0f);
                return true;
            }
            break;
        }

        case 2: {
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].isPlayingForwardsOnce() &&
                10.0f <= mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].getFrame() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setFrame(20.0f);
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].isPlayingBackwardsOnce() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].getFrame() <= 10.0f &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setFrame(20.0f);
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingBackwardsOnce() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].getFrame() <= 20.0f &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setFrame(10.0f);
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setFrame(0.0f);
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingForwardsOnce() &&
                20.0f <= mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].getFrame() &&
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setFrame(10.0f);
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setFrame(0.0f);
                return true;
            }
            break;
        }

        case 3: {
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].isStop2() && mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].isEnabled()) {
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].isStop2() && mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setFrame(0.0f);
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].isStop2() && mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setFrame(0.0f);
                return true;
            }

            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].isStop2() && mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].isEnabled()) {
                mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setFrame(0.0f);
                return true;
            }
            break;
        }

        case 4: {
            if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].isStop2() && mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].isEnabled()) {
                return true;
            }
            break;
        }
    }

    return false;
}

void dLytMeterSuiryuScoreBase_c::stopMove() {
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].setAnimEnable(false);
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].setAnimEnable(false);
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].setAnimEnable(false);
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].setAnimEnable(false);
    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_ALL].setAnimEnable(false);
}

bool dLytMeterSuiryuScoreBase_c::fn_80298B80() {
    switch (field_0x2250) {
        case 0: {
            if (field_0x2263 == 1) {
                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingBackwardsOnce() &&
                    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isStop2()) {
                    return true;
                }

                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingForwardsOnce() &&
                    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isStop2()) {
                    return true;
                }
            } else {
                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].isPlayingForwardsOnce() &&
                    10.0f <= mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].getFrame()) {
                    return true;
                }

                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].isPlayingBackwardsOnce() &&
                    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_1].getFrame() <= 10.0f) {
                    return true;
                }
            }
            break;
        }

        case 1: {
            if (field_0x2263 == 1) {
                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingForwardsOnce() &&
                    10.0f <= mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].getFrame()) {
                    return true;
                }

                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingBackwardsOnce() &&
                    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].getFrame() <= 10.0f) {
                    return true;
                }
            } else {
                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].isPlayingForwardsOnce() &&
                    10.0f <= mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].getFrame()) {
                    return true;
                }

                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].isPlayingBackwardsOnce() &&
                    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_2].getFrame() <= 10.0f) {
                    return true;
                }
            }
            break;
        }

        case 2: {
            if (field_0x2263 == 1) {
                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingBackwardsOnce() &&
                    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].getFrame() <= 20.0f) {
                    return true;
                }

                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].isPlayingForwardsOnce() &&
                    20.0f <= mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_123].getFrame()) {
                    return true;
                }
            } else {
                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].isPlayingForwardsOnce() &&
                    10.0f <= mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].getFrame()) {
                    return true;
                }

                if (mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].isPlayingBackwardsOnce() &&
                    mAnm[SUIRYU_SCORE_BASE_ANIM_MOVE_3].getFrame() <= 10.0f) {
                    return true;
                }
            }
            break;
        }
    }

    return false;
}

STATE_DEFINE(dLytMeterClefMain_c, None);
STATE_DEFINE(dLytMeterClefMain_c, In);
STATE_DEFINE(dLytMeterClefMain_c, Wait);
STATE_DEFINE(dLytMeterClefMain_c, Out);

#define SUIRYU_SCORE_NUM_NOTES 24

void dLytMeterClefMain_c::initializeState_None() {}
void dLytMeterClefMain_c::executeState_None() {}
void dLytMeterClefMain_c::finalizeState_None() {}

void dLytMeterClefMain_c::initializeState_In() {}
void dLytMeterClefMain_c::executeState_In() {}
void dLytMeterClefMain_c::finalizeState_In() {}

void dLytMeterClefMain_c::initializeState_Wait() {}
void dLytMeterClefMain_c::executeState_Wait() {
    // TODO - Nonmatching but maybe equivalent. Function suffers from register pressure,
    // so regalloc might have an effect on codegen patterns too...
    fn_8029A200();
    fn_8029A2C0();
    fn_8029A260();
    fn_8029A440();

    bool b = dPad::ex_c::fn_80059100();
    if (dScGame_c::isCurrentStage("F103") && dScGame_c::currentSpawnInfo.layer == 13) {
        b = false;
        field_0x5F40 = 0;
    }

    if (dLytMeter_c::GetMain()->fn_800D5380(0) || dLytMeter_c::GetInstance()->isHelpOpen()) {
        b = true;
    }

    bool b2 = false;

    s32 acc = 0;
    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        if (field_0x5E81[i] != 0) {
            for (int j = field_0x5E81[i] - 1; j >= 0; j--) {
                s32 noteIdx = field_0x5EE8[acc + j];
                if (noteIdx != NUM_TOTAL_TADTONES && mNotes[noteIdx].mNote.isStateWait()) {
                    if (mNotes[noteIdx].field_0x278 == 5) {
                        field_0x5E81[i] -= 1;
                    }
                    mNotes[noteIdx].field_0x278 = 7;
                    field_0x5EE8[acc + j] = NUM_TOTAL_TADTONES;
                }
            }
        }
        acc += field_0x5E4E[i];
    }

    acc = 0;
    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        if (field_0x5E5F[i] != 0) {
            for (int j = field_0x5E5F[i] - 1; j >= 0; j--) {
                s32 noteIdx = field_0x5EE8[acc + j];
                if (noteIdx != NUM_TOTAL_TADTONES && !mNotes[noteIdx].mNote.isStateWait()) {
                    b2 = true;
                }
            }
        }

        if (field_0x5E81[i] != 0) {
            for (int j = field_0x5E81[i] - 1; j >= 0; j--) {
                s32 noteIdx = field_0x5EE8[acc + j];
                if (noteIdx != NUM_TOTAL_TADTONES && !mNotes[noteIdx].mNote.isStateWait()) {
                    b2 = true;
                }
            }
        }

        if (field_0x5EA4[i] == 0) {
            b2 = true;
        }

        acc += field_0x5E4E[i];
    }

    if (!b && !b2) {
        if (field_0x5F40 <= 0) {
            mBase.fn_80297AE0();
            field_0x5F35 = NUM_TADTONE_GROUPS + 1; // TODO ???
            field_0x5F36 = NUM_TADTONE_GROUPS + 1;
        } else {
            field_0x5F40--;
        }
    } else {
        field_0x5F40 = 45;
    }

    acc = 0;
    for (int j = 0; j < NUM_TADTONE_GROUPS; j++) {
        if (field_0x5F36 == j) {
            break;
        }
        acc += field_0x5E4E[j];
    }

    // TODO ???
    static const u8 sUnk1[NUM_TADTONE_GROUPS + 2] = {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 4
    };

    if (mBase.getField_0x2251(field_0x5F36) == 2 && mBase.fn_80297E10(sUnk1[field_0x5F35])) {
        for (int j = 0; j < field_0x5E4E[field_0x5F36]; j++) {
            s32 noteIdx = field_0x5EE8[acc + j];
            if (noteIdx != NUM_TOTAL_TADTONES) {
                mNotes[noteIdx].mNote.forceOut();
            }
        }
    }

    if (field_0x5F40 > 0) {
        if (b2 == true) {
            mBase.fn_80297BA0();
        } else {
            mBase.fn_80297B40();
            field_0x5F35 = NUM_TADTONE_GROUPS;
            field_0x5F36 = NUM_TADTONE_GROUPS;
        }
    }

    mBase.fn_80297FE0(sUnk1[field_0x5F35]);

    // TODO new local?
    acc = 0;

    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        if (field_0x5E81[i] != 0) {
            switch (field_0x5EA4[i]) {
                case 1: {
                    for (int j = field_0x5E5F[i]; j < field_0x5E70[i]; j++) {
                        s32 noteIdx = field_0x5EE8[acc + j];
                        if (noteIdx != NUM_TOTAL_TADTONES && mNotes[noteIdx].field_0x278 != 5) {
                            mNotes[noteIdx].field_0x278 = 5;
                            mNotes[noteIdx].mNote.lose();
                        }
                    }
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_Clef_GROUP_DROP_OUT);
                    field_0x5EA4[i] = 3;
                    break;
                }
                case 2: {
                    for (int j = 0; j < field_0x5E70[i]; j++) {
                        s32 noteIdx = field_0x5EE8[acc + j];
                        if (noteIdx != NUM_TOTAL_TADTONES && mNotes[noteIdx].field_0x278 != 5) {
                            mNotes[noteIdx].field_0x278 = 5;
                            mNotes[noteIdx].mNote.lose();
                        }
                    }
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_Clef_GROUP_DROP_OUT);
                    field_0x5EA4[i] = 4;
                    break;
                }
            }
        }

        for (int j = 0; j < field_0x5E81[i]; j++) {
            s32 noteIdx = field_0x5EE8[acc + j];
            if (noteIdx != NUM_TOTAL_TADTONES) {
                f32 f = mNotes[noteIdx].mNote.getField0x260();
                if (mNotes[noteIdx].field_0x278 == 2 || mNotes[noteIdx].field_0x278 == 4) {
                    mNotes[noteIdx].mNote.setFrame(f);
                }
            }
        }

        if (field_0x5E81[i] != field_0x5E4E[i]) {
            field_0x5F3C = 60;
            if (field_0x5E92[i] < field_0x5F3C && field_0x5E92[i] != 0) {
                mBase.setField_0x2251(i, 1);

                for (int j = 0; j < field_0x5E81[i]; j++) {
                    s32 noteIdx = field_0x5EE8[acc + j];
                    if (noteIdx != NUM_TOTAL_TADTONES && mNotes[noteIdx].field_0x278 == 3) {
                        if (j != 0) {
                            s32 otherNoteIdx = field_0x5EE8[acc + j - 1];
                            f32 f = mNotes[otherNoteIdx].mNote.getField0x260();
                            if (0.0f <= f - 2.0f) {
                                mNotes[noteIdx].field_0x278 = 4;
                                mNotes[noteIdx].mNote.setFrame(f - 2.0f);
                            } else {
                                mNotes[noteIdx].mNote.setFrame(0.0f);
                            }
                        } else {
                            f32 f = mNotes[noteIdx].mNote.getField0x260();
                            if (0.0f <= f) {
                                mNotes[noteIdx].field_0x278 = 4;
                                mNotes[noteIdx].mNote.setFrame(f);
                            }
                        }
                    }
                    if (mNotes[noteIdx].field_0x278 >= 1 && mNotes[noteIdx].field_0x278 <= 2) {
                        mNotes[noteIdx].field_0x278 = 3;
                        mNotes[noteIdx].mNote.startLose();
                    }
                }
            } else {
                mBase.setField_0x2251(i, 0);

                for (int j = 0; j < field_0x5E81[i]; j++) {
                    s32 noteIdx = field_0x5EE8[acc + j];
                    if (noteIdx != NUM_TOTAL_TADTONES && mNotes[noteIdx].field_0x278 == 1) {
                        if (j != 0) {
                            s32 otherNoteIdx = field_0x5EE8[acc + j - 1];
                            f32 f = mNotes[otherNoteIdx].mNote.getField0x260();
                            if (0.0f <= f - 2.0f) {
                                mNotes[noteIdx].field_0x278 = 2;
                                mNotes[noteIdx].mNote.setFrame(f - 2.0f);
                            } else {
                                mNotes[noteIdx].mNote.setFrame(0.0f);
                            }
                        } else {
                            f32 f = mNotes[noteIdx].mNote.getField0x260();
                            if (0.0f <= f) {
                                mNotes[noteIdx].field_0x278 = 2;
                                mNotes[noteIdx].mNote.setFrame(f);
                            }
                        }
                    }
                    if (mNotes[noteIdx].field_0x278 >= 3 && mNotes[noteIdx].field_0x278 <= 4) {
                        mNotes[noteIdx].field_0x278 = 1;
                        mNotes[noteIdx].mNote.stopLose();
                    }
                }
            }
        }

        acc += field_0x5E4E[i];
    }

    static const u8 sTadtoneGroupColors[NUM_TADTONE_GROUPS] = {0, 1, 2, 3, 4, 5, 2, 3, 4, 5, 0, 1, 3, 5, 0, 1, 2};

    if (mBase.fn_80298B80()) {
        // TODO new local?
        acc = 0;
        for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
            switch (field_0x5EA4[i]) {
                case 0: {
                    for (int j = 0; j < field_0x5E5F[i]; j++) {
                        if (field_0x5EE8[acc + j] == NUM_TOTAL_TADTONES) {
                            if (dLytForceLine_c::isDone(j)) {
                                for (int k = 0; k < 24; k++) {
                                    if (mNotes[k].field_0x278 == 7) {
                                        mNotes[k].field_0x278 = 0;
                                        mNotes[k].mNote.setColor(sTadtoneGroupColors[i]);
                                        mNotes[k].mNote.setTransform(*mBase.getNotePane(acc + j));
                                        mNotes[k].mNote.setMove();
                                        field_0x5EE8[acc + j] = k;
                                        break;
                                    }
                                }
                            }
                        } else if (mNotes[field_0x5EE8[acc + j]].field_0x278 == 5) {
                            break;
                        }
                    }

                    for (int j = 0; j < field_0x5E5F[i]; j++) {
                        s32 noteIdx = field_0x5EE8[acc + j];
                        if (noteIdx != NUM_TOTAL_TADTONES && mNotes[noteIdx].mNote.isMove() &&
                            mNotes[noteIdx].field_0x278 == 0) {
                            if (j != 0) {
                                s32 otherNoteIdx = field_0x5EE8[acc + j - 1];
                                f32 f = mNotes[otherNoteIdx].mNote.getField0x260();
                                if (0.0f <= f - 2.0f) {
                                    mNotes[noteIdx].field_0x278 = 2;
                                    mNotes[noteIdx].mNote.setFrame(f - 2.0f);
                                    field_0x5E81[i]++;
                                } else {
                                    mNotes[noteIdx].mNote.setFrame(0.0f);
                                }
                            } else {
                                f32 f = mNotes[noteIdx].mNote.getField0x260();
                                if (0.0f <= f) {
                                    mNotes[noteIdx].field_0x278 = 2;
                                    mNotes[noteIdx].mNote.setFrame(f);
                                    field_0x5E81[i]++;
                                }
                            }
                        }
                    }
                    break;
                }
                case 5: {
                    for (int j = 0; j < field_0x5E4E[i]; j++) {
                        s32 noteIdx = field_0x5EE8[acc + j];
                        if (noteIdx != NUM_TOTAL_TADTONES) {
                            mNotes[noteIdx].field_0x278 = 6;
                            mNotes[noteIdx].mNote.collect();
                        }
                    }
                    mBase.setColor1(i);
                    mBase.setField_0x2251_(i, 2);
                    field_0x5EA4[i] = 7;
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_Clef_GROUP_FIX);
                    break;
                }
            }

            acc += field_0x5E4E[i];
        }

        fn_8029A3B0();
        field_0x5F36 = field_0x5F35;
    }
}
void dLytMeterClefMain_c::finalizeState_Wait() {}

void dLytMeterClefMain_c::initializeState_Out() {}
void dLytMeterClefMain_c::executeState_Out() {}
void dLytMeterClefMain_c::finalizeState_Out() {}

void dLytMeterClefMain_c::startIn() {
    if (mStateMgr.isState(StateID_Wait)) {
        if (!field_0x5E4C) {
            fn_80299DF0();
        }
        mBase.startIn();
    }
}

void dLytMeterClefMain_c::startOut() {
    if (mStateMgr.isState(StateID_Wait)) {
        mBase.startOut();
    }
}

bool dLytMeterClefMain_c::build(d2d::ResAccIf_c *resAcc) {
    mBase.build(resAcc);

    for (int i = 0; i < SUIRYU_SCORE_NUM_NOTES; i++) {
        mNotes[i].mNote.build(resAcc);
    }

    fn_80299E30();
    fn_8029A200();
    fn_8029A2C0();

    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        field_0x5E70[i] = field_0x5E5F[i];
        field_0x5E81[i] = field_0x5E5F[i];
    }

    mStateMgr.changeState(StateID_Wait);

    return true;
}

bool dLytMeterClefMain_c::remove() {
    mBase.remove();
    for (int i = 0; i < SUIRYU_SCORE_NUM_NOTES; i++) {
        mNotes[i].mNote.remove();
    }
    return true;
}

bool dLytMeterClefMain_c::execute() {
    bool hasCollectedAllTadtones = StoryflagManager::sInstance->getFlag(STORYFLAG_ALL_TADTONES_GATHERED);
    bool hasTadtoneScroll = StoryflagManager::sInstance->getFlag(STORYFLAG_HAS_TADTONE_SCROLL);
    if (hasCollectedAllTadtones == true) {
        startOut();
    } else if (hasTadtoneScroll == true) {
        if (dLytMeter_c::shouldHideLytClef()) {
            startOut();
        } else {
            startIn();
        }
    } else {
        startOut();
    }

    mStateMgr.executeState();

    mBase.execute();

    for (int i = 0; i < SUIRYU_SCORE_NUM_NOTES; i++) {
        mNotes[i].mNote.execute(mBase.getInOutAllPane()->GetAlpha());
    }

    return true;
}

bool dLytMeterClefMain_c::draw() {
    mBase.draw();
    for (int i = 0; i < SUIRYU_SCORE_NUM_NOTES; i++) {
        mNotes[i].mNote.draw();
    }
    return true;
}

void dLytMeterClefMain_c::fn_80299DF0() {
    fn_8029A050();
    field_0x5E4C = 1;
}

void dLytMeterClefMain_c::fn_80299E30() {
    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        field_0x5E4E[i] = 0;
        field_0x5E5F[i] = 0;
        field_0x5E70[i] = 0;
        field_0x5E81[i] = 0;
        field_0x5EA4[i] = 9;
    }

    for (int i = 0; i < SUIRYU_SCORE_NUM_NOTES; i++) {
        mNotes[i].field_0x278 = 7;
    }

    for (int i = 0; i < NUM_TOTAL_TADTONES; i++) {
        field_0x5EE8[i] = NUM_TOTAL_TADTONES;
    }

    field_0x5F35 = NUM_TADTONE_GROUPS + 1; // TODO ???
    field_0x5F36 = NUM_TADTONE_GROUPS + 1;
    field_0x5E4D = 0;

    field_0x5F3C = 60;
    field_0x5F40 = 0;
}

void dLytMeterClefMain_c::fn_8029A050() {
    fn_8029A200();
    fn_8029A2C0();

    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        field_0x5E70[i] = field_0x5E5F[i];
        field_0x5E81[i] = field_0x5E5F[i];
    }

    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        if (field_0x5E5F[i] == field_0x5E4E[i]) {
            field_0x5EA4[i] = 8;
            field_0x5E81[i] = field_0x5E4E[i];
            mBase.fn_802978F0((u8)i);
        }
    }
}

void dLytMeterClefMain_c::fn_8029A200() {
    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        unkTadtoneInline2(i, &field_0x5E4E[i]);
    }
}

void dLytMeterClefMain_c::fn_8029A260() {
    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        unkTadtoneInline3(i, &field_0x5E92[i]);
    }
}

void dLytMeterClefMain_c::fn_8029A2C0() {
    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        u8 *result = field_0x5E5F + i;
        unkTadtoneInline(i, &field_0x5E5F[i]);
    }
}

void dLytMeterClefMain_c::fn_8029A3B0() {
    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        field_0x5E70[i] = field_0x5E5F[i];
    }
}

void dLytMeterClefMain_c::fn_8029A440() {
    s32 num8 = 0;
    for (int i = 0; i < NUM_TADTONE_GROUPS; i++) {
        if (field_0x5EA4[i] == 8) {
            num8++;
            if (num8 == NUM_TADTONE_GROUPS) {
                field_0x5E4D = 1;
            }
        } else if (field_0x5EA4[i] == 7) {
            int acc = 0;
            for (int j = 0; j < NUM_TADTONE_GROUPS; j++) {
                if (i == j) {
                    break;
                }
                acc += field_0x5E4E[j];
            }
            bool b = true;
            for (int j = 0; j < field_0x5E4E[i]; j++) {
                s32 noteIdx = field_0x5EE8[acc + j];
                if (noteIdx != NUM_TOTAL_TADTONES && !mNotes[noteIdx].mNote.isStateWait()) {
                    b = false;
                }
            }

            if (b) {
                field_0x5EA4[i] = 8;
            }
        } else {
            if ((field_0x5E5F[i] == field_0x5E4E[i]) && (field_0x5E81[i] == field_0x5E4E[i])) {
                field_0x5EA4[i] = 5;
            } else if ((field_0x5E5F[i] == 0) && (field_0x5E81[i] == 0)) {
                field_0x5EA4[i] = 9;
            } else if (field_0x5EA4[i] != 4) {
                if (field_0x5E5F[i] != field_0x5E81[i]) {
                    if (field_0x5E81[i] < field_0x5E5F[i]) {
                        field_0x5EA4[i] = 0;
                        field_0x5F35 = i;
                    }
                    if (field_0x5EA4[field_0x5F36] != 3 && field_0x5E5F[i] < field_0x5E81[i]) {
                        field_0x5EA4[i] = 1;
                    }
                    if (field_0x5E5F[i] == field_0x5E4E[i]) {
                        field_0x5F35 = i;
                    }
                }
            }
        }
    }

    if (field_0x5F35 < NUM_TADTONE_GROUPS && field_0x5F36 < NUM_TADTONE_GROUPS && field_0x5F35 != field_0x5F36) {
        if ((field_0x5EA4[field_0x5F36] < 3 || field_0x5EA4[field_0x5F36] > 5) &&
            (field_0x5EA4[field_0x5F36] < 7 || field_0x5EA4[field_0x5F36] > 8)) {
            field_0x5EA4[field_0x5F36] = 2;
        }
    }
}

SPECIAL_BASE_PROFILE(LYT_SUIRYU_SCORE, dLytSuiryuScore_c, fProfile::LYT_SUIRYU_SCORE, 0x2B5, 0x06F9);

dLytSuiryuScore_c *dLytSuiryuScore_c::sInstance;

int dLytSuiryuScore_c::create() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("Score");
    mResAcc.attach(data, "");
    mMain.build(&mResAcc);
    sInstance = this;
    return SUCCEEDED;
}

int dLytSuiryuScore_c::doDelete() {
    mMain.remove();
    sInstance = nullptr;
    return SUCCEEDED;
}

int dLytSuiryuScore_c::execute() {
    mMain.execute();
    return SUCCEEDED;
}

int dLytSuiryuScore_c::draw() {
    mMain.draw();
    return SUCCEEDED;
}
