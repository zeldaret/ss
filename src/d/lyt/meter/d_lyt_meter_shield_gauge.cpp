#include "d/lyt/meter/d_lyt_meter_shield_gauge.h"

#include "common.h"
#include "d/a/d_a_itembase.h"
#include "d/d_pouch.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_meter_hio.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "toBeSorted/file_manager.h"

STATE_DEFINE(dLytMeterShieldGauge_c, Normal);
STATE_DEFINE(dLytMeterShieldGauge_c, Use);
STATE_DEFINE(dLytMeterShieldGauge_c, ToMax);

static const d2d::LytBrlanMapping brlanMap[] = {
    {       "shield_00_break.brlan",      "G_break_00"},
    {       "shield_00_level.brlan",      "G_level_00"},
    {"shield_00_shieldUpDown.brlan",     "G_shield_00"},
    {  "shield_00_shieldType.brlan", "G_shieldType_00"},
    {   "shield_00_shieldUse.brlan",     "G_shield_01"},
    {       "shield_00_toMax.brlan",      "G_toMax_00"},
    {        "shield_00_loop.brlan",       "G_loop_00"},
    {        "shield_00_loop.brlan",       "G_loop_01"},
};

#define SHIELD_ANIM_BREAK 0
#define SHIELD_ANIM_LEVEL 1
#define SHIELD_ANIM_UPDOWN 2
#define SHIELD_ANIM_TYPE 3
#define SHIELD_ANIM_USE 4
#define SHIELD_ANIM_TO_MAX 5
#define SHIELD_ANIM_LOOP_0 6
#define SHIELD_ANIM_LOOP_1 7

#define SHIELD_NUM_ANIMS 8

// Incredible use of the state manager here
void dLytMeterShieldGauge_c::initializeState_Normal() {}
void dLytMeterShieldGauge_c::executeState_Normal() {}
void dLytMeterShieldGauge_c::finalizeState_Normal() {}

void dLytMeterShieldGauge_c::initializeState_Use() {}
void dLytMeterShieldGauge_c::executeState_Use() {}
void dLytMeterShieldGauge_c::finalizeState_Use() {}

void dLytMeterShieldGauge_c::initializeState_ToMax() {}
void dLytMeterShieldGauge_c::executeState_ToMax() {}
void dLytMeterShieldGauge_c::finalizeState_ToMax() {}

bool dLytMeterShieldGauge_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("shield_00.brlyt", nullptr);

    for (int i = 0; i < SHIELD_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mpOwnerPane = nullptr;
    field_0x31D = 1;
    mCurrentDurability = 0.0f;
    field_0x2E4 = 0.0f;
    mSavedDurability = 0.0f;
    mAnimatingDurability = 0.0f;

    mCurrentShieldPouchSlot = convertFilePouchSlot(FileManager::GetInstance()->getShieldPouchSlot());
    field_0x300 = mAnimatingDurability;
    field_0x304 = mCurrentDurability;
    field_0x2FC = dLytMeter_HIO_c::GetInstance()->getField_0x1C8();
    mMaxDurability = 80.0f;
    field_0x2F4 = 80.0f;

    field_0x2F8 = mAnm[SHIELD_ANIM_USE].getAnimDuration() - 1.0f;
    field_0x30D = 0;
    field_0x30E = 0;
    field_0x30C = 0;
    field_0x31F = 0;
    field_0x31E = 0;
    field_0x318 = 0;
    mShieldId = 0;
    mSavedShieldType = -1;

    mAnm[SHIELD_ANIM_UPDOWN].setFrame(mCurrentDurability);
    mAnm[SHIELD_ANIM_UPDOWN].setAnimEnable(true);
    field_0x2E4 = calcUpdownRatio(mCurrentDurability);
    mAnm[SHIELD_ANIM_USE].setFrame(field_0x2F8 - field_0x2E4);
    mAnm[SHIELD_ANIM_USE].setAnimEnable(true);
    setLevel(mMaxDurability);
    mAnm[SHIELD_ANIM_LEVEL].setAnimEnable(true);
    mAnm[SHIELD_ANIM_TYPE].setFrame(getLytFrameForShield(0));
    mAnm[SHIELD_ANIM_TYPE].setAnimEnable(true);
    mAnm[SHIELD_ANIM_LOOP_0].setAnimEnable(true);
    mAnm[SHIELD_ANIM_LOOP_1].setAnimEnable(true);

    mAnm[SHIELD_ANIM_BREAK].setToEnd();
    mAnm[SHIELD_ANIM_BREAK].setAnimEnable(true);

    mLyt.calc();

    mAnm[SHIELD_ANIM_UPDOWN].setAnimEnable(false);
    mAnm[SHIELD_ANIM_LEVEL].setAnimEnable(false);
    mAnm[SHIELD_ANIM_TYPE].setAnimEnable(false);
    mAnm[SHIELD_ANIM_BREAK].setAnimEnable(false);
    field_0x31C = 1;
    field_0x30C = 1;
    mStateMgr.changeState(StateID_Normal);
    return true;
}

bool dLytMeterShieldGauge_c::remove() {
    for (int i = 0; i < 8; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterShieldGauge_c::execute() {
    if (!mpOwnerPane) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        return true;
    }

    if (mCurrentDurability == 0.0f) {
        field_0x318 = 2;
    }

    bool bIsAnimatingDurability = false;
    bool b2 = true;
    bool b3 = false;
    bool b4 = false;

    if (mShieldId != mSavedShieldType ||
        mCurrentShieldPouchSlot != convertFilePouchSlot(FileManager::GetInstance()->getShieldPouchSlot())) {
        mLyt.findPane("N_alpha_00")->SetVisible(true);
        mSavedShieldType = mShieldId;
        mAnm[SHIELD_ANIM_TYPE].setFrame(getLytFrameForShield(mShieldId));
        mAnm[SHIELD_ANIM_TYPE].setAnimEnable(true);
        mAnm[SHIELD_ANIM_LOOP_0].setFrame(0.0f);
        field_0x31D = 1;
        b2 = false;
        b3 = true;
        mSavedDurability = mCurrentDurability;
        mCurrentShieldPouchSlot = convertFilePouchSlot(FileManager::GetInstance()->getShieldPouchSlot());
    }

    if (mCurrentDurability != mSavedDurability || field_0x31D) {
        if (mCurrentDurability < mSavedDurability) {
            f32 f = calcUpdownRatio(mCurrentDurability);
            mSavedDurability = mCurrentDurability;
            mAnm[SHIELD_ANIM_UPDOWN].setFrame(f);
            mAnm[SHIELD_ANIM_UPDOWN].setAnimEnable(true);
            bIsAnimatingDurability = true;
            if (field_0x31F && field_0x31D) {
                field_0x31F = 0;
                bIsAnimatingDurability = false;
            }
            field_0x31E = 0;
            b4 = true;
        } else {
            bIsAnimatingDurability = true;
            mAnimatingDurability += 1.0f;
            if (field_0x31F && field_0x31D) {
                field_0x31F = 0;
                bIsAnimatingDurability = false;
            }
            if (mAnimatingDurability >= mCurrentDurability || field_0x31D) {
                mAnimatingDurability = mCurrentDurability;
            }
            if (mCurrentDurability < mAnimatingDurability) {
                // @bug (?) Unreachable: `mCurrentDurability < mAnimatingDurability` => `mAnimatingDurability >= mCurrentDurability`,
                // so the above block sets `mAnimatingDurability = mCurrentDurability` and this condition will never be hit.
                field_0x31E = 1;
            }

            if (!field_0x31D) {
                if (mCurrentDurability >= mMaxDurability) {
                    if (field_0x31E) {
                        // Unreachable?
                        dSndSmallEffectMgr_c::GetInstance()->playSoundWithPitch(SE_S_GAUGE_SHIELD_UP_LV, 1.0f);
                    }
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GAUGE_SHIELD_UP_MAX);
                } else {
                    if (field_0x31E) {
                        // Unreachable?
                        dSndSmallEffectMgr_c::GetInstance()->playSoundWithPitch(
                            SE_S_GAUGE_SHIELD_UP_LV, mCurrentDurability / mMaxDurability
                        );
                    }
                }
            }
            f32 tmp = calcUpdownRatio(mAnimatingDurability);
            field_0x2E4 = tmp;
            if (!field_0x31F) {
                mAnm[SHIELD_ANIM_USE].setFrame(field_0x2F8 - field_0x2E4);
                mAnm[SHIELD_ANIM_USE].setAnimEnable(true);
            }
            mSavedDurability = mAnimatingDurability;
            mAnm[SHIELD_ANIM_UPDOWN].setFrame(tmp);
            mAnm[SHIELD_ANIM_UPDOWN].setAnimEnable(true);
        }

        if (FileManager::GetInstance()->getShieldPouchSlot() == 8) {
            if (mCurrentDurability <= 0.0f && b4) {
                mAnm[SHIELD_ANIM_BREAK].setFrame(0.0f);
                mAnm[SHIELD_ANIM_BREAK].setAnimEnable(true);
                field_0x30D = 1;
            } else {
                mAnm[SHIELD_ANIM_BREAK].setToEnd();
                mAnm[SHIELD_ANIM_BREAK].setAnimEnable(true);
                mLyt.calc();
                mAnm[SHIELD_ANIM_BREAK].setAnimEnable(false);
                field_0x30C = 1;
            }
        } else {
            if (mCurrentDurability <= 0.0f && b4) {
                mAnm[SHIELD_ANIM_BREAK].setFrame(0.0f);
                mAnm[SHIELD_ANIM_BREAK].setAnimEnable(true);
                field_0x30D = 1;
            } else if (b2 && mCurrentDurability >= mMaxDurability) {
                mAnm[SHIELD_ANIM_TO_MAX].setFrame(0.0f);
                mAnm[SHIELD_ANIM_TO_MAX].setAnimEnable(true);
                field_0x30E = 1;
            }
        }
    }

    f32 f1 = dLytMeter_HIO_c::GetInstance()->getField_0x1CC() * 40.0f / 4.0f;
    f32 f2 = dLytMeter_HIO_c::GetInstance()->getField_0x1D0() * 40.0f / 4.0f;
    f32 f3 = dLytMeter_HIO_c::GetInstance()->getField_0x1C4() * 40.0f / 4.0f;

    (void)calcUpdownRatio(mCurrentDurability);

    if (field_0x31F) {
        if (mCurrentDurability < field_0x304 && mCurrentDurability < mAnimatingDurability - f1) {
            field_0x2FC = dLytMeter_HIO_c::GetInstance()->getField_0x1C8();
            field_0x300 = mAnimatingDurability;
            field_0x304 = mCurrentDurability;
            field_0x2E4 = calcUpdownRatio(mAnimatingDurability);
            mAnm[SHIELD_ANIM_USE].setFrame(field_0x2F8 - field_0x2E4);
            mAnm[SHIELD_ANIM_USE].setAnimEnable(true);
        } else {
            if (field_0x2FC > 0) {
                field_0x2FC--;
            }
            if (field_0x2FC == 0) {
                field_0x300 = field_0x300 - f2;
                if (field_0x300 < mCurrentDurability) {
                    field_0x31F = 0;
                    field_0x300 = mCurrentDurability;
                }
                mAnm[SHIELD_ANIM_USE].setAnimEnable(true);
                field_0x2E4 = calcUpdownRatio(field_0x300);
                mAnm[SHIELD_ANIM_USE].setFrame(field_0x2F8 - field_0x2E4);
            }
        }
    } else if (!field_0x31D && mCurrentDurability < mAnimatingDurability - f3) {
        field_0x31F = 1;
        field_0x300 = mAnimatingDurability;
        field_0x304 = mCurrentDurability;
        field_0x2FC = dLytMeter_HIO_c::GetInstance()->getField_0x1C8();
        field_0x2E4 = calcUpdownRatio(mAnimatingDurability);
        mAnm[SHIELD_ANIM_USE].setFrame(field_0x2F8 - field_0x2E4);
        mAnm[SHIELD_ANIM_USE].setAnimEnable(true);
    } else if (!bIsAnimatingDurability) {
        field_0x2E4 = calcUpdownRatio(mAnimatingDurability);
        mAnm[SHIELD_ANIM_USE].setFrame(field_0x2F8 - field_0x2E4);
        mAnm[SHIELD_ANIM_USE].setAnimEnable(true);
    }

    if (!bIsAnimatingDurability) {
        mAnimatingDurability = mCurrentDurability;
    }
    field_0x31D = 0;
    if (mMaxDurability != field_0x2F4 || b3) {
        setLevel(mMaxDurability);
        mAnm[SHIELD_ANIM_LEVEL].setAnimEnable(true);
        field_0x2F4 = mMaxDurability;
    }

    if (mAnm[SHIELD_ANIM_BREAK].isEnabled()) {
        if (field_0x318 == 0 && mCurrentDurability > 0.0f) {
            mAnm[SHIELD_ANIM_BREAK].setFrame(0.0f);
            mAnm[SHIELD_ANIM_BREAK].setAnimEnable(true); // redundant?
            field_0x30C = 0;
            field_0x30D = 0;
        } else if (mAnm[SHIELD_ANIM_BREAK].isEndReached()) {
            mAnm[SHIELD_ANIM_BREAK].setAnimEnable(false);
            field_0x30C = 1;
            field_0x30D = 0;
        } else {
            mAnm[SHIELD_ANIM_BREAK].play();
        }
    } else if (field_0x30C && !field_0x318 && mCurrentDurability > 0.0f) {
        mAnm[SHIELD_ANIM_BREAK].setFrame(0.0f);
        mAnm[SHIELD_ANIM_BREAK].setAnimEnable(true);
        field_0x30C = 0;
        field_0x30D = 0;
    }

    if (mAnm[SHIELD_ANIM_TO_MAX].isEnabled()) {
        if (mAnm[SHIELD_ANIM_TO_MAX].isEndReached()) {
            mAnm[SHIELD_ANIM_TO_MAX].setAnimEnable(false);
            field_0x30E = 0;
        } else {
            mAnm[SHIELD_ANIM_TO_MAX].play();
        }
    }

    mStateMgr.executeState();

    if ((mSavedShieldType == SHIELD_SACRED_SHIELD || mSavedShieldType == SHIELD_DIVINE_SHIELD ||
         mSavedShieldType == SHIELD_GODDESS_SHIELD) &&
        mAnm[SHIELD_ANIM_USE].getFrame() == 0.0f && mAnm[SHIELD_ANIM_LOOP_0].getFrame() == 0.0f) {
        if (mAnm[SHIELD_ANIM_LOOP_0].isEnabled()) {
            mAnm[SHIELD_ANIM_LOOP_0].setAnimEnable(false);
        }
    } else if (!mAnm[SHIELD_ANIM_LOOP_0].isEnabled()) {
        mAnm[SHIELD_ANIM_LOOP_0].setAnimEnable(true);
    }

    if (mAnm[SHIELD_ANIM_LOOP_0].isEnabled()) {
        mAnm[SHIELD_ANIM_LOOP_0].play();
    }

    if (mAnm[SHIELD_ANIM_LOOP_1].isEnabled()) {
        mAnm[SHIELD_ANIM_LOOP_1].play();
    }

    mLyt.calc();

    if (mAnm[SHIELD_ANIM_UPDOWN].isEnabled()) {
        mAnm[SHIELD_ANIM_UPDOWN].setAnimEnable(false);
    }

    if (mAnm[SHIELD_ANIM_LEVEL].isEnabled()) {
        mAnm[SHIELD_ANIM_LEVEL].setAnimEnable(false);
    }

    if (mAnm[SHIELD_ANIM_TYPE].isEnabled()) {
        mAnm[SHIELD_ANIM_TYPE].setAnimEnable(false);
    }

    if (mAnm[SHIELD_ANIM_USE].isEnabled()) {
        mAnm[SHIELD_ANIM_USE].setAnimEnable(false);
    }

    if (!field_0x30E && mAnm[SHIELD_ANIM_TO_MAX].isEnabled()) {
        mAnm[SHIELD_ANIM_TO_MAX].setAnimEnable(false);
    }

    if (!field_0x30C && !field_0x30D && mAnm[SHIELD_ANIM_BREAK].isEnabled()) {
        mAnm[SHIELD_ANIM_BREAK].setAnimEnable(false);
    }

    if (field_0x318 > 0) {
        field_0x318--;
    }

    return true;
}

void dLytMeterShieldGauge_c::setLevel(f32 lv) {
    mAnm[SHIELD_ANIM_LEVEL].setFrame(lv * 0.5f);
}

s32 dLytMeterShieldGauge_c::getLytFrameForShield(s32 shield) const {
    static const int table[] = {0, 4, 7, 1, 5, 8, 2, 6, 9, 3};
    if (shield == SHIELD_NONE) {
        shield = 0;
    }
    return table[shield];
}

f32 dLytMeterShieldGauge_c::calcUpdownRatio(f32 f) const {
    f32 duration = mAnm[SHIELD_ANIM_UPDOWN].getAnimDuration() - 1.0f;
    f32 bound = 0.0f;
    f32 b = (mMaxDurability > bound ? f / mMaxDurability : bound);
    return b * duration;
}
