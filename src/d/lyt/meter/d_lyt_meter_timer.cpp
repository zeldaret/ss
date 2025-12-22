#define NEED_DIRECT_FRAMECTRL_ACCESS 1

#include "d/lyt/meter/d_lyt_meter_timer.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/col/c/c_m3d.h"
#include "d/d_sc_game.h"
#include "d/flag/itemflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_drop_line.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/t/d_t_siren.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/math/math_types.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event_manager.h"

#include <cstring>

STATE_DEFINE(dLytMeterTimer_c, ChangeSiren);
STATE_DEFINE(dLytMeterTimer_c, Siren);
STATE_DEFINE(dLytMeterTimer_c, ChangeSafeBloom);
STATE_DEFINE(dLytMeterTimer_c, ChangeSafe);
STATE_DEFINE(dLytMeterTimer_c, Safe);
STATE_DEFINE(dLytMeterTimer_c, ChangeFruits);
STATE_DEFINE(dLytMeterTimer_c, Fruits);

LytMeterTimerPart1_c *LytMeterTimerPart1_c::sInstance;
LytMeterTimerPart2_c *LytMeterTimerPart2_c::sInstance;
bool dLytMeterTimer_c::sDoExit;
bool dLytMeterTimer_c::sDoFinishAnim;
bool dLytMeterTimer_c::sFinished;

static const d2d::LytBrlanMapping brlanMapPart1[] = {
    {         "timer_01_in.brlan",    "G_inOut_00"},
    {"timer_01_mapPosition.brlan",    "G_inOut_00"},
    {       "timer_01_Loop.brlan",  "G_nutLoop_00"},
    {      "timer_01_Loop2.brlan",  "G_nutLoop_00"},
    {      "timer_01_Loop3.brlan",  "G_nutLoop_00"},
    {      "timer_01_Loop4.brlan",  "G_nutLoop_00"},
    {       "timer_01_Loop.brlan", "G_bowlLoop_00"},
    {      "timer_01_Loop2.brlan", "G_bowlLoop_00"},
    {      "timer_01_Loop3.brlan", "G_bowlLoop_00"},
    {      "timer_01_Loop4.brlan", "G_bowlLoop_00"},
    {    "timer_01_bowlNut.brlan",      "G_nut_00"},
    {    "timer_01_bowlNut.brlan",      "G_nut_01"},
    {    "timer_01_bowlNut.brlan",      "G_nut_02"},
    {    "timer_01_bowlNut.brlan",      "G_nut_03"},
    {    "timer_01_bowlNut.brlan",      "G_nut_04"},
    {    "timer_01_bowlNut.brlan",      "G_nut_05"},
    {    "timer_01_bowlNut.brlan",      "G_nut_06"},
    {    "timer_01_bowlNut.brlan",      "G_nut_07"},
    {    "timer_01_bowlNut.brlan",      "G_nut_08"},
    {    "timer_01_bowlNut.brlan",      "G_nut_09"},
    {    "timer_01_bowlNut.brlan",      "G_nut_10"},
    {    "timer_01_bowlNut.brlan",      "G_nut_11"},
    {    "timer_01_bowlNut.brlan",      "G_nut_12"},
    {    "timer_01_bowlNut.brlan",      "G_nut_13"},
    {    "timer_01_bowlNut.brlan",      "G_nut_14"},
    {        "timer_01_out.brlan",    "G_inOut_00"},
};

#define TIMER_01_ANIM_IN 0
#define TIMER_01_ANIM_MAP_POSITION 1
#define TIMER_01_ANIM_NUT_LOOP_OFFSET 2
#define TIMER_01_ANIM_BOWL_LOOP_OFFSET 6
#define TIMER_01_ANIM_BOWL_NUT_OFFSET 10
#define TIMER_01_ANIM_OUT 25

#define TIMER_01_NUM_ANIMS 26

#define TIMER_01_COLOR_00_CIRCLE 0
#define TIMER_01_COLOR_00_SHADOW 1
#define TIMER_01_COLOR_00_RAINBOW_CIRCLE 2

#define TIMER_01_COLOR_01_CIRCLE 0
#define TIMER_01_COLOR_01_SHADOW 1
#define TIMER_01_COLOR_01_RAINBOW_CIRCLE 2

#define TIMER_01_NUM_TRIALS 4

static const char *sPaneNamesPart1[] = {
    "N_bowlNut_00", "N_bowlNut_01", "N_bowlNut_02", "N_bowlNut_03", "N_bowlNut_04",
    "N_bowlNut_05", "N_bowlNut_06", "N_bowlNut_07", "N_bowlNut_08", "N_bowlNut_09",
    "N_bowlNut_10", "N_bowlNut_11", "N_bowlNut_12", "N_bowlNut_13", "N_bowlNut_14",
};

#define TIMER_01_NUM_TEARS 15

bool LytMeterTimerPart1_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.build("timer_01.brlyt", resAcc);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < TIMER_01_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapPart1[i].mFile, resAcc, mLyt.getLayout(), brlanMapPart1[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < TIMER_01_NUM_TEARS; i++) {
        mpPanes[i] = mLyt.findPane(sPaneNamesPart1[i]);
    }
    return true;
}

bool LytMeterTimerPart1_c::remove() {
    for (int i = 0; i < TIMER_01_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool LytMeterTimerPart1_c::execute() {
    if (dLytDropLine_c::getActivePane() == nullptr) {
        dLytDropLine_c::setPane(mpPanes[field_0x760]);
    }
    for (int i = 0; i < TIMER_01_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    return true;
}

void LytMeterTimerPart1_c::init() {
    mTrial = dAcItem_c::getCurrentTrial();
    initLoopAnim();
    initBowlNuts();
    initOutAnim();
    initColors();
    initInAnim();
    field_0x760 = 0;
    dLytDropLine_c::setPane(mpPanes[field_0x760]);
}

void LytMeterTimerPart1_c::initInAnim() {
    mAnm[TIMER_01_ANIM_IN].setForwardOnce();
    mAnm[TIMER_01_ANIM_IN].setFrame(0.0f);
    mAnm[TIMER_01_ANIM_IN].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_01_ANIM_IN].setAnimEnable(false);
}

void LytMeterTimerPart1_c::initLoopAnim() {
    for (u8 i = 0; i < TIMER_01_NUM_TRIALS; i++) {
        if (i == mTrial) {
            mAnm[i + TIMER_01_ANIM_BOWL_LOOP_OFFSET].setForwardLoop();
            mAnm[i + TIMER_01_ANIM_BOWL_LOOP_OFFSET].setFrame(0.0f);
            mAnm[i + TIMER_01_ANIM_BOWL_LOOP_OFFSET].setAnimEnable(true);
            mLyt.calc();
            mAnm[i + TIMER_01_ANIM_BOWL_LOOP_OFFSET].setAnimEnable(false);

            mAnm[i + TIMER_01_ANIM_NUT_LOOP_OFFSET].setForwardLoop();
            mAnm[i + TIMER_01_ANIM_NUT_LOOP_OFFSET].setFrame(0.0f);
            mAnm[i + TIMER_01_ANIM_NUT_LOOP_OFFSET].setAnimEnable(true);
            mLyt.calc();
            mAnm[i + TIMER_01_ANIM_NUT_LOOP_OFFSET].setAnimEnable(false);
        } else {
            mAnm[i + TIMER_01_ANIM_BOWL_LOOP_OFFSET].setAnimEnable(false);
            mAnm[i + TIMER_01_ANIM_NUT_LOOP_OFFSET].setAnimEnable(false);
        }
    }
}

void LytMeterTimerPart1_c::initBowlNuts() {
    for (int i = 0; i < TIMER_01_NUM_TEARS; i++) {
        mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].setForwardOnce();
        mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].setFrame(0.0f);
        mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].setAnimEnable(true);
    }
    mLyt.calc();
    for (int i = 0; i < TIMER_01_NUM_TEARS; i++) {
        mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].setAnimEnable(false);
    }
}

void LytMeterTimerPart1_c::resetBowlNuts() {
    for (int i = 0; i < TIMER_01_NUM_TEARS; i++) {
        mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].setBackwardsOnce();
        mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].setToStart();
        mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].setAnimEnable(true);
    }
    mLyt.calc();
    for (int i = 0; i < TIMER_01_NUM_TEARS; i++) {
        mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].setAnimEnable(false);
    }
}

void LytMeterTimerPart1_c::initOutAnim() {
    mAnm[TIMER_01_ANIM_OUT].setForwardOnce();
    mAnm[TIMER_01_ANIM_OUT].setFrame(0.0f);
    mAnm[TIMER_01_ANIM_OUT].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_01_ANIM_OUT].setAnimEnable(false);
}

void LytMeterTimerPart1_c::startInAnim() {
    if (dLytMeter_c::GetMain()->isInModeMap()) {
        mAnm[TIMER_01_ANIM_MAP_POSITION].setFrame(1.0f);
    } else {
        mAnm[TIMER_01_ANIM_MAP_POSITION].setFrame(0.0f);
    }
    mAnm[TIMER_01_ANIM_MAP_POSITION].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_01_ANIM_MAP_POSITION].setAnimEnable(false);

    mAnm[TIMER_01_ANIM_IN].setToStart();
    mAnm[TIMER_01_ANIM_IN].setAnimEnable(true);
}

void LytMeterTimerPart1_c::realizeTrial() {
    switch (mTrial) {
        case dAcItem_c::TRIAL_ELDIN:
            mAnm[0 + TIMER_01_ANIM_NUT_LOOP_OFFSET].setAnimEnable(true);
            mAnm[0 + TIMER_01_ANIM_BOWL_LOOP_OFFSET].setAnimEnable(true);
            break;
        case dAcItem_c::TRIAL_FARON:
            mAnm[1 + TIMER_01_ANIM_NUT_LOOP_OFFSET].setAnimEnable(true);
            mAnm[1 + TIMER_01_ANIM_BOWL_LOOP_OFFSET].setAnimEnable(true);
            break;
        case dAcItem_c::TRIAL_SKYLOFT:
            mAnm[2 + TIMER_01_ANIM_NUT_LOOP_OFFSET].setAnimEnable(true);
            mAnm[2 + TIMER_01_ANIM_BOWL_LOOP_OFFSET].setAnimEnable(true);
            break;
        case dAcItem_c::TRIAL_LANAYRU:
            mAnm[3 + TIMER_01_ANIM_NUT_LOOP_OFFSET].setAnimEnable(true);
            mAnm[3 + TIMER_01_ANIM_BOWL_LOOP_OFFSET].setAnimEnable(true);
            break;
    }
}

void LytMeterTimerPart1_c::startNextFruitAnim() {
    if (mActualTearCount > 0 && !mAnm[mActualTearCount - 1 + TIMER_01_ANIM_BOWL_NUT_OFFSET].isEnabled() &&
        mAnm[mActualTearCount - 1 + TIMER_01_ANIM_BOWL_NUT_OFFSET].getFrame() == 0.0f) {
        mAnm[mActualTearCount - 1 + TIMER_01_ANIM_BOWL_NUT_OFFSET].setAnimEnable(true);
    }
}

void LytMeterTimerPart1_c::startFruitAnim(s32 index) {
    if (mChangeFruitIndex < TIMER_01_NUM_TEARS && !mAnm[index + TIMER_01_ANIM_BOWL_NUT_OFFSET].isEnabled()) {
        mAnm[index + TIMER_01_ANIM_BOWL_NUT_OFFSET].setAnimEnable(true);
    }
}

void LytMeterTimerPart1_c::startOutAnim() {
    mAnm[TIMER_01_ANIM_OUT].setToStart();
    mAnm[TIMER_01_ANIM_OUT].setAnimEnable(true);
}

void LytMeterTimerPart1_c::stopInAnim() {
    mAnm[TIMER_01_ANIM_IN].setAnimEnable(false);
}

void LytMeterTimerPart1_c::disableCurrentFruitAnim() {
    if (mActualTearCount > 0 && mAnm[mActualTearCount - 1 + TIMER_01_ANIM_BOWL_NUT_OFFSET].isEnabled()) {
        mAnm[mActualTearCount - 1 + TIMER_01_ANIM_BOWL_NUT_OFFSET].setAnimEnable(false);
    }
}

void LytMeterTimerPart1_c::stopFinishedFruit() {
    for (s32 i = 0; i <= mChangeFruitIndex; i++) {
        if (mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].isEnabled() && mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].isStop2()) {
            mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].setAnimEnable(false);
            return;
        }
    }
}

void LytMeterTimerPart1_c::stopOutAnim() {
    mAnm[TIMER_01_ANIM_OUT].setAnimEnable(false);
}

bool LytMeterTimerPart1_c::isInAnimFinished() {
    if (mAnm[TIMER_01_ANIM_IN].isEnabled() && mAnm[TIMER_01_ANIM_IN].isEndReached()) {
        return true;
    }
    return false;
}

bool LytMeterTimerPart1_c::isCurrentFruitAnimFinished() {
    if (mActualTearCount > 0 && mAnm[mActualTearCount - 1 + TIMER_01_ANIM_BOWL_NUT_OFFSET].isEnabled() &&
        mAnm[mActualTearCount - 1 + TIMER_01_ANIM_BOWL_NUT_OFFSET].isStop2()) {
        return true;
    }
    return false;
}

bool LytMeterTimerPart1_c::isAnyFruitAnimFinished() {
    for (s32 i = 0; i <= mChangeFruitIndex; i++) {
        if (mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].isEnabled() && mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].isStop2()) {
            return true;
        }
    }
    return false;
}

bool LytMeterTimerPart1_c::isLastFruitAnimFinished() {
    return mAnm[TIMER_01_ANIM_BOWL_NUT_OFFSET + TIMER_01_NUM_TEARS - 1].isStop2();
}

bool LytMeterTimerPart1_c::isOutAnimFinished() {
    if (mAnm[TIMER_01_ANIM_OUT].isEnabled() && mAnm[TIMER_01_ANIM_OUT].isEndReached()) {
        return true;
    }
    return false;
}

bool LytMeterTimerPart1_c::isAnyFruitAnimAtFrame(f32 f) const {
    for (int i = 0; i <= mChangeFruitIndex; i++) {
        if (mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].isEnabled() &&
            f - 1.0f < mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].getFrame() &&
            mAnm[i + TIMER_01_ANIM_BOWL_NUT_OFFSET].getFrame() <= f) {
            return true;
        }
    }
    return false;
}

bool LytMeterTimerPart1_c::incrementTearCount() {
    mActualTearCount = ItemflagManager::sInstance->getFlagDirect(500);
    if (mDisplayedTearCount < mActualTearCount && dLytDropLine_c::finishPart()) {
        startNextFruitAnim();
        if (!dLytMeter_c::getfn_800C9FE0()) {
            startEffect(mActualTearCount - 1);
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SIREN_SHIZUKU_GET_IN);
        }

        if (mActualTearCount < TIMER_01_NUM_TEARS) {
            field_0x760++;
            dLytDropLine_c::setPane(mpPanes[field_0x760]);
        }
        mDisplayedTearCount++;
    }

    if (isCurrentFruitAnimFinished()) {
        disableCurrentFruitAnim();
        return true;
    }
    return false;
}

void LytMeterTimerPart1_c::initColors() {
    switch (mTrial) {
        case dAcItem_c::TRIAL_ELDIN:
            mColors1[TIMER_01_COLOR_00_CIRCLE] = mColor(0xFF, 0xFF, 0x80, 0xFF);
            mColors1[TIMER_01_COLOR_00_SHADOW] = mColor(0xFF, 0xFF, 0xC8, 0xFF);
            mColors1[TIMER_01_COLOR_00_RAINBOW_CIRCLE] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            mColors2[TIMER_01_COLOR_01_CIRCLE] = mColor(0xFF, 0xFF, 0x00, 0xFF);
            mColors2[TIMER_01_COLOR_01_SHADOW] = mColor(0xFF, 0xFF, 0x40, 0xFF);
            mColors2[TIMER_01_COLOR_01_RAINBOW_CIRCLE] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            break;
        case dAcItem_c::TRIAL_FARON:
            mColors1[TIMER_01_COLOR_00_CIRCLE] = mColor(0xFF, 0x80, 0xA0, 0xFF);
            mColors1[TIMER_01_COLOR_00_SHADOW] = mColor(0xFF, 0xC8, 0xC8, 0xFF);
            mColors1[TIMER_01_COLOR_00_RAINBOW_CIRCLE] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            mColors2[TIMER_01_COLOR_01_CIRCLE] = mColor(0xFF, 0x40, 0x80, 0xFF);
            mColors2[TIMER_01_COLOR_01_SHADOW] = mColor(0xFF, 0x40, 0x80, 0xFF);
            mColors2[TIMER_01_COLOR_01_RAINBOW_CIRCLE] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            break;
        case dAcItem_c::TRIAL_SKYLOFT:
            mColors1[TIMER_01_COLOR_00_CIRCLE] = mColor(0x80, 0xFF, 0x80, 0xFF);
            mColors1[TIMER_01_COLOR_00_SHADOW] = mColor(0xC8, 0xFF, 0xC8, 0xFF);
            mColors1[TIMER_01_COLOR_00_RAINBOW_CIRCLE] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            mColors2[TIMER_01_COLOR_01_CIRCLE] = mColor(0x00, 0xFF, 0x40, 0xFF);
            mColors2[TIMER_01_COLOR_01_SHADOW] = mColor(0x40, 0xFF, 0x40, 0xFF);
            mColors2[TIMER_01_COLOR_01_RAINBOW_CIRCLE] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            break;
        case dAcItem_c::TRIAL_LANAYRU:
            mColors1[TIMER_01_COLOR_00_CIRCLE] = mColor(0x80, 0xFF, 0xFF, 0xFF);
            mColors1[TIMER_01_COLOR_00_SHADOW] = mColor(0xC8, 0xC8, 0xFF, 0xFF);
            mColors1[TIMER_01_COLOR_00_RAINBOW_CIRCLE] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            mColors2[TIMER_01_COLOR_01_CIRCLE] = mColor(0x00, 0x80, 0xFF, 0xFF);
            mColors2[TIMER_01_COLOR_01_SHADOW] = mColor(0x40, 0xA0, 0xFF, 0xFF);
            mColors2[TIMER_01_COLOR_01_RAINBOW_CIRCLE] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            break;
    }
}

void LytMeterTimerPart1_c::startEffect(s32 fruitIndex) {
    nw4r::math::MTX34 mtx = mpPanes[fruitIndex]->GetGlobalMtx();
    mVec3_c pos;
    pos.x = mtx._03;
    pos.y = mtx._13;
    pos.z = 0.0f;
    dJEffManager_c::spawnUIEffect(
        PARTICLE_RESOURCE_ID_MAPPING_135_, pos, nullptr, nullptr, &mColors1[TIMER_01_COLOR_00_CIRCLE],
        &mColors2[TIMER_01_COLOR_01_CIRCLE]
    );
    dJEffManager_c::spawnUIEffect(
        PARTICLE_RESOURCE_ID_MAPPING_136_, pos, nullptr, nullptr, &mColors1[TIMER_01_COLOR_00_SHADOW],
        &mColors2[TIMER_01_COLOR_01_SHADOW]
    );
    dJEffManager_c::spawnUIEffect(
        PARTICLE_RESOURCE_ID_MAPPING_137_, pos, nullptr, nullptr, &mColors1[TIMER_01_COLOR_00_RAINBOW_CIRCLE],
        &mColors2[TIMER_01_COLOR_01_RAINBOW_CIRCLE]
    );
}

void LytMeterTimerPart1_c::startDropLineChange(nw4r::lyt::Pane *pane) {
    u8 idx = mChangeFruitIndex;
    dLytDropLine_c::startChange(mpPanes[idx], pane, mTrial, mChangeFruitIndex % 2 != 0);
}

static const d2d::LytBrlanMapping brlanMapPart2[] = {
    {         "timer_02_in.brlan",      "G_inOut_00"},
    {"timer_02_mapPosition.brlan",      "G_inOut_00"},
    {      "timer_02_dying.brlan",       "G_fall_08"},
    {      "timer_02_dying.brlan",       "G_fall_07"},
    {      "timer_02_dying.brlan",       "G_fall_06"},
    {      "timer_02_dying.brlan",       "G_fall_05"},
    {      "timer_02_dying.brlan",       "G_fall_04"},
    {      "timer_02_dying.brlan",       "G_fall_03"},
    {      "timer_02_dying.brlan",       "G_fall_02"},
    {      "timer_02_dying.brlan",       "G_fall_01"},
    {      "timer_02_dying.brlan",       "G_fall_00"},
    {    "timer_02_fallOut.brlan",       "G_fall_08"},
    {    "timer_02_fallOut.brlan",       "G_fall_07"},
    {    "timer_02_fallOut.brlan",       "G_fall_06"},
    {    "timer_02_fallOut.brlan",       "G_fall_05"},
    {    "timer_02_fallOut.brlan",       "G_fall_04"},
    {    "timer_02_fallOut.brlan",       "G_fall_03"},
    {    "timer_02_fallOut.brlan",       "G_fall_02"},
    {    "timer_02_fallOut.brlan",       "G_fall_01"},
    {    "timer_02_fallOut.brlan",       "G_fall_00"},
    {"timer_02_changeFruit.brlan",  "G_fruitsAll_00"},
    {"timer_02_fruitsColor.brlan",  "G_fruitsAll_00"},
    { "timer_02_petalOnOff.brlan", "G_petalOnOff_00"},
    {      "timer_02_bloom.brlan",      "G_bloom_00"},
    { "timer_02_beforeFall.brlan", "G_beforeFall_00"},
    {"timer_02_changeSiren.brlan",     "G_change_00"},
    {  "timer_02_sirenLoop.brlan",  "G_sirenLoop_00"},
    { "timer_02_changeSafe.brlan",     "G_change_00"},
    {       "timer_02_Loop.brlan", "G_flowreLoop_00"},
    {        "timer_02_out.brlan",      "G_inOut_00"},
};

#define TIMER_02_ANIM_IN 0
#define TIMER_02_ANIM_MAP_POSITION 1
#define TIMER_02_ANIM_DYING_OFFSET 2
#define TIMER_02_ANIM_FALL_OUT_OFFSET 11
#define TIMER_02_ANIM_CHANGE_FRUIT 20
#define TIMER_02_ANIM_FRUITS_COLOR 21
#define TIMER_02_ANIM_PETAL_ONOFF 22
#define TIMER_02_ANIM_BLOOM 23
#define TIMER_02_ANIM_BEFORE_FALL 24
#define TIMER_02_ANIM_CHANGE_SIREN 25
#define TIMER_02_ANIM_SIREN_LOOP 26
#define TIMER_02_ANIM_SIREN_SAFE 27
#define TIMER_02_ANIM_FLOWER_LOOP 28
#define TIMER_02_ANIM_OUT 29

#define TIMER_02_NUM_ANIMS 30

static const char *sPaneNamePart2 = "N_flowerAll_00";

#define TIMER_02_NUM_PETALS 9

#define TIMER_02_COLOR_00_00 0
#define TIMER_02_COLOR_00_01 1

#define TIMER_02_COLOR_01_00 0
#define TIMER_02_COLOR_01_01 1

bool LytMeterTimerPart2_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.build("timer_02.brlyt", resAcc);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < TIMER_02_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapPart2[i].mFile, resAcc, mLyt.getLayout(), brlanMapPart2[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
        mAnm[i].setAnimEnable(false);
    }

    mpPane = mLyt.findPane(sPaneNamePart2);

    return true;
}

bool LytMeterTimerPart2_c::remove() {
    for (int i = 0; i < TIMER_02_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool LytMeterTimerPart2_c::execute() {
    if (mVisible) {
        for (int i = 0; i < TIMER_02_NUM_ANIMS; i++) {
            if (mAnm[i].isEnabled()) {
                mAnm[i].play();
            }
        }
        mLyt.calc();
    }
    return true;
}

void LytMeterTimerPart2_c::init() {
    mTrial = dAcItem_c::getCurrentTrial();
    initBloomAnim();
    initBeforeFallAnim();
    initChangeSirenAnim();
    initSirenLoopAnim();
    initSirenSafeAnim();
    initFlowerLoopAnim();
    initDyingAnims();
    initFallOutAnims();
    initOutAnim();
    initInAnim();
    initChangeFruitAnim();
    initColors();
}

void LytMeterTimerPart2_c::initInAnim() {
    mAnm[TIMER_02_ANIM_IN].setForwardOnce();
    mAnm[TIMER_02_ANIM_IN].setFrame(0.0f);
    mAnm[TIMER_02_ANIM_IN].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_IN].setAnimEnable(false);
}

void LytMeterTimerPart2_c::initBloomAnim() {
    mAnm[TIMER_02_ANIM_BLOOM].setForwardOnce();
    mAnm[TIMER_02_ANIM_BLOOM].setFrame(0.0f);
    mAnm[TIMER_02_ANIM_BLOOM].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_BLOOM].setAnimEnable(false);
}

void LytMeterTimerPart2_c::initBeforeFallAnim() {
    mAnm[TIMER_02_ANIM_BEFORE_FALL].setForwardLoop();
    mAnm[TIMER_02_ANIM_BEFORE_FALL].setFrame(0.0f);
    mAnm[TIMER_02_ANIM_BEFORE_FALL].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_BEFORE_FALL].setAnimEnable(false);
}

void LytMeterTimerPart2_c::initChangeSirenAnim() {
    mAnm[TIMER_02_ANIM_CHANGE_SIREN].setForwardOnce();
    mAnm[TIMER_02_ANIM_CHANGE_SIREN].setFrame(0.0f);
    mAnm[TIMER_02_ANIM_CHANGE_SIREN].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_CHANGE_SIREN].setAnimEnable(false);
}

void LytMeterTimerPart2_c::initSirenLoopAnim() {
    mAnm[TIMER_02_ANIM_SIREN_LOOP].setForwardLoop();
    mAnm[TIMER_02_ANIM_SIREN_LOOP].setFrame(0.0f);
    mAnm[TIMER_02_ANIM_SIREN_LOOP].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_SIREN_LOOP].setAnimEnable(false);
}

void LytMeterTimerPart2_c::initSirenSafeAnim() {
    mAnm[TIMER_02_ANIM_SIREN_SAFE].setForwardOnce();
    mAnm[TIMER_02_ANIM_SIREN_SAFE].setFrame(0.0f);
    mAnm[TIMER_02_ANIM_SIREN_SAFE].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_SIREN_SAFE].setAnimEnable(false);
}

void LytMeterTimerPart2_c::initFlowerLoopAnim() {
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setForwardOnce();
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setFrame(0.0f);
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setAnimEnable(false);
}

void LytMeterTimerPart2_c::initDyingAnims() {
    for (int i = 0; i < TIMER_02_NUM_PETALS; i++) {
        if (i < mNumPetals) {
            mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setForwardOnce();
            mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setToStart();
            mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setAnimEnable(true);
        }
    }
    mLyt.calc();
    for (int i = 0; i < TIMER_02_NUM_PETALS; i++) {
        mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setAnimEnable(false);
    }
}

void LytMeterTimerPart2_c::initFallOutAnims() {
    for (int i = 0; i < TIMER_02_NUM_PETALS; i++) {
        if (i < mNumPetals) {
            mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].setForwardOnce();
            mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].setToStart();
            mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].setAnimEnable(true);
        }
    }
    mLyt.calc();
    for (int i = 0; i < TIMER_02_NUM_PETALS; i++) {
        mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].setAnimEnable(false);
    }
}

void LytMeterTimerPart2_c::initOutAnim() {
    mAnm[TIMER_02_ANIM_OUT].setForwardOnce();
    mAnm[TIMER_02_ANIM_OUT].setFrame(0.0f);
    mAnm[TIMER_02_ANIM_OUT].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_OUT].setAnimEnable(false);
}

void LytMeterTimerPart2_c::initChangeFruitAnim() {
    mAnm[TIMER_02_ANIM_CHANGE_FRUIT].setForwardOnce();
    mAnm[TIMER_02_ANIM_CHANGE_FRUIT].setFrame(0.0f);
    mAnm[TIMER_02_ANIM_CHANGE_FRUIT].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_CHANGE_FRUIT].setAnimEnable(false);
}

void LytMeterTimerPart2_c::startInAnim() {
    if (dLytMeter_c::GetMain()->isInModeMap()) {
        mAnm[TIMER_02_ANIM_MAP_POSITION].setFrame(1.0f);
    } else {
        mAnm[TIMER_02_ANIM_MAP_POSITION].setFrame(0.0f);
    }
    mAnm[TIMER_02_ANIM_MAP_POSITION].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_MAP_POSITION].setAnimEnable(false);

    mAnm[TIMER_02_ANIM_IN].setToStart();
    mAnm[TIMER_02_ANIM_IN].setAnimEnable(true);
}

void LytMeterTimerPart2_c::enableBloomAnim() {
    mAnm[TIMER_02_ANIM_BLOOM].setAnimEnable(true);
}

void LytMeterTimerPart2_c::enableBeforeFallAnim() {
    if (!mAnm[TIMER_02_ANIM_BEFORE_FALL].isEnabled()) {
        mAnm[TIMER_02_ANIM_BEFORE_FALL].setAnimEnable(true);
    }
}

void LytMeterTimerPart2_c::enableChangeSirenAnim() {
    mAnm[TIMER_02_ANIM_CHANGE_SIREN].setAnimEnable(true);
}

void LytMeterTimerPart2_c::enableSirenLoopAnim() {
    mAnm[TIMER_02_ANIM_SIREN_LOOP].setAnimEnable(true);
}

void LytMeterTimerPart2_c::enableSafeAnim() {
    mAnm[TIMER_02_ANIM_SIREN_SAFE].setAnimEnable(true);
}

void LytMeterTimerPart2_c::resumeFlowerLoop() {
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setFrame(mFlowerLoopFrame);
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setAnimEnable(true);
}

void LytMeterTimerPart2_c::startFlowerLoop() {
    if (!mAnm[TIMER_02_ANIM_FLOWER_LOOP].isEnabled()) {
        mAnm[TIMER_02_ANIM_FLOWER_LOOP].setToStart();
        mAnm[TIMER_02_ANIM_FLOWER_LOOP].setAnimEnable(true);
    }
}

void LytMeterTimerPart2_c::syncPetalsTime() {
    for (s32 i = 0; i < TIMER_02_NUM_PETALS; i++) {
        if (i == mNumPetals - 1) {
            if (!mAnm[i + TIMER_02_ANIM_DYING_OFFSET].isEnabled()) {
                f32 duration = mAnm[i + TIMER_02_ANIM_DYING_OFFSET].getAnimDuration() - 1.0f;
                s32 time = dTgSiren_c::getTime() % 10000;
                if (time == 0) {
                    time = 10000;
                }
                f32 tmp = duration - (time / 10000.0f) * duration;
                f32 targetFrame = (s32)(tmp + 0.5f);
                mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setRate(1.0f);
                mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setFrame(targetFrame);
                mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setAnimEnable(true);
            }
        } else if (mNumPetals <= i) {
            if (!mAnm[i + TIMER_02_ANIM_DYING_OFFSET].isEnabled()) {
                mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setToEnd();
                mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setAnimEnable(true);
                mLyt.calc();
                mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setAnimEnable(false);
            }
        }
    }
}

void LytMeterTimerPart2_c::updatePetalsRate() {
    for (s32 i = 0; i < TIMER_02_NUM_PETALS; i++) {
        if (i < mNumPetals) {
            mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setRate(20.0f);
            mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setAnimEnable(true);
        } else {
            mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setToEnd2();
            mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setAnimEnable(false);
        }
    }
}

void LytMeterTimerPart2_c::restartPetals() {
    for (s32 i = 0; i < TIMER_02_NUM_PETALS; i++) {
        if (mAnm[i + TIMER_02_ANIM_DYING_OFFSET].isEndReached() &&
            !mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].isEnabled()) {
            mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setToStart();
            mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].setAnimEnable(true);
        }
    }
}

void LytMeterTimerPart2_c::startOutAnim() {
    mAnm[TIMER_02_ANIM_OUT].setToStart();
    mAnm[TIMER_02_ANIM_OUT].setAnimEnable(true);
}

void LytMeterTimerPart2_c::enableChangeFruitAnim() {
    mAnm[TIMER_02_ANIM_CHANGE_FRUIT].setAnimEnable(true);
}

void LytMeterTimerPart2_c::syncTime() {
    f32 duration = mAnm[TIMER_02_ANIM_FLOWER_LOOP].getAnimDuration() - 1.0f;
    s32 time = dTgSiren_c::getTime() % 2000;
    if (time == 0) {
        time = 2000;
    }
    mFlowerLoopFrame = duration - (time / 2000.0f) * duration;
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setRate(1.0f);
}

void LytMeterTimerPart2_c::syncTimeWithSafe() {
    f32 duration = mAnm[TIMER_02_ANIM_FLOWER_LOOP].getAnimDuration() - 1.0f;
    s32 time = dTgSiren_c::getTime() % 2000;
    if (time == 0) {
        time = 2000;
    }
    f32 loopFrame = duration - (time / 2000.0f) * duration;
    f32 safeDuration = mAnm[TIMER_02_ANIM_SIREN_SAFE].getFrameCtrl()->mEndFrame - 1.0f;
    f32 finalFrame = loopFrame;
    if (safeDuration < finalFrame) {
        finalFrame = finalFrame - safeDuration;
    }
    mFlowerLoopFrame = finalFrame;
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setRate(1.0f);
}

void LytMeterTimerPart2_c::stopInAnim() {
    mAnm[TIMER_02_ANIM_IN].setAnimEnable(false);
}

void LytMeterTimerPart2_c::stopBloomAnim() {
    mAnm[TIMER_02_ANIM_BLOOM].setAnimEnable(false);
}

void LytMeterTimerPart2_c::stopBeforeFallAnim() {
    if (mAnm[TIMER_02_ANIM_BEFORE_FALL].isEnabled()) {
        mAnm[TIMER_02_ANIM_BEFORE_FALL].setAnimEnable(false);
    }
}

void LytMeterTimerPart2_c::stopChangeSirenAnim() {
    mAnm[TIMER_02_ANIM_CHANGE_SIREN].setAnimEnable(false);
}

void LytMeterTimerPart2_c::stopSirenLoopAnim() {
    mAnm[TIMER_02_ANIM_SIREN_LOOP].setAnimEnable(false);
}

void LytMeterTimerPart2_c::stopSirenSafeAnim() {
    mAnm[TIMER_02_ANIM_SIREN_SAFE].setAnimEnable(false);
}

void LytMeterTimerPart2_c::stopFlowerLoopAnim() {
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setAnimEnable(false);
}

void LytMeterTimerPart2_c::resetFlowerLoopAnim() {
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setFrame(0.0f);
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_FLOWER_LOOP].setAnimEnable(false);
}

void LytMeterTimerPart2_c::stopDyingAnims() {
    for (int i = 0; i < TIMER_02_NUM_PETALS; i++) {
        if (mAnm[i + TIMER_02_ANIM_DYING_OFFSET].isEnabled()) {
            mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setAnimEnable(false);
        }
    }
}

void LytMeterTimerPart2_c::stopFinishedDyingAnims() {
    for (int i = 0; i < TIMER_02_NUM_PETALS; i++) {
        if (mAnm[i + TIMER_02_ANIM_DYING_OFFSET].isEnabled() && mAnm[i + TIMER_02_ANIM_DYING_OFFSET].isEndReached()) {
            mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setAnimEnable(false);
        }
    }
}

void LytMeterTimerPart2_c::stopFallOutAnims() {
    for (int i = 0; i < TIMER_02_NUM_PETALS; i++) {
        if (mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].isEnabled()) {
            mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].setAnimEnable(false);
        }
    }
}

void LytMeterTimerPart2_c::stopFinishedFallOutAnims() {
    for (int i = 0; i < TIMER_02_NUM_PETALS; i++) {
        if (mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].isEnabled() &&
            mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].isEndReached()) {
            mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].setAnimEnable(false);
            // ??? - if one of the fall out anims is finished, stop all others too???
            stopFallOutAnims();
        }
    }
}

void LytMeterTimerPart2_c::stopOutAnim() {
    mAnm[TIMER_02_ANIM_OUT].setAnimEnable(false);
}

void LytMeterTimerPart2_c::stopChangeFruitAnim() {
    mAnm[TIMER_02_ANIM_CHANGE_FRUIT].setAnimEnable(false);
}

bool LytMeterTimerPart2_c::isInAnimFinished() {
    if (mAnm[TIMER_02_ANIM_IN].isEnabled() && mAnm[TIMER_02_ANIM_IN].isEndReached()) {
        return true;
    }
    return false;
}

bool LytMeterTimerPart2_c::isBloomAnimFinished() {
    if (mAnm[TIMER_02_ANIM_BLOOM].isEnabled() && mAnm[TIMER_02_ANIM_BLOOM].isEndReached()) {
        return true;
    }
    return false;
}

bool LytMeterTimerPart2_c::isBeforeFallAnimAtStart() {
    if (mAnm[TIMER_02_ANIM_BEFORE_FALL].isEnabled() && cM3d_IsZero(mAnm[TIMER_02_ANIM_BEFORE_FALL].getFrame())) {
        return true;
    }
    return false;
}

bool LytMeterTimerPart2_c::isChangeSirenAnimFinished() {
    if (mAnm[TIMER_02_ANIM_CHANGE_SIREN].isEnabled() && mAnm[TIMER_02_ANIM_CHANGE_SIREN].isEndReached()) {
        return true;
    }
    return false;
}

bool LytMeterTimerPart2_c::isSirenSafeAnimFinished() {
    if (mAnm[TIMER_02_ANIM_SIREN_SAFE].isEnabled() && mAnm[TIMER_02_ANIM_SIREN_SAFE].isEndReached()) {
        return true;
    }
    return false;
}

bool LytMeterTimerPart2_c::isFlowerLoopAnimFinished() {
    if (mAnm[TIMER_02_ANIM_FLOWER_LOOP].isEnabled() && mAnm[TIMER_02_ANIM_FLOWER_LOOP].isEndReached()) {
        return true;
    }
    return false;
}

bool LytMeterTimerPart2_c::isPetalDyingAnimFinished() {
    if (mNumPetals > 0) {
        s32 idx = mNumPetals - 1;
        if (mAnm[idx + TIMER_02_ANIM_DYING_OFFSET].isEnabled() &&
            mAnm[idx + TIMER_02_ANIM_DYING_OFFSET].isEndReached()) {
            return true;
        }
    }
    return false;
}

bool LytMeterTimerPart2_c::isFirstFallOutAnimFinished() {
    if (mAnm[TIMER_02_ANIM_FALL_OUT_OFFSET].isEnabled() && mAnm[TIMER_02_ANIM_FALL_OUT_OFFSET].isEndReached()) {
        return true;
    }
    return false;
}

bool LytMeterTimerPart2_c::isOutAnimFinished() {
    if (mAnm[TIMER_02_ANIM_OUT].isEnabled() && mAnm[TIMER_02_ANIM_OUT].isEndReached()) {
        return true;
    }
    return false;
}

bool LytMeterTimerPart2_c::isChangeFruitAnimFinished() {
    if (mAnm[TIMER_02_ANIM_CHANGE_FRUIT].isEnabled() && mAnm[TIMER_02_ANIM_CHANGE_FRUIT].isEndReached()) {
        return true;
    }
    return false;
}

void LytMeterTimerPart2_c::realizeFruitsColor() {
    f32 frame = 0.0f;
    switch (mTrial) {
        case dAcItem_c::TRIAL_ELDIN:   frame = 0.0f; break;
        case dAcItem_c::TRIAL_FARON:   frame = 1.0f; break;
        case dAcItem_c::TRIAL_SKYLOFT: frame = 2.0f; break;
        case dAcItem_c::TRIAL_LANAYRU: frame = 3.0f; break;
    }
    mAnm[TIMER_02_ANIM_FRUITS_COLOR].setForwardOnce();
    mAnm[TIMER_02_ANIM_FRUITS_COLOR].setFrame(frame);
    mAnm[TIMER_02_ANIM_FRUITS_COLOR].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_FRUITS_COLOR].setAnimEnable(false);
}

void LytMeterTimerPart2_c::finishDyingAnims() {
    for (s32 i = 0; i < TIMER_02_NUM_PETALS; i++) {
        mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setToEnd2();
        mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setAnimEnable(true);
    }
    mLyt.calc();
    for (s32 i = 0; i < TIMER_02_NUM_PETALS; i++) {
        mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setAnimEnable(false);
    }
}

void LytMeterTimerPart2_c::setPetalsToStart() {
    for (s32 i = 0; i < TIMER_02_NUM_PETALS; i++) {
        mAnm[i + TIMER_02_ANIM_DYING_OFFSET].setToStart();
        mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].setToEnd2();
        mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].setAnimEnable(true);
    }
    mLyt.calc();
    for (s32 i = 0; i < TIMER_02_NUM_PETALS; i++) {
        mAnm[i + TIMER_02_ANIM_FALL_OUT_OFFSET].setAnimEnable(false);
    }
}

void LytMeterTimerPart2_c::realizePetalsOnOff() {
    mAnm[TIMER_02_ANIM_PETAL_ONOFF].setAnimEnable(true);
    mAnm[TIMER_02_ANIM_PETAL_ONOFF].setFrame(9 - mNumPetals);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_PETAL_ONOFF].setAnimEnable(false);
}

void LytMeterTimerPart2_c::finishSingleAnim(u8 idx) {
    mAnm[idx].setToEnd2();
}

void LytMeterTimerPart2_c::resetSingleAnim(u8 idx) {
    mAnm[idx].setToStart();
}

void LytMeterTimerPart2_c::calc() {
    mLyt.calc();
}

void LytMeterTimerPart2_c::initColors() {
    switch (mTrial) {
        case dAcItem_c::TRIAL_ELDIN:
            mColors1[TIMER_02_COLOR_00_00] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            mColors1[TIMER_02_COLOR_00_01] = mColor(0xFF, 0xFF, 0x40, 0xFF);
            mColors2[TIMER_02_COLOR_01_00] = mColor(0xFF, 0xFF, 0x00, 0xFF);
            mColors2[TIMER_02_COLOR_01_01] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            break;
        case dAcItem_c::TRIAL_FARON:
            mColors1[TIMER_02_COLOR_00_00] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            mColors1[TIMER_02_COLOR_00_01] = mColor(0xFF, 0x50, 0x82, 0xFF);
            mColors2[TIMER_02_COLOR_01_00] = mColor(0xFF, 0x00, 0x40, 0xFF);
            mColors2[TIMER_02_COLOR_01_01] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            break;
        case dAcItem_c::TRIAL_SKYLOFT:
            mColors1[TIMER_02_COLOR_00_00] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            mColors1[TIMER_02_COLOR_00_01] = mColor(0x78, 0xFF, 0x78, 0xFF);
            mColors2[TIMER_02_COLOR_01_00] = mColor(0x00, 0xFF, 0x3C, 0xFF);
            mColors2[TIMER_02_COLOR_01_01] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            break;
        case dAcItem_c::TRIAL_LANAYRU:
            mColors1[TIMER_02_COLOR_00_00] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            mColors1[TIMER_02_COLOR_00_01] = mColor(0x78, 0xB4, 0xFF, 0xFF);
            mColors2[TIMER_02_COLOR_01_00] = mColor(0x00, 0x80, 0xFF, 0xFF);
            mColors2[TIMER_02_COLOR_01_01] = mColor(0xFF, 0xFF, 0xFF, 0xFF);
            break;
    }
}

void LytMeterTimerPart2_c::createSingleFruitEffect() {
    nw4r::math::MTX34 mtx = mpPane->GetGlobalMtx();
    mVec3_c pos;
    pos.x = mtx._03;
    pos.y = mtx._13;
    pos.z = 0.0f;
    dJEffManager_c::spawnUIEffect(
        PARTICLE_RESOURCE_ID_MAPPING_174_, pos, nullptr, nullptr, &mColors1[TIMER_02_COLOR_00_00],
        &mColors2[TIMER_02_COLOR_01_00]
    );
}

void LytMeterTimerPart2_c::createFruitCompleteEffect() {
    nw4r::math::MTX34 mtx = mpPane->GetGlobalMtx();
    mVec3_c pos;
    pos.x = mtx._03;
    pos.y = mtx._13;
    pos.z = 0.0f;
    dJEffManager_c::spawnUIEffect(
        PARTICLE_RESOURCE_ID_MAPPING_206_, pos, nullptr, nullptr, &mColors1[TIMER_02_COLOR_00_01],
        &mColors2[TIMER_02_COLOR_01_01]
    );
}

void dLytMeterTimer_c::initializeState_ChangeSiren() {}
void dLytMeterTimer_c::executeState_ChangeSiren() {
    if (mpPart2->isFlowerLoopAnimFinished()) {
        mpPart2->stopFlowerLoopAnim();
        mpPart2->resetFlowerLoopAnim();
        mpPart2->resetSingleAnim(TIMER_02_ANIM_FLOWER_LOOP);
    }
    if (mpPart2->isBeforeFallAnimAtStart()) {
        mpPart2->stopBeforeFallAnim();
        mpPart2->resetSingleAnim(TIMER_02_ANIM_BEFORE_FALL);
    }

    if (field_0x57 <= 1) {
        if (mLastTime < mActualTime && field_0x58 == 1 && mpPart1->getActualTearCount() != TIMER_01_NUM_TEARS) {
            gotoChangeSafeBloom();
        }
        if (!EventManager::isInEvent()) {
            mpPart2->setVisible(true);
        }
    }

    if (dLytMeter_c::getfn_800D97A0() && !field_0x54) {
        mpPart2->setVisible(true);
    }

    switch (field_0x57) {
        case 0:
            if (mpPart2->isPetalDyingAnimFinished()) {
                mpPart2->stopFinishedDyingAnims();
                field_0x57 = 1;
                mpPart2->restartPetals();
            }
            break;
        case 1:
            if (mpPart2->isFirstFallOutAnimFinished()) {
                dLytMeter_c::setField_0x13B61(1);
                field_0x57 = 2;
            }
            break;
        case 2:
            if (mpPart2->isFirstFallOutAnimFinished() && field_0x48) {
                mpPart2->stopFinishedFallOutAnims();
                mpPart2->enableChangeSirenAnim();
            }
            if (mpPart2->isChangeSirenAnimFinished()) {
                mpPart2->stopChangeSirenAnim();
                mpPart2->resetSingleAnim(TIMER_02_ANIM_CHANGE_SIREN);
                field_0x58 = 1;
                gotoSiren();
            }
            break;
    }
}
void dLytMeterTimer_c::finalizeState_ChangeSiren() {}

void dLytMeterTimer_c::initializeState_Siren() {}
void dLytMeterTimer_c::executeState_Siren() {
    if (!field_0x58) {
        if (mpPart2->isSirenSafeAnimFinished()) {
            mpPart2->stopSirenSafeAnim();
        }
        const char *eventName = EventManager::getCurrentEventName();
        if (eventName != nullptr && (!strcmp(eventName, "SirenChaser") || !strcmp(eventName, "SirenChaserL"))) {
            gotoChangeSiren();
        }
    }
}
void dLytMeterTimer_c::finalizeState_Siren() {}

void dLytMeterTimer_c::initializeState_ChangeSafeBloom() {}
void dLytMeterTimer_c::executeState_ChangeSafeBloom() {
    if (mpPart2->isBloomAnimFinished()) {
        mpPart2->stopSirenSafeAnim();
        mpPart2->stopBloomAnim();
        gotoChangeSafe();
    } else if (mpPart1->incrementTearCount() == true) {
        mpPart2->stopSirenSafeAnim();
        mpPart2->initBeforeFallAnim();
        mpPart2->setNumPetals(0);
        mpPart2->realizePetalsOnOff();
        mpPart2->enableSafeAnim();
        mpPart2->finishSingleAnim(TIMER_02_ANIM_SIREN_SAFE);
        mpPart2->resetSingleAnim(TIMER_02_ANIM_BLOOM);
        mpPart2->enableBloomAnim();
        mpPart2->stopDyingAnims();
    }
}
void dLytMeterTimer_c::finalizeState_ChangeSafeBloom() {}

void dLytMeterTimer_c::initializeState_ChangeSafe() {}
void dLytMeterTimer_c::executeState_ChangeSafe() {
    if (mpPart1->getActualTearCount() != TIMER_01_NUM_TEARS &&
        mpPart1->getDisplayedTearCount() < mpPart1->getActualTearCount()) {
        gotoChangeSafeBloom();
    } else {
        if (mLastTime < mActualTime) {
            gotoChangeSafeBloom();
        } else {
            if (mpPart2->isSirenSafeAnimFinished()) {
                mpPart2->stopSirenSafeAnim();
                gotoSafe();
            }
            s32 rem = mActualTime % 1000;
            s32 quot = mActualTime / 1000;
            if (rem == 0 && quot % 2 == 0) {
                mpPart2->syncTime();
                field_0x59 = true;
                field_0x5C = 4;
            }

            if (field_0x59) {
                if (field_0x5C-- == 0) {
                    mpPart2->resumeFlowerLoop();
                    field_0x59 = false;
                }
            }
        }
    }
}
void dLytMeterTimer_c::finalizeState_ChangeSafe() {}

void dLytMeterTimer_c::initializeState_Safe() {}
void dLytMeterTimer_c::executeState_Safe() {
    const char *eventName = EventManager::getCurrentEventName();
    if (eventName != nullptr && !strcmp(eventName, "SirenSeekerFind")) {
        gotoChangeSiren();
    } else if (eventName != nullptr && !strcmp(eventName, "SirenAreaEnter")) {
        gotoChangeSiren();
    } else {
        if (mActualTime == 0) {
            gotoChangeSiren();
        } else if (!checkForPetalChangeMaybe()) {
            if (mLastTime < mActualTime) {
                gotoChangeSafeBloom();
            } else {
                mpPart2->setNumPetals(getPetalForTimerMaybe(mActualTime));
                if (mpPart1->getActualTearCount() != TIMER_01_NUM_TEARS) {
                    if (mpPart2->getNumPetals() == 1 && mActualTime < 9000) {
                        mpPart2->enableBeforeFallAnim();
                    } else {
                        mpPart2->stopBeforeFallAnim();
                    }
                }
                if (mLastTime == mActualTime) {
                    if (mpPart1->getActualTearCount() != TIMER_01_NUM_TEARS) {
                        mpPart2->stopDyingAnims();
                        mpPart2->stopFinishedFallOutAnims();
                    } else {
                        mpPart2->startFlowerLoop();
                    }
                    if (mpPart2->isFlowerLoopAnimFinished()) {
                        mpPart2->stopFlowerLoopAnim();
                    }
                } else {
                    s32 quot = mActualTime / 1000;
                    if (mpPart2->isFlowerLoopAnimFinished()) {
                        mpPart2->stopFlowerLoopAnim();
                    }
                    s32 rem = mActualTime % 1000;
                    if (rem == 0 && quot % 2 == 0) {
                        mpPart2->syncTime();
                        field_0x59 = true;
                        field_0x5C = 4;
                    }
                    if (field_0x59) {
                        if (field_0x5C-- == 0) {
                            mpPart2->resumeFlowerLoop();
                            field_0x59 = false;
                        }
                    }
                    mpPart2->stopFinishedDyingAnims();
                    mpPart2->stopFinishedFallOutAnims();
                    if (mActualTime <= 90000) {
                        mpPart2->syncPetalsTime();
                    }
                }
            }
        }
    }
}
void dLytMeterTimer_c::finalizeState_Safe() {}

void dLytMeterTimer_c::initializeState_ChangeFruits() {}
void dLytMeterTimer_c::executeState_ChangeFruits() {
    switch (field_0x56) {
        case 3:
            if (mpPart2->isBloomAnimFinished()) {
                mpPart2->stopSirenSafeAnim();
                mpPart2->stopBloomAnim();
                mpPart2->resetSingleAnim(TIMER_02_ANIM_BLOOM);
                startSafe();
            }
            break;
        case 4:
            if (mpPart2->isSirenSafeAnimFinished()) {
                mpPart2->stopSirenSafeAnim();
                field_0x56 = 5;
            }
            break;
        case 5:
            if (sDoFinishAnim == true) {
                doPickup();
            }
            break;
        case 6:
            sFinished = true;
            if (mpPart2->isFlowerLoopAnimFinished()) {
                mpPart2->stopFlowerLoopAnim();
                mpPart2->resetFlowerLoopAnim();
                mpPart2->resetSingleAnim(TIMER_02_ANIM_FLOWER_LOOP);
            }

            if (dLytDropLine_c::finishPart()) {
                if (mpPart1->isLastFruitAnimFinished()) {
                    mpPart2->realizeFruitsColor();
                    mpPart2->enableChangeFruitAnim();
                    mpPart2->initChangeSirenAnim();
                    mpPart2->createFruitCompleteEffect();
                } else {
                    mpPart2->createSingleFruitEffect();
                }
            }

            if (mpPart1->isAnyFruitAnimAtFrame(4.0f)) {
                s32 fruit = mpPart1->getChangeFruitIndex() + 1;
                mpPart1->setChangeFruitIndex(fruit);
                if (mpPart1->getChangeFruitIndex() < TIMER_01_NUM_TEARS) {
                    mpPart1->startDropLineChange(mpPart2->i_getPane());
                    mpPart1->startFruitAnim(fruit);
                    mpPart1->startEffect(fruit);
                }
            }

            if (mpPart1->isAnyFruitAnimFinished()) {
                mpPart1->stopFinishedFruit();
            }

            if (mpPart2->isChangeFruitAnimFinished()) {
                if (field_0x60 <= 0) {
                    mpPart2->stopSirenSafeAnim();
                    mpPart2->stopChangeFruitAnim();
                    mpPart2->resetSingleAnim(TIMER_02_ANIM_CHANGE_FRUIT);
                    dLytMeter_c::setField_0x13B64(1);
                    gotoFruits();
                } else {
                    field_0x60--;
                }
            }
            break;
    }
}
void dLytMeterTimer_c::finalizeState_ChangeFruits() {}

void dLytMeterTimer_c::initializeState_Fruits() {
    sDoExit = true;
}
void dLytMeterTimer_c::executeState_Fruits() {
    if (mpPart2->isFlowerLoopAnimFinished()) {
        mpPart2->stopFlowerLoopAnim();
        mpPart2->resetFlowerLoopAnim();
        mpPart2->resetSingleAnim(TIMER_02_ANIM_FLOWER_LOOP);
    }

    if (mActualTime == 0) {
        gotoChangeSiren();
    }
}
void dLytMeterTimer_c::finalizeState_Fruits() {}

bool dLytMeterTimer_c::build() {
    mpPart1 = LytMeterTimerPart1_c::GetInstance();
    mpPart2 = LytMeterTimerPart2_c::GetInstance();
    mpPart1->init();
    mpPart2->init();
    mpPart1->realizeTrial();
    mpPart2->setNumPetals(TIMER_02_NUM_PETALS);
    mpPart2->realizePetalsOnOff();
    mpPart2->initFallOutAnims();
    mpPart2->initDyingAnims();
    mpPart2->resetSingleAnim(TIMER_02_ANIM_SIREN_SAFE);
    mpPart2->enableSafeAnim();
    mStateMgr.changeState(StateID_Siren);
    field_0x60 = 20;
    sDoExit = false;
    sDoFinishAnim = false;
    sFinished = false;
    return true;
}

bool dLytMeterTimer_c::remove() {
    mpPart1 = nullptr;
    mpPart2 = nullptr;
    return true;
}

bool dLytMeterTimer_c::execute() {
    if (dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL) {
        mActualTime = dTgSiren_c::getTime();
        if (mActualTime > 500 && isInSiren() && mpPart1->getActualTearCount() != TIMER_01_NUM_TEARS) {
            gotoChangeSafeBloom();
        }

        if (mpPart1->getActualTearCount() == TIMER_01_NUM_TEARS && dLytMeter_c::getField_0x13B63()) {
            if (!field_0x55) {
                field_0x60 = 20;
            }
            field_0x55 = true;
        }

        if (field_0x55) {
            if (field_0x54) {
                gotoChangeFruits3();
            } else if (sDoFinishAnim == true) {
                gotoChangeFruits6();
            }
        }

        if (mpPart1->isInAnimFinished() && mpPart2->isInAnimFinished()) {
            mpPart1->stopInAnim();
            mpPart2->stopInAnim();
        }

        if (mpPart1->isOutAnimFinished() && mpPart2->isOutAnimFinished()) {
            mpPart1->stopOutAnim();
            mpPart2->stopOutAnim();
        }

        mStateMgr.executeState();
        mLastTime = mActualTime;
    }

    return true;
}

bool dLytMeterTimer_c::startIn2() {
    startIn();
    field_0x48 = 1;
    return true;
}

bool dLytMeterTimer_c::startOut2() {
    startOut();
    field_0x48 = 0;
    return true;
}

void dLytMeterTimer_c::startIn() {
    mpPart1->stopOutAnim();
    mpPart2->stopOutAnim();
    mpPart1->startInAnim();
    mpPart2->startInAnim();
    if (dLytMeter_c::GetMain()->isInModeMap()) {
        if (mStateMgr.isState(StateID_ChangeSiren)) {
            mpPart2->stopFlowerLoopAnim();
            mpPart2->resetFlowerLoopAnim();
            mpPart2->resetSingleAnim(TIMER_02_ANIM_FLOWER_LOOP);
            mpPart2->stopBeforeFallAnim();
            mpPart2->resetSingleAnim(TIMER_02_ANIM_BEFORE_FALL);
            mpPart2->finishDyingAnims();
            mpPart2->setPetalsToStart();
            mpPart2->finishSingleAnim(TIMER_02_ANIM_CHANGE_SIREN);
            mpPart2->enableChangeSirenAnim();
            mpPart2->calc();
            mpPart2->stopChangeSirenAnim();
            mpPart2->resetSingleAnim(TIMER_02_ANIM_CHANGE_SIREN);
            gotoSiren();
        } else if (mStateMgr.isState(StateID_ChangeSafeBloom) || mStateMgr.isState(StateID_ChangeSafe)) {
            mpPart2->stopSirenSafeAnim();
            mpPart2->finishSingleAnim(TIMER_02_ANIM_BLOOM);
            mpPart2->enableBloomAnim();
            mpPart2->calc();
            mpPart2->stopBloomAnim();
            mpPart2->setNumPetals(TIMER_02_NUM_PETALS);
            mpPart2->realizePetalsOnOff();
            mpPart2->initFallOutAnims();
            mpPart2->initDyingAnims();
            mpPart2->finishSingleAnim(TIMER_02_ANIM_SIREN_SAFE);
            mpPart2->enableSafeAnim();
            mpPart2->calc();
            mpPart2->stopSirenSafeAnim();
            gotoSafe();
        }
    }
}

void dLytMeterTimer_c::gotoChangeSiren() {
    if (mStateMgr.isState(StateID_Safe) || mStateMgr.isState(StateID_Siren)) {
        if (mpPart2->getNumPetals() > 1) {
            // Okay
            mpPart2->setNumPetals(mpPart2->getNumPetals());
            mpPart2->initDyingAnims();
            mpPart2->updatePetalsRate();
        } else if (mpPart2->getNumPetals() == 1) {
            mpPart2->updatePetalsRate();
        }
        mpPart2->setVisible(false);
        field_0x57 = 0;
        mStateMgr.changeState(StateID_ChangeSiren);
    }
}

void dLytMeterTimer_c::gotoSiren() {
    mpPart2->enableSirenLoopAnim();
    field_0x54 = true;
    mStateMgr.changeState(StateID_Siren);
}

void dLytMeterTimer_c::gotoChangeSafeBloom() {
    if (mStateMgr.isState(StateID_Siren) || mStateMgr.isState(StateID_Safe) || mStateMgr.isState(StateID_ChangeSiren) ||
        mStateMgr.isState(StateID_ChangeSafe)) {
        if (mpPart1->getActualTearCount() == TIMER_01_NUM_TEARS - 1) {
            mpPart2->stopDyingAnims();
        }
        mpPart2->stopSirenLoopAnim();
        field_0x54 = false;
        mStateMgr.changeState(StateID_ChangeSafeBloom);
    }
}

void dLytMeterTimer_c::gotoChangeSafe() {
    mpPart2->setNumPetals(TIMER_02_NUM_PETALS);
    mpPart2->realizePetalsOnOff();
    mpPart2->initFallOutAnims();
    mpPart2->initDyingAnims();
    mpPart2->resetSingleAnim(TIMER_02_ANIM_SIREN_SAFE);
    mpPart2->enableSafeAnim();
    mLastTime = mActualTime;
    if ((mLastTime / 1000) % 2 != 0) {
        mpPart2->syncTimeWithSafe();
        field_0x59 = true;
        field_0x5C = 2;
    }
    mStateMgr.changeState(StateID_ChangeSafe);
}

void dLytMeterTimer_c::gotoSafe() {
    mStateMgr.changeState(StateID_Safe);
}

void dLytMeterTimer_c::gotoChangeFruits6() {
    if (mStateMgr.isState(StateID_Siren) || mStateMgr.isState(StateID_Safe)) {
        mpPart1->resetBowlNuts();
        mpPart1->startDropLineChange(mpPart2->i_getPane());
        mpPart1->startFruitAnim(mpPart1->getChangeFruitIndex());
        mpPart1->startEffect(mpPart1->getChangeFruitIndex());
        field_0x54 = false;
        field_0x56 = 6;
        mStateMgr.changeState(StateID_ChangeFruits);
    }
}

void dLytMeterTimer_c::gotoFruits() {
    mStateMgr.changeState(StateID_Fruits);
}

void dLytMeterTimer_c::startOut() {
    mpPart1->stopInAnim();
    mpPart2->stopInAnim();
    mpPart1->startOutAnim();
    mpPart2->startOutAnim();
}

void dLytMeterTimer_c::gotoChangeFruits3() {
    if (mStateMgr.isState(StateID_Siren) || mStateMgr.isState(StateID_Safe)) {
        mpPart2->initBeforeFallAnim();
        mpPart2->setNumPetals(0);
        mpPart2->realizePetalsOnOff();
        mpPart2->enableSafeAnim();
        mpPart2->finishSingleAnim(TIMER_02_ANIM_SIREN_SAFE);
        mpPart2->resetSingleAnim(TIMER_02_ANIM_BLOOM);
        mpPart2->enableBloomAnim();
        field_0x54 = false;
        field_0x56 = 3;
        mStateMgr.changeState(StateID_ChangeFruits);
    }
}

void dLytMeterTimer_c::startSafe() {
    mpPart2->setNumPetals(TIMER_02_NUM_PETALS);
    mpPart2->realizePetalsOnOff();
    mpPart2->initFallOutAnims();
    mpPart2->initDyingAnims();
    mpPart2->resetSingleAnim(TIMER_02_ANIM_SIREN_SAFE);
    mpPart2->enableSafeAnim();
    mLastTime = mActualTime;
    field_0x56 = 4;
}

void dLytMeterTimer_c::doPickup() {
    mpPart1->resetBowlNuts();
    mpPart1->startDropLineChange(mpPart2->i_getPane());
    mpPart1->startFruitAnim(mpPart1->getChangeFruitIndex());
    mpPart1->startEffect(mpPart1->getChangeFruitIndex());
    field_0x56 = 6;
}

bool dLytMeterTimer_c::isInSiren() {
    if (EventManager::isInEvent()) {
        return false;
    }
    return mStateMgr.isState(StateID_Siren);
}

s32 dLytMeterTimer_c::getPetalForTimerMaybe(s32 time) {
    s32 result = time / 10000;
    if (time % 10000 != 0) {
        result += 1;
    }
    if (result <= TIMER_02_NUM_PETALS) {
        return result;
    }
    return TIMER_02_NUM_PETALS;
}

bool dLytMeterTimer_c::checkForPetalChangeMaybe() {
    if (mLastTime - mActualTime > 10000) {
        mpPart2->setNumPetals(getPetalForTimerMaybe(mLastTime));
        mLastTime = mActualTime;
        return true;
    }
    return false;
}
