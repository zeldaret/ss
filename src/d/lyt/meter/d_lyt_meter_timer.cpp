#include "d/lyt/meter/d_lyt_meter_timer.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_drop_line.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/t/d_t_siren.h"
#include "m/m_vec.h"
#include "nw4r/math/math_types.h"
#include "toBeSorted/effects_struct.h"

STATE_DEFINE(dLytMeterTimer_c, ChangeSiren);
STATE_DEFINE(dLytMeterTimer_c, Siren);
STATE_DEFINE(dLytMeterTimer_c, ChangeSafeBloom);
STATE_DEFINE(dLytMeterTimer_c, ChangeSafe);
STATE_DEFINE(dLytMeterTimer_c, Safe);
STATE_DEFINE(dLytMeterTimer_c, ChangeFruits);
STATE_DEFINE(dLytMeterTimer_c, Fruits);

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
    if (dLytMeterContainer_c::GetMeter()->fn_800D5650()) {
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

void LytMeterTimerPart1_c::startOutAnim() {
    mAnm[TIMER_01_ANIM_OUT].setToStart();
    mAnm[TIMER_01_ANIM_OUT].setAnimEnable(true);
}

void LytMeterTimerPart1_c::stopInAnim() {
    mAnm[TIMER_01_ANIM_IN].setAnimEnable(false);
}

void LytMeterTimerPart1_c::stopFinishedFruit() {
    for (s32 i = 0; i <= field_0x780; i++) {
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
    for (s32 i = 0; i <= field_0x780; i++) {
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
extern "C" const u16 PARTICLE_RESOURCE_ID_MAPPING_135_;
extern "C" const u16 PARTICLE_RESOURCE_ID_MAPPING_136_;
extern "C" const u16 PARTICLE_RESOURCE_ID_MAPPING_137_;
void LytMeterTimerPart1_c::createEffect(s32 fruitIndex) {
    nw4r::math::MTX34 mtx = mpPanes[fruitIndex]->GetGlobalMtx();
    mVec3_c pos;
    pos.x = mtx._03;
    pos.y = mtx._13;
    pos.z = 0.0f;
    EffectsStruct::fn_800298C0(
        PARTICLE_RESOURCE_ID_MAPPING_135_, &pos, nullptr, nullptr, &mColors1[TIMER_01_COLOR_00_CIRCLE],
        &mColors2[TIMER_01_COLOR_01_CIRCLE]
    );
    EffectsStruct::fn_800298C0(
        PARTICLE_RESOURCE_ID_MAPPING_136_, &pos, nullptr, nullptr, &mColors1[TIMER_01_COLOR_00_SHADOW],
        &mColors2[TIMER_01_COLOR_01_SHADOW]
    );
    EffectsStruct::fn_800298C0(
        PARTICLE_RESOURCE_ID_MAPPING_137_, &pos, nullptr, nullptr, &mColors1[TIMER_01_COLOR_00_RAINBOW_CIRCLE],
        &mColors2[TIMER_01_COLOR_01_RAINBOW_CIRCLE]
    );
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
    mAnm[TIMER_02_ANIM_BEFORE_FALL].setForwardOnce();
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

void LytMeterTimerPart2_c::startInAnim() {
    if (dLytMeterContainer_c::GetMeter()->fn_800D5650()) {
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

void LytMeterTimerPart2_c::initChangeFruitAnim() {
    mAnm[TIMER_02_ANIM_CHANGE_FRUIT].setForwardOnce();
    mAnm[TIMER_02_ANIM_CHANGE_FRUIT].setFrame(0.0f);
    mAnm[TIMER_02_ANIM_CHANGE_FRUIT].setAnimEnable(true);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_CHANGE_FRUIT].setAnimEnable(false);
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
                f32 targetFrame = (s32)(duration - (time / 10000.0f) * duration + 0.5f);
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

void LytMeterTimerPart2_c::realizePetalsOnOff() {
    mAnm[TIMER_02_ANIM_PETAL_ONOFF].setAnimEnable(true);
    mAnm[TIMER_02_ANIM_PETAL_ONOFF].setFrame(9 - mNumPetals);
    mLyt.calc();
    mAnm[TIMER_02_ANIM_PETAL_ONOFF].setAnimEnable(false);
}

void LytMeterTimerPart2_c::resetSingleAnim(u8 idx) {
    mAnm[idx].setToStart();
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

void dLytMeterTimer_c::initializeState_ChangeSiren() {}
void dLytMeterTimer_c::executeState_ChangeSiren() {}
void dLytMeterTimer_c::finalizeState_ChangeSiren() {}

void dLytMeterTimer_c::initializeState_Siren() {}
void dLytMeterTimer_c::executeState_Siren() {}
void dLytMeterTimer_c::finalizeState_Siren() {}

void dLytMeterTimer_c::initializeState_ChangeSafeBloom() {}
void dLytMeterTimer_c::executeState_ChangeSafeBloom() {}
void dLytMeterTimer_c::finalizeState_ChangeSafeBloom() {}

void dLytMeterTimer_c::initializeState_ChangeSafe() {}
void dLytMeterTimer_c::executeState_ChangeSafe() {}
void dLytMeterTimer_c::finalizeState_ChangeSafe() {}

void dLytMeterTimer_c::initializeState_Safe() {}
void dLytMeterTimer_c::executeState_Safe() {}
void dLytMeterTimer_c::finalizeState_Safe() {}

void dLytMeterTimer_c::initializeState_ChangeFruits() {}
void dLytMeterTimer_c::executeState_ChangeFruits() {}
void dLytMeterTimer_c::finalizeState_ChangeFruits() {}

void dLytMeterTimer_c::initializeState_Fruits() {}
void dLytMeterTimer_c::executeState_Fruits() {}
void dLytMeterTimer_c::finalizeState_Fruits() {}

bool dLytMeterTimer_c::build() {
    mpPart1 = LytMeterTimerPart1_c::GetInstance();
    mpPart2 = LytMeterTimerPart2_c::GetInstance();
    mpPart1->init();
    mpPart2->init();
    mpPart1->realizeTrial();
    mpPart2->setNumPetals(9);
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
