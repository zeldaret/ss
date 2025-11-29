#include "d/lyt/meter/d_lyt_meter_rupy.h"

#include "c/c_math.h"
#include "d/a/d_a_item.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_meter_hio.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "nw4r/math/math_types.h"

STATE_DEFINE(dLytMeterRupyStart_c, Wait);
STATE_DEFINE(dLytMeterRupyStart_c, Start);

STATE_DEFINE(dLytMeterRupyBlink_c, Wait);
STATE_DEFINE(dLytMeterRupyBlink_c, Flash);

STATE_DEFINE(dLytMeterRupy_c, In);
STATE_DEFINE(dLytMeterRupy_c, Active);
STATE_DEFINE(dLytMeterRupy_c, Start);
STATE_DEFINE(dLytMeterRupy_c, Out);

void dLytMeterRupyStart_c::initializeState_Wait() {}
void dLytMeterRupyStart_c::executeState_Wait() {
    if (mShouldStart) {
        mpAnm->setForwardOnce();
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_Start);
    }
}
void dLytMeterRupyStart_c::finalizeState_Wait() {}

void dLytMeterRupyStart_c::initializeState_Start() {}
void dLytMeterRupyStart_c::executeState_Start() {
    if (mpAnm->isEndReached()) {
        mpAnm->setAnimEnable(false);
        mShouldStart = false;
        mStateMgr.changeState(StateID_Wait);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterRupyStart_c::finalizeState_Start() {}

void dLytMeterRupyStart_c::init() {
    mShouldStart = false;
    mStateMgr.changeState(StateID_Wait);
}

void dLytMeterRupyStart_c::execute() {
    mStateMgr.executeState();
}

void dLytMeterRupyBlink_c::initializeState_Wait() {}
void dLytMeterRupyBlink_c::executeState_Wait() {
    if (mShouldFlash) {
        mpAnm->setForwardOnce();
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_Flash);
    }
}
void dLytMeterRupyBlink_c::finalizeState_Wait() {}

void dLytMeterRupyBlink_c::initializeState_Flash() {}
void dLytMeterRupyBlink_c::executeState_Flash() {
    if (mImmediatelyCancelFlash) {
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(false);
        mImmediatelyCancelFlash = false;
        mShouldFlash = false;
        mStateMgr.changeState(StateID_Wait);
    } else {
        if (mpAnm->isEndReached()) {
            mpAnm->setAnimEnable(false);
            mShouldFlash = false;
            mStateMgr.changeState(StateID_Wait);
        }

        if (mpAnm->isEnabled()) {
            mpAnm->play();
        }
    }
}
void dLytMeterRupyBlink_c::finalizeState_Flash() {}

void dLytMeterRupyBlink_c::init() {
    mShouldFlash = false;
    mImmediatelyCancelFlash = false;
    mStateMgr.changeState(StateID_Wait);
}

void dLytMeterRupyBlink_c::execute() {
    mStateMgr.executeState();
}

static const d2d::LytBrlanMapping brlanMap[] = {
    {         "rupy_01_in.brlan",   "G_inOut_00"},
    { "rupy_01_sizeChange.brlan", "G_sizeChange"},
    {       "rupy_01_keta.brlan",    "G_keta_00"},
    {      "rupy_01_color.brlan",   "G_color_00"},
    {     "rupy_01_number.brlan",  "G_number_00"},
    {     "rupy_01_number.brlan",  "G_number_01"},
    {     "rupy_01_number.brlan",  "G_number_02"},
    {     "rupy_01_number.brlan",  "G_number_03"},
    {       "rupy_01_loop.brlan",    "G_loop_00"},
    {"rupy_01_randomFlash.brlan",  "G_number_00"},
    {"rupy_01_randomFlash.brlan",  "G_number_01"},
    {"rupy_01_randomFlash.brlan",  "G_number_02"},
    {"rupy_01_randomFlash.brlan",  "G_number_03"},
    {       "rupy_01_jump.brlan",  "G_number_00"},
    {       "rupy_01_jump.brlan",  "G_number_01"},
    {       "rupy_01_jump.brlan",  "G_number_02"},
    {       "rupy_01_jump.brlan",  "G_number_03"},
    {      "rupy_01_start.brlan",  "G_number_00"},
    {      "rupy_01_start.brlan",  "G_number_01"},
    {      "rupy_01_start.brlan",  "G_number_02"},
    {      "rupy_01_start.brlan",  "G_number_03"},
    {        "rupy_01_out.brlan",   "G_inOut_00"},
};

#define RUPY_ANIM_IN 0
#define RUPY_ANIM_SIZE_CHANGE 1
#define RUPY_ANIM_KETA 2
#define RUPY_ANIM_COLOR 3
#define RUPY_ANIM_NUMBER_OFFSET 4
#define RUPY_ANIM_LOOP 8
#define RUPY_ANIM_FLASH_OFFSET 9
#define RUPY_ANIM_JUMP_OFFSET 13
#define RUPY_ANIM_START_OFFSET 17
#define RUPY_ANIM_OUT 21

#define RUPY_NUM_ANIMS 22

#define RUPY_NUM_DIGITS 4

static const char *sPaneNames[] = {
    "N_ketaT_00",
    "N_ketaT_01",
    "N_ketaT_02",
    "N_ketaT_03",
};

void dLytMeterRupy_c::initializeState_In() {
    mAnm[RUPY_ANIM_OUT].setAnimEnable(false);
    mAnm[RUPY_ANIM_OUT].setFrame(0.0f);

    mAnm[RUPY_ANIM_IN].setForwardOnce();
    if (field_0x8A9 == 1) {
        mAnm[RUPY_ANIM_IN].setFrame(0.0f);
    } else {
        mAnm[RUPY_ANIM_IN].setToEnd();
    }

    for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
        if (mAnm[i + RUPY_ANIM_FLASH_OFFSET].isEnabled()) {
            mBlinkParts[i].cancelFlash();
        } else {
            mAnm[i + RUPY_ANIM_FLASH_OFFSET].setAnimEnable(true);
        }

        mAnm[i + RUPY_ANIM_FLASH_OFFSET].setFrame(0.0f);
    }

    mLyt.calc();

    for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
        mAnm[i + RUPY_ANIM_FLASH_OFFSET].setAnimEnable(false);
    }
    field_0x8A9 = 0;
    mAnm[RUPY_ANIM_IN].setAnimEnable(true);
    field_0x8AB = 1;
}
void dLytMeterRupy_c::executeState_In() {
    if (mAnm[RUPY_ANIM_IN].isEndReached()) {
        mAnm[RUPY_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMeterRupy_c::finalizeState_In() {}

void dLytMeterRupy_c::initializeState_Active() {}
void dLytMeterRupy_c::executeState_Active() {
    bool result = updateDisplayedAmount(false);
    if (field_0x890 == 0) {
        mStateMgr.changeState(StateID_Start);
    } else if (!result && field_0x890 != -1) {
        field_0x890 = -1;
    }
}
void dLytMeterRupy_c::finalizeState_Active() {}

void dLytMeterRupy_c::initializeState_Start() {
    for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
        if (i < mNumDisplayedDigits) {
            mStartParts[i].setShouldStart(true);
        }
    }
}
void dLytMeterRupy_c::executeState_Start() {
    updateDisplayedAmount(false);
    bool bAllDone = true;
    for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
        if (mStartParts[i].isStarting()) {
            bAllDone = false;
        }
    }

    if (bAllDone) {
        mStateMgr.changeState(StateID_Active);
    }

    field_0x890++;
}
void dLytMeterRupy_c::finalizeState_Start() {}

void dLytMeterRupy_c::initializeState_Out() {
    mAnm[RUPY_ANIM_OUT].setForwardOnce();
    if (field_0x8AA == 1 || field_0x8AA == 3) {
        mAnm[RUPY_ANIM_OUT].setFrame(0.0f);
    } else {
        mAnm[RUPY_ANIM_OUT].setToEnd();
    }

    if (field_0x8AA >= 3 && field_0x8AA < 5) {
        field_0x8AE = 1;
    } else {
        field_0x8AE = 0;
    }

    field_0x8AA = 0;
    mAnm[RUPY_ANIM_OUT].setAnimEnable(true);
}
void dLytMeterRupy_c::executeState_Out() {
    if (mAnm[RUPY_ANIM_OUT].isEndReached()) {
        field_0x8AB = 0;

        if (field_0x8AE == 0 && hasChangeInRupees()) {
            mDisplayedRupeeCount = dAcItem_c::getRupeeCounter();
            updateDisplayedAmount(true);
        }

        mAnm[RUPY_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMeterRupy_c::finalizeState_Out() {}

bool dLytMeterRupy_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("rupy_01.brlyt", nullptr);

    for (int i = 0; i < RUPY_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
        mStartParts[i].setAnm(&mAnm[i + RUPY_ANIM_START_OFFSET]);
        mStartParts[i].init();
        mBlinkParts[i].setAnm(&mAnm[i + RUPY_ANIM_FLASH_OFFSET]);
        mBlinkParts[i].init();
    }

    for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
        mpPanes[i] = mLyt.findPane(sPaneNames[i]);
    }

    mAnm[RUPY_ANIM_SIZE_CHANGE].setAnimEnable(true);
    mAnm[RUPY_ANIM_COLOR].setAnimEnable(true);
    mAnm[RUPY_ANIM_LOOP].setAnimEnable(true);

    for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
        mAnm[i + RUPY_ANIM_NUMBER_OFFSET].setAnimEnable(true);
        mAnm[i + RUPY_ANIM_JUMP_OFFSET].setAnimEnable(true);
        mAnm[i + RUPY_ANIM_JUMP_OFFSET].setFrame(0.0f);
    }

    mSize = 1;
    mAnm[RUPY_ANIM_SIZE_CHANGE].setFrame(mSize);
    mRupeeColor = 0;
    mAnm[RUPY_ANIM_COLOR].setFrame(mRupeeColor);
    mAnm[RUPY_ANIM_IN].setAnimEnable(true);
    mAnm[RUPY_ANIM_IN].setToEnd();
    mLyt.calc();
    mAnm[RUPY_ANIM_IN].setAnimEnable(false);

    field_0x8AB = 0;
    mDisplayedRupeeCount = dAcItem_c::getRupeeCounter();
    field_0x890 = -1;
    mBlinkDelay = cM::rndF(10.0f);
    for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
        mJumpState[i] = 0xFF;
        mDisplayedDigits[i] = 0;
        mPrevDigits[i] = 0;
    }

    field_0x8A9 = 0;
    field_0x8AA = 0;
    mNumDisplayedDigits = 0;
    field_0x8AC = 0;
    field_0x8AD = 0;

    field_0x8AE = 0;
    updateDisplayedAmount(true);

    mStateMgr.changeState(StateID_Active);
    return true;
}

bool dLytMeterRupy_c::remove() {
    for (int i = 0; i < RUPY_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterRupy_c::execute() {
    if (field_0x8A9 != 0) {
        mStateMgr.changeState(StateID_In);
    } else if (field_0x8AA != 0) {
        mStateMgr.changeState(StateID_Out);
    }

    mStateMgr.executeState();
    for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
        mStartParts[i].execute();
        mBlinkParts[i].execute();
    }

    for (s32 i = 0; i < RUPY_NUM_ANIMS; i++) {
        switch (i) {
            case RUPY_ANIM_SIZE_CHANGE:
            case RUPY_ANIM_KETA:
            case RUPY_ANIM_COLOR:
            case RUPY_ANIM_NUMBER_OFFSET + 0:
            case RUPY_ANIM_NUMBER_OFFSET + 1:
            case RUPY_ANIM_NUMBER_OFFSET + 2:
            case RUPY_ANIM_NUMBER_OFFSET + 3:
            case RUPY_ANIM_JUMP_OFFSET + 0:
            case RUPY_ANIM_JUMP_OFFSET + 1:
            case RUPY_ANIM_JUMP_OFFSET + 2:
            case RUPY_ANIM_JUMP_OFFSET + 3:
            case RUPY_ANIM_START_OFFSET + 0:
            case RUPY_ANIM_START_OFFSET + 1:
            case RUPY_ANIM_START_OFFSET + 2:
            case RUPY_ANIM_START_OFFSET + 3:  continue;
        }
        // Yes, really
        u32 idx = i;
        if (mAnm[idx].isEnabled()) {
            mAnm[idx].play();
        }
    }

    mAnm[RUPY_ANIM_SIZE_CHANGE].setFrame(mSize);

    bool isMax = false;
    u32 current = mDisplayedRupeeCount;
    u32 capacity = dAcItem_c::getCurrentWalletCapacity();
    if (current != 0 && capacity == current) {
        isMax = true;
    };
    if (isMax != mRupeeColor) {
        mRupeeColor = isMax;
        mAnm[RUPY_ANIM_COLOR].setFrame(mRupeeColor);
    }

    return true;
}

bool dLytMeterRupy_c::hasChangeInRupees() const {
    s32 amount = dAcItem_c::getRupeeCounter();
    if (amount > 9999) {
        amount = 9999;
    } else if (amount < 0) {
        amount = 0;
    }
    return amount != mDisplayedRupeeCount;
}

s32 dLytMeterRupy_c::getRupeeDifference() const {
    return dAcItem_c::getRupeeCounter() - mDisplayedRupeeCount;
}

mVec3_c dLytMeterRupy_c::getLastVisibleDigitPosition() {
    mVec3_c ret(0.0f, 0.0f, 0.0f);
    u32 idx = mNumDisplayedDigits - 1;
    if (idx <= 3) {
        nw4r::math::MTX34 mtx = mpPanes[idx]->GetGlobalMtx();
        ret.x = mtx._03;
        ret.y = mtx._13;
    }

    return ret;
}

void dLytMeterRupy_c::setDigit(s32 index, s32 digit) {
    mDisplayedDigits[index] = digit;
    s32 num = index + RUPY_ANIM_NUMBER_OFFSET;
    mAnm[num].setFrame(digit);
}

bool dLytMeterRupy_c::updateDisplayedAmount(bool suppressSound) {
    s32 amount = dAcItem_c::getRupeeCounter();
    if (amount > 9999) {
        amount = 9999;
    } else if (amount < 0) {
        amount = 0;
    }

    s32 newNumDisplayed = amount;
    if (amount != mDisplayedRupeeCount || suppressSound) {
        bool b = false;
        if (amount < mDisplayedRupeeCount) {
            if (mDisplayedRupeeCount - amount >= dLytMeter_HIO_c::sInstance->getField_0x1DC()) {
                newNumDisplayed = mDisplayedRupeeCount - dLytMeter_HIO_c::sInstance->getField_0x1E4();
            } else if (mDisplayedRupeeCount - amount >= dLytMeter_HIO_c::sInstance->getField_0x1D8()) {
                newNumDisplayed = mDisplayedRupeeCount - dLytMeter_HIO_c::sInstance->getField_0x1E0();
            } else {
                newNumDisplayed = mDisplayedRupeeCount - 1;
            }
            b = true;
            if (field_0x8AB) {
                if (newNumDisplayed == amount) {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_RUPEE_COUNT_DOWN_END);
                } else {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_RUPEE_COUNT_DOWN);
                }
            }
            if (field_0x8AD) {
                field_0x8AD = 0;
            }
        } else if (amount > mDisplayedRupeeCount) {
            if (amount - mDisplayedRupeeCount >= dLytMeter_HIO_c::sInstance->getField_0x1DC()) {
                newNumDisplayed = mDisplayedRupeeCount + dLytMeter_HIO_c::sInstance->getField_0x1E4();
            } else if (amount - mDisplayedRupeeCount >= dLytMeter_HIO_c::sInstance->getField_0x1D8()) {
                newNumDisplayed = mDisplayedRupeeCount + dLytMeter_HIO_c::sInstance->getField_0x1E0();
            } else {
                newNumDisplayed = mDisplayedRupeeCount + 1;
            }
            b = true;
            if (field_0x8AB) {
                if (newNumDisplayed == amount) {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_RUPEE_COUNT_UP_END);
                    if (field_0x8AD) {
                        field_0x890 = 0;
                        field_0x8AD = 0;
                    }
                } else {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_RUPEE_COUNT_UP);
                }
            }
        }
        if (newNumDisplayed > 9999) {
            newNumDisplayed = 9999;
        } else if (newNumDisplayed < 0) {
            newNumDisplayed = 0;
        }

        s32 newNumDigits = 0;
        if (newNumDisplayed >= 1000) {
            newNumDigits = 4;
            setDigit(0, newNumDisplayed / 1000);
            s32 rem = (newNumDisplayed % 1000);
            setDigit(1, rem / 100);
            rem = rem % 100;
            setDigit(2, rem / 10);
            setDigit(3, rem % 10);
        } else if (newNumDisplayed >= 100) {
            newNumDigits = 3;
            setDigit(0, newNumDisplayed / 100);
            s32 rem = (newNumDisplayed % 100);
            setDigit(1, rem / 10);
            setDigit(2, rem % 10);
        } else if (newNumDisplayed >= 10) {
            newNumDigits = 2;
            setDigit(0, newNumDisplayed / 10);
            setDigit(1, newNumDisplayed % 10);
        } else {
            newNumDigits = 1;
            setDigit(0, newNumDisplayed);
        }

        if (mNumDisplayedDigits > newNumDigits) {
            if (mAnm[mNumDisplayedDigits - 1 + RUPY_ANIM_FLASH_OFFSET].isEnabled()) {
                mBlinkParts[mNumDisplayedDigits - 1].cancelFlash();
                mAnm[mNumDisplayedDigits - 1 + RUPY_ANIM_FLASH_OFFSET].setFrame(0.0f);
                mLyt.calc();
            }
        }

        mNumDisplayedDigits = newNumDigits;
        mAnm[RUPY_ANIM_KETA].setAnimEnable(true);
        mAnm[RUPY_ANIM_KETA].setFrame(mNumDisplayedDigits - 1);

        if (*mStateMgr.getStateID() == StateID_Active) {
            executeDigitJump();
        }

        if (field_0x8AB && b) {
            executeDigitBlink(newNumDisplayed);
        }

        mDisplayedRupeeCount = newNumDisplayed;
        mPrevDigits[0] = mDisplayedDigits[0];
        mPrevDigits[1] = mDisplayedDigits[1];
        mPrevDigits[2] = mDisplayedDigits[2];
        mPrevDigits[3] = mDisplayedDigits[3];
    } else if (field_0x8AD && amount == dAcItem_c::getCurrentWalletCapacity()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_RUPEE_MAX);
        field_0x890 = 0;
        field_0x8AD = 0;
    }

    if (mDisplayedRupeeCount == amount) {
        bool b = false;
        for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
            if (mJumpState[i] != 0xFF) {
                mJumpState[i] = 0xFF;
                b = true;
            }
        }
        if (b) {
            for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
                mAnm[i + RUPY_ANIM_JUMP_OFFSET].setFrame(0.0f);
            }
            mBlinkDelay = cM::rndF(10.0f);
        }
        return false;
    } else {
        return true;
    }
}

void dLytMeterRupy_c::executeDigitJump() {
    // Is this not possibly out of bounds???
    s32 last = mNumDisplayedDigits + 3;
    if (mPrevDigits[last] == 1) {
        mAnm[mNumDisplayedDigits - 1 + RUPY_ANIM_JUMP_OFFSET].setFrame(2.0f);
        s32 last = mNumDisplayedDigits + 3;
        mPrevDigits[last] = 0;
    } else {
        mAnm[mNumDisplayedDigits - 1 + RUPY_ANIM_JUMP_OFFSET].setFrame(1.0f);
        s32 last = mNumDisplayedDigits + 3;
        mPrevDigits[last] = 1;
    }

    if (mNumDisplayedDigits >= 4) {
        s32 last = mNumDisplayedDigits - 4;
        if (mDisplayedDigits[last] != mPrevDigits[last] && mJumpState[last] == 0xFF) {
            mJumpState[last] = 0;
        }
    }
    if (mNumDisplayedDigits >= 3) {
        s32 last = mNumDisplayedDigits - 3;
        if (mDisplayedDigits[last] != mPrevDigits[last] && mJumpState[last] == 0xFF) {
            mJumpState[last] = 0;
        }
    }
    if (mNumDisplayedDigits >= 2) {
        s32 last = mNumDisplayedDigits - 2;
        if (mDisplayedDigits[last] != mPrevDigits[last] && mJumpState[last] == 0xFF) {
            mJumpState[last] = 0;
        }
    }

    for (int i = 0; i < RUPY_NUM_DIGITS; i++) {
        if (mJumpState[i] == 0xFF) {
            mAnm[i + RUPY_ANIM_JUMP_OFFSET].setFrame(0.0f);
        } else if (i != mNumDisplayedDigits - 1) {
            if (mJumpState[i] == 0) {
                mAnm[i + RUPY_ANIM_JUMP_OFFSET].setFrame(1.0f);
                mJumpState[i] = 1;
            } else if (mJumpState[i] == 1) {
                mAnm[i + RUPY_ANIM_JUMP_OFFSET].setFrame(2.0f);
                mJumpState[i] = 2;
            } else if (mJumpState[i] == 2) {
                mAnm[i + RUPY_ANIM_JUMP_OFFSET].setFrame(0.0f);
                mJumpState[i] = 0xFF;
            }
        }
    }
}

void dLytMeterRupy_c::executeDigitBlink(s32 amount) {
    if (amount >= 1000 && amount % 1000 == 0) {
        mBlinkParts[0].doFlash();
    }

    if (amount >= 100 && amount % 100 == 0) {
        if (amount >= 1000) {
            mBlinkParts[1].doFlash();
        } else {
            mBlinkParts[0].doFlash();
        }
    }

    mBlinkDelay--;
    if (mBlinkDelay <= 0) {
        if (mNumDisplayedDigits == 1) {
            mBlinkParts[mNumDisplayedDigits - 1].doFlash();
        } else {
            if (cM::rndF(100.0f) >= 50.0f) {
                mBlinkParts[mNumDisplayedDigits - 1].doFlash();
            } else {
                mBlinkParts[mNumDisplayedDigits - 2].doFlash();
            }
        }
        mBlinkDelay = mAnm[RUPY_ANIM_FLASH_OFFSET + 0].getAnimDuration() + cM::rndF(10.0f);
    }
}
