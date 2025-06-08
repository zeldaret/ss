#include "d/lyt/meter/d_lyt_meter_key.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_item.h"
#include "d/d_stage_mgr.h"
#include "d/lyt/d2d.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"

STATE_DEFINE(dLytMeterKakeraKey_c, Wait);
STATE_DEFINE(dLytMeterKakeraKey_c, On);
STATE_DEFINE(dLytMeterKakeraKey_c, Increase);
STATE_DEFINE(dLytMeterKakeraKey_c, Active);
STATE_DEFINE(dLytMeterKakeraKey_c, Off);

STATE_DEFINE(dLytMeterBossKey_c, Wait);
STATE_DEFINE(dLytMeterBossKey_c, On);
STATE_DEFINE(dLytMeterBossKey_c, Active);
STATE_DEFINE(dLytMeterBossKey_c, Off);

STATE_DEFINE(dLytMeterSmallKey_c, Wait);
STATE_DEFINE(dLytMeterSmallKey_c, On);
STATE_DEFINE(dLytMeterSmallKey_c, Active);
STATE_DEFINE(dLytMeterSmallKey_c, Off);

static const d2d::LytBrlanMapping brlanMapKakera[] = {
    {     "keyKakera_00_in.brlan",   "G_inOut_00"},
    {"keyKakera_00_number1.brlan", "G_number1_00"},
    {"keyKakera_00_number2.brlan", "G_number2_00"},
    {"keyKakera_00_number3.brlan", "G_number3_00"},
    {"keyKakera_00_number4.brlan", "G_number4_00"},
    {"keyKakera_00_number5.brlan", "G_number5_00"},
    {   "keyKakera_00_loop.brlan",    "G_loop_00"},
    {    "keyKakera_00_out.brlan",   "G_inOut_00"},
};

#define KEY_KAKERA_ANIM_IN 0
#define KEY_KAKERA_ANIM_NUMBER_OFFSET 1
#define KEY_KAKERA_ANIM_LOOP 6
#define KEY_KAKERA_ANIM_OUT 7

#define KEY_KAKERA_NUM_ANIMS 8

#define KEY_KAKERA_NUM_PIECES 5

void dLytMeterKakeraKey_c::initializeState_Wait() {}
void dLytMeterKakeraKey_c::executeState_Wait() {
    if (mShouldBeVisible) {
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterKakeraKey_c::finalizeState_Wait() {}

void dLytMeterKakeraKey_c::initializeState_On() {
    mAnm[KEY_KAKERA_ANIM_IN].setAnimEnable(true);
    mAnm[KEY_KAKERA_ANIM_IN].setToEnd();
    for (int i = 0; i < KEY_KAKERA_NUM_PIECES; i++) {
        if (i < mSavedKeyPieceCount) {
            mAnm[i + KEY_KAKERA_ANIM_NUMBER_OFFSET].setAnimEnable(true);
            mAnm[i + KEY_KAKERA_ANIM_NUMBER_OFFSET].setToEnd();
        }
    }

    mLyt.calc();

    for (int i = 0; i < KEY_KAKERA_NUM_PIECES; i++) {
        if (mAnm[i + KEY_KAKERA_ANIM_NUMBER_OFFSET].isEnabled()) {
            mAnm[i + KEY_KAKERA_ANIM_NUMBER_OFFSET].setAnimEnable(false);
        }
    }

    mAnm[KEY_KAKERA_ANIM_IN].setFrame(0.0f);
}
void dLytMeterKakeraKey_c::executeState_On() {
    if (mAnm[KEY_KAKERA_ANIM_IN].isEndReached()) {
        mAnm[KEY_KAKERA_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMeterKakeraKey_c::finalizeState_On() {}

void dLytMeterKakeraKey_c::initializeState_Increase() {
    mNextKeyPieceCount = mSavedKeyPieceCount + 1;
    mAnm[mNextKeyPieceCount - 1 + KEY_KAKERA_ANIM_NUMBER_OFFSET].setAnimEnable(true);
    mAnm[mNextKeyPieceCount - 1 + KEY_KAKERA_ANIM_NUMBER_OFFSET].setFrame(0.0f);
    mSavedKeyPieceCount = dAcItem_c::getKeyPieceCount();
    if (mSavedKeyPieceCount == KEY_KAKERA_NUM_PIECES) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_KEY_COMPLETE);
    } else {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_KEY_PARTS);
    }
}
void dLytMeterKakeraKey_c::executeState_Increase() {
    if (mAnm[mNextKeyPieceCount + KEY_KAKERA_ANIM_IN].isEndReached()) {
        mAnm[mNextKeyPieceCount + KEY_KAKERA_ANIM_IN].setAnimEnable(false);
        mNextKeyPieceCount = -1;
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMeterKakeraKey_c::finalizeState_Increase() {}

void dLytMeterKakeraKey_c::initializeState_Active() {}
void dLytMeterKakeraKey_c::executeState_Active() {
    if (mSavedKeyPieceCount != dAcItem_c::getKeyPieceCount()) {
        mStateMgr.changeState(StateID_Increase);
    } else if (!mShouldBeVisible) {
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterKakeraKey_c::finalizeState_Active() {}

void dLytMeterKakeraKey_c::initializeState_Off() {
    mAnm[KEY_KAKERA_ANIM_OUT].setAnimEnable(true);
    mAnm[KEY_KAKERA_ANIM_OUT].setFrame(0.0f);
}
void dLytMeterKakeraKey_c::executeState_Off() {
    if (mAnm[KEY_KAKERA_ANIM_OUT].isEndReached()) {
        mAnm[KEY_KAKERA_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMeterKakeraKey_c::finalizeState_Off() {}

bool dLytMeterKakeraKey_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("keyKakera_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < KEY_KAKERA_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapKakera[i].mFile, resAcc, mLyt.getLayout(), brlanMapKakera[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[KEY_KAKERA_ANIM_LOOP].setAnimEnable(true);
    mSavedKeyPieceCount = dAcItem_c::getKeyPieceCount();
    mNextKeyPieceCount = -1;
    mpPane = mLyt.findPane("N_inOut_00");
    nw4r::lyt::Pane *positionPane = mLyt.findPane("N_keyPosition_00");

    mPanePositions[0].copyFrom(positionPane->GetTranslate());
    mPanePositions[1].copyFrom(mpPane->GetTranslate());

    mPosititionIndex = 1;
    mOldPosititionIndex = 1;
    mMovementFrame = 0;

    mPosition.copyFrom(mPanePositions[mPosititionIndex]);
    mShouldBeVisible = 0;
    mpPane->SetTranslate(mPosition);

    mAnm[KEY_KAKERA_ANIM_IN].setFrame(0.0f);
    mAnm[KEY_KAKERA_ANIM_IN].setAnimEnable(true);
    mLyt.calc();
    mAnm[KEY_KAKERA_ANIM_IN].setAnimEnable(false);

    mStateMgr.changeState(StateID_Wait);

    return true;
}

bool dLytMeterKakeraKey_c::remove() {
    for (int i = 0; i < KEY_KAKERA_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterKakeraKey_c::execute() {
    mStateMgr.executeState();
    if (mPosition != mPanePositions[mPosititionIndex]) {
        if (mMovementFrame < 5) {
            mVec3_c diff = mPanePositions[mPosititionIndex] - mPanePositions[mOldPosititionIndex];
            mPosition = diff * cLib::easeOut(++mMovementFrame / 5.0f, 3.0f) + mPanePositions[mOldPosititionIndex];
        } else {
            mMovementFrame = 0;
            mPosition = mPanePositions[mPosititionIndex];
        }
        mpPane->SetTranslate(mPosition);
    }

    for (int i = 0; i < KEY_KAKERA_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();

    return true;
}

bool dLytMeterKakeraKey_c::draw() {
    mLyt.addToDrawList();
    return true;
}

void dLytMeterKakeraKey_c::setPosition(s32 position) {
    if (mPosititionIndex == position) {
        return;
    }

    mOldPosititionIndex = mPosititionIndex;
    mPosititionIndex = position;
    mMovementFrame = 0;
}

static const d2d::LytBrlanMapping brlanMapBoss[] = {
    {      "keyBoss_00_in.brlan",    "G_inOut_00"},
    {     "keyBoss_00_out.brlan",    "G_inOut_00"},
    { "keyBoss_00_keyBoss.brlan",  "G_keyBoss_00"},
    {    "keyBoss_00_loop.brlan",     "G_loop_00"},
    {"keyBoss_00_position.brlan", "G_position_00"},
};

#define KEY_BOSS_ANIM_IN 0
#define KEY_BOSS_ANIM_OUT 1
#define KEY_BOSS_ANIM_KEYBOSS 2
#define KEY_BOSS_ANIM_LOOP 3
#define KEY_BOSS_ANIM_POSITION 4

#define KEY_BOSS_NUM_ANIMS 5

void dLytMeterBossKey_c::initializeState_Wait() {}
void dLytMeterBossKey_c::executeState_Wait() {
    if (mShouldBeVisible) {
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterBossKey_c::finalizeState_Wait() {}

void dLytMeterBossKey_c::initializeState_On() {
    mAnm[KEY_BOSS_ANIM_IN].setAnimEnable(true);
    mAnm[KEY_BOSS_ANIM_IN].setFrame(0.0f);
}
void dLytMeterBossKey_c::executeState_On() {
    if (mAnm[KEY_BOSS_ANIM_IN].isEndReached()) {
        mAnm[KEY_BOSS_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMeterBossKey_c::finalizeState_On() {}

void dLytMeterBossKey_c::initializeState_Active() {}
void dLytMeterBossKey_c::executeState_Active() {
    if (!mShouldBeVisible) {
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterBossKey_c::finalizeState_Active() {}

void dLytMeterBossKey_c::initializeState_Off() {
    mAnm[KEY_BOSS_ANIM_OUT].setAnimEnable(true);
    mAnm[KEY_BOSS_ANIM_OUT].setFrame(0.0f);
}
void dLytMeterBossKey_c::executeState_Off() {
    if (mAnm[KEY_BOSS_ANIM_OUT].isEndReached()) {
        mAnm[KEY_BOSS_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMeterBossKey_c::finalizeState_Off() {}

bool dLytMeterBossKey_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("keyBoss_00.brlyt", nullptr);

    for (int i = 0; i < KEY_BOSS_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapBoss[i].mFile, resAcc, mLyt.getLayout(), brlanMapBoss[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[KEY_BOSS_ANIM_LOOP].setAnimEnable(true);
    mpPane = mLyt.findPane("N_keyBoss_00");
    mAnm[KEY_BOSS_ANIM_POSITION].setAnimEnable(true);

    for (int i = 0; i < 4; i++) {
        mAnm[KEY_BOSS_ANIM_POSITION].setFrame(i);
        mLyt.calc();
        mPanePositions[i].copyFrom(mLyt.findPane("N_keyPosition_00")->GetTranslate());
    }

    mAnm[KEY_BOSS_ANIM_POSITION].setAnimEnable(false);

    mPosititionIndex = 1;
    mOldPosititionIndex = 1;
    mMovementFrame = 0;

    mPosition.copyFrom(mPanePositions[mPosititionIndex]);
    mShouldBeVisible = 0;
    mpPane->SetTranslate(mPosition);

    mAnm[KEY_BOSS_ANIM_IN].setFrame(0.0f);
    mAnm[KEY_BOSS_ANIM_IN].setAnimEnable(true);

    f32 frame = 0.0f;
    if (dStageMgr_c::GetInstance()->getMapNameId() == 11) {
        frame = 0.0f;
    } else if (dStageMgr_c::GetInstance()->getMapNameId() == 14) {
        frame = 1.0f;
    } else if (dStageMgr_c::GetInstance()->getMapNameId() == 12) {
        frame = 2.0f;
    } else if (dStageMgr_c::GetInstance()->getMapNameId() == 16) {
        frame = 3.0f;
    } else if (dStageMgr_c::GetInstance()->getMapNameId() == 13) {
        frame = 4.0f;
    } else if (dStageMgr_c::GetInstance()->getMapNameId() == 15) {
        frame = 5.0f;
    }

    mAnm[KEY_BOSS_ANIM_KEYBOSS].setAnimEnable(true);
    mAnm[KEY_BOSS_ANIM_KEYBOSS].setFrame(frame);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();
    mAnm[KEY_BOSS_ANIM_IN].setAnimEnable(false);
    mAnm[KEY_BOSS_ANIM_KEYBOSS].setAnimEnable(false);

    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeterBossKey_c::remove() {
    for (int i = 0; i < KEY_BOSS_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterBossKey_c::execute() {
    mStateMgr.executeState();

    if (mPosition != mPanePositions[mPosititionIndex]) {
        if (mMovementFrame < 5) {
            mVec3_c diff = mPanePositions[mPosititionIndex] - mPanePositions[mOldPosititionIndex];
            mPosition = diff * cLib::easeOut(++mMovementFrame / 5.0f, 3.0f) + mPanePositions[mOldPosititionIndex];
        } else {
            mMovementFrame = 0;
            mPosition = mPanePositions[mPosititionIndex];
        }
        mpPane->SetTranslate(mPosition);
    }

    for (int i = 0; i < KEY_BOSS_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();

    return true;
}

bool dLytMeterBossKey_c::draw() {
    mLyt.addToDrawList();
    return true;
}

void dLytMeterBossKey_c::setPosition(s32 position) {
    if (mPosititionIndex == position) {
        return;
    }

    mOldPosititionIndex = mPosititionIndex;
    mPosititionIndex = position;
    mMovementFrame = 0;
}

static const d2d::LytBrlanMapping brlanMapSmall[] = {
    {      "keySmall_00_in.brlan",    "G_inOut_00"},
    {     "keySmall_00_out.brlan",    "G_inOut_00"},
    {"keySmall_00_keySmall.brlan", "G_keySmall_00"},
    {    "keySmall_00_loop.brlan",     "G_loop_00"},
    {"keySmall_00_position.brlan", "G_position_00"},
};

#define KEY_SMALL_ANIM_IN 0
#define KEY_SMALL_ANIM_OUT 1
#define KEY_SMALL_ANIM_KEYSMALL 2
#define KEY_SMALL_ANIM_LOOP 3
#define KEY_SMALL_ANIM_POSITION 4

#define KEY_SMALL_NUM_ANIMS 5

void dLytMeterSmallKey_c::initializeState_Wait() {}
void dLytMeterSmallKey_c::executeState_Wait() {
    if (mShouldBeVisible && dAcItem_c::getSmallKeyCount() != 0) {
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterSmallKey_c::finalizeState_Wait() {}

void dLytMeterSmallKey_c::initializeState_On() {
    mAnm[KEY_SMALL_ANIM_IN].setAnimEnable(true);
    mAnm[KEY_SMALL_ANIM_IN].setToEnd();
    mSavedSmallKeyCount = dAcItem_c::getSmallKeyCount();
    if (mSavedSmallKeyCount > 0 && mSavedSmallKeyCount <= 5) {
        mAnm[KEY_SMALL_ANIM_KEYSMALL].setFrame(mSavedSmallKeyCount - 1);
    }
    mAnm[KEY_SMALL_ANIM_IN].setFrame(0.0f);
}
void dLytMeterSmallKey_c::executeState_On() {
    if (mAnm[KEY_SMALL_ANIM_IN].isEndReached()) {
        mAnm[KEY_SMALL_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMeterSmallKey_c::finalizeState_On() {}

void dLytMeterSmallKey_c::initializeState_Active() {}
void dLytMeterSmallKey_c::executeState_Active() {
    if (mSavedSmallKeyCount != dAcItem_c::getSmallKeyCount()) {
        mSavedSmallKeyCount = dAcItem_c::getSmallKeyCount();
        if (mSavedSmallKeyCount > 0 && mSavedSmallKeyCount <= 5) {
            mAnm[KEY_SMALL_ANIM_KEYSMALL].setFrame(mSavedSmallKeyCount - 1);
        }
    }

    if (!mShouldBeVisible || dAcItem_c::getSmallKeyCount() == 0) {
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterSmallKey_c::finalizeState_Active() {}

void dLytMeterSmallKey_c::initializeState_Off() {
    mAnm[KEY_SMALL_ANIM_OUT].setAnimEnable(true);
    mAnm[KEY_SMALL_ANIM_OUT].setFrame(0.0f);
}
void dLytMeterSmallKey_c::executeState_Off() {
    if (mAnm[KEY_SMALL_ANIM_OUT].isEndReached()) {
        mAnm[KEY_SMALL_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMeterSmallKey_c::finalizeState_Off() {}

bool dLytMeterSmallKey_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("keySmall_00.brlyt", nullptr);

    for (int i = 0; i < KEY_SMALL_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapSmall[i].mFile, resAcc, mLyt.getLayout(), brlanMapSmall[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[KEY_SMALL_ANIM_LOOP].setAnimEnable(true);
    mAnm[KEY_SMALL_ANIM_KEYSMALL].setAnimEnable(true);
    mpPane = mLyt.findPane("N_keyAll_00");
    mAnm[KEY_SMALL_ANIM_POSITION].setAnimEnable(true);

    for (int i = 0; i < 4; i++) {
        mAnm[KEY_SMALL_ANIM_POSITION].setFrame(i);
        mLyt.calc();
        mPanePositions[i].copyFrom(mLyt.findPane("N_keyPosition_00")->GetTranslate());
    }

    mAnm[KEY_SMALL_ANIM_POSITION].setAnimEnable(false);

    mPosititionIndex = 1;
    mOldPosititionIndex = 1;
    mMovementFrame = 0;

    mPosition.copyFrom(mPanePositions[mPosititionIndex]);
    mShouldBeVisible = 0;
    mSavedSmallKeyCount = 0;
    mpPane->SetTranslate(mPosition);

    mAnm[KEY_SMALL_ANIM_IN].setFrame(0.0f);
    mAnm[KEY_SMALL_ANIM_IN].setAnimEnable(true);
    mLyt.calc();
    mAnm[KEY_SMALL_ANIM_IN].setAnimEnable(false);

    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeterSmallKey_c::remove() {
    for (int i = 0; i < KEY_SMALL_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterSmallKey_c::execute() {
    mStateMgr.executeState();

    if (mPosition != mPanePositions[mPosititionIndex]) {
        if (mMovementFrame < 5) {
            mVec3_c diff = mPanePositions[mPosititionIndex] - mPanePositions[mOldPosititionIndex];
            mPosition = diff * cLib::easeOut(++mMovementFrame / 5.0f, 3.0f) + mPanePositions[mOldPosititionIndex];
        } else {
            mMovementFrame = 0;
            mPosition = mPanePositions[mPosititionIndex];
        }
        mpPane->SetTranslate(mPosition);
    }

    for (int i = 0; i < KEY_SMALL_NUM_ANIMS; i++) {
        if (i != KEY_SMALL_ANIM_KEYSMALL && mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();

    return true;
}

bool dLytMeterSmallKey_c::draw() {
    mLyt.addToDrawList();
    return true;
}

void dLytMeterSmallKey_c::setPosition(s32 position) {
    if (mPosititionIndex == position) {
        return;
    }

    mOldPosititionIndex = mPosititionIndex;
    mPosititionIndex = position;
    mMovementFrame = 0;
}
