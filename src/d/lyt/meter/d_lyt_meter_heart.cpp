#include "d/lyt/meter/d_lyt_meter_heart.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_sc_game.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_game_over.h"
#include "nw4r/math/math_types.h"
#include "toBeSorted/effects_struct.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/small_sound_mgr.h"

static const d2d::LytBrlanMapping brlanMap[] = {
    {     "heartAll_00_heat.brlan",     "G_heart_00"},
    {     "heartAll_00_heat.brlan",     "G_heart_01"},
    {     "heartAll_00_heat.brlan",     "G_heart_02"},
    {     "heartAll_00_heat.brlan",     "G_heart_03"},
    {     "heartAll_00_heat.brlan",     "G_heart_04"},
    {     "heartAll_00_heat.brlan",     "G_heart_05"},
    {     "heartAll_00_heat.brlan",     "G_heart_06"},
    {     "heartAll_00_heat.brlan",     "G_heart_07"},
    {     "heartAll_00_heat.brlan",     "G_heart_08"},
    {     "heartAll_00_heat.brlan",     "G_heart_09"},
    {     "heartAll_00_heat.brlan",     "G_heart_10"},
    {     "heartAll_00_heat.brlan",     "G_heart_11"},
    {     "heartAll_00_heat.brlan",     "G_heart_12"},
    {     "heartAll_00_heat.brlan",     "G_heart_13"},
    {     "heartAll_00_heat.brlan",     "G_heart_14"},
    {     "heartAll_00_heat.brlan",     "G_heart_15"},
    {     "heartAll_00_heat.brlan",     "G_heart_16"},
    {     "heartAll_00_heat.brlan",     "G_heart_17"},
    {     "heartAll_00_heat.brlan",     "G_heart_18"},
    {     "heartAll_00_heat.brlan",     "G_heart_19"},
    {"heartAll_00_heartMain.brlan", "G_heartMain_00"},
    {    "heartAll_00_drink.brlan",     "G_drink_00"},
    {    "heartAll_00_mLoop.brlan",     "G_mLoop_00"},
    { "heartAll_00_mCaution.brlan",  "G_mCaution_00"},
    {     "heartAll_00_loop.brlan",      "G_loop_00"},
    {   "heartAll_00_danger.brlan",    "G_danger_00"},
};

#define HEART_ANIM_HEAT_OFFSET 0
#define HEART_ANIM_MAIN 20
#define HEART_ANIM_DRINK 21
#define HEART_ANIM_MLOOP 22
#define HEART_ANIM_MCAUTION 23
#define HEART_ANIM_LOOP 24
#define HEART_ANIM_DANGER 25

#define HEART_NUM_ANIMS 26

#define HEART_NUM_HEARTS 20

static const char *sPaneNames[] = {
    "N_heart_00", "N_heart_01", "N_heart_02", "N_heart_03", "N_heart_04", "N_heart_05", "N_heart_06",
    "N_heart_07", "N_heart_08", "N_heart_09", "N_heart_10", "N_heart_11", "N_heart_12", "N_heart_13",
    "N_heart_14", "N_heart_15", "N_heart_16", "N_heart_17", "N_heart_18", "N_heart_19",

};

bool dLytMeterHeart_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("heartAll_00.brlyt", nullptr);

    for (int i = 0; i < HEART_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        if (i <= HEART_NUM_HEARTS) {
            mAnm[i].setAnimEnable(true);
        } else {
            mAnm[i].setAnimEnable(false);
        }
    }

    for (int i = 0; i < HEART_NUM_HEARTS; i++) {
        mpHeartPanes[i] = mLyt.findPane(sPaneNames[i]);
    }

    mpAlphaPane = mLyt.findPane("N_heartAlpha_00");
    mpMainPane = mLyt.findPane("N_heartMain_00");

    mCurrentHealth = 0;
    mCurrentHealthCapacity = 0;
    mStoredHealth = 0;
    mStoredHealthCapacity = 0;
    mHealthCapacityIncreaseDelayTimer = 0;
    mEffectHeartIdx = -1;
    field_0x78C = 0;
    field_0x790 = 0;
    field_0x794 = 0;
    field_0x797 = 0;
    mIsLoop = false;
    mIsDanger = false;
    field_0x79A = 0;

    mCurrentHealth = getCurrentHealth();
    mCurrentHealthCapacity = getCurrentHealthCapacity();
    mStoredHealth = mCurrentHealth;
    mStoredHealthCapacity = mCurrentHealthCapacity;
    field_0x794 = 1;
    realizeHeartsState();
    field_0x795 = 1;

    mLyt.calc();

    return true;
}

bool dLytMeterHeart_c::remove() {
    return true;
}

bool dLytMeterHeart_c::execute() {
    if (field_0x78C > 0) {
        field_0x78C--;
    }
    if (field_0x790 > 0) {
        field_0x790--;
    }
    executeInternal();

    if (mIsLoop) {
        if (!mAnm[HEART_ANIM_LOOP].isEnabled()) {
            mAnm[HEART_ANIM_LOOP].setFrame(0.0f);
            mAnm[HEART_ANIM_LOOP].setAnimEnable(true);
        }
    } else {
        if (mAnm[HEART_ANIM_LOOP].isEnabled()) {
            mAnm[HEART_ANIM_LOOP].setFrame(0.0f);
            mLyt.calc();
            mAnm[HEART_ANIM_LOOP].setAnimEnable(false);
        }
    }

    if (mIsDanger) {
        if (dAcPy_c::GetLink2()->getField_0x4564() == 1.0f) {
            if (!mAnm[HEART_ANIM_DANGER].isEnabled()) {
                mAnm[HEART_ANIM_DANGER].setAnimEnable(true);
            }
            mAnm[HEART_ANIM_DANGER].setFrame(0.0f);
        }
    } else {
        if (mAnm[HEART_ANIM_DANGER].isEnabled()) {
            mAnm[HEART_ANIM_DANGER].setToEnd();
            mLyt.getLayout()->Animate(0);
            mLyt.calc();
            mAnm[HEART_ANIM_DANGER].setAnimEnable(false);
        }
    }

    f32 drinkFrame = 0.0f;
    bool hasGuardianPotion = false;
    bool hasExpiringGuardianPotion = false;
    if (FileManager::GetInstance()->hasGuardianPotionPlus()) {
        drinkFrame = 2.0f;
        hasGuardianPotion = true;
    } else if (FileManager::GetInstance()->hasGuardianPotionNormal()) {
        hasGuardianPotion = true;
        drinkFrame = 1.0f;
    }

    if ((FileManager::GetInstance()->hasGuardianPotionNormal() &&
         FileManager::GetInstance()->getGuardianPotionTimer() < 600) ||
        (FileManager::GetInstance()->hasGuardianPotionPlus() &&
         FileManager::GetInstance()->getGuardianPotionPlusTimer() < 600)) {
        hasExpiringGuardianPotion = true;
    }

    if (hasGuardianPotion) {
        if (!mAnm[HEART_ANIM_MLOOP].isEnabled()) {
            mAnm[HEART_ANIM_MLOOP].setFrame(0.0f);
            mAnm[HEART_ANIM_MLOOP].setAnimEnable(true);
        }
    } else if (mAnm[HEART_ANIM_MLOOP].getFrame() < 1.0f) {
        mAnm[HEART_ANIM_MLOOP].setFrame(0.0f);
        mLyt.getLayout()->Animate(0);
        mAnm[HEART_ANIM_MLOOP].setAnimEnable(false);
    }

    if (hasGuardianPotion && hasExpiringGuardianPotion) {
        if (!mAnm[HEART_ANIM_MCAUTION].isEnabled()) {
            mAnm[HEART_ANIM_MCAUTION].setFrame(0.0f);
            mAnm[HEART_ANIM_MCAUTION].setAnimEnable(true);
        }
    } else if (mAnm[HEART_ANIM_MCAUTION].getFrame() < 1.0f) {
        mAnm[HEART_ANIM_MCAUTION].setFrame(0.0f);
        mLyt.getLayout()->Animate(0);
        mAnm[HEART_ANIM_MCAUTION].setAnimEnable(false);
    }

    mAnm[HEART_ANIM_DRINK].setFrame(drinkFrame);
    mAnm[HEART_ANIM_DRINK].setAnimEnable(true);

    for (int i = 0; i < HEART_NUM_ANIMS; i++) {
        if (i > HEART_NUM_HEARTS + 1 && mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    if (mIsDanger && mAnm[HEART_ANIM_DANGER].isEndReached()) {
        mAnm[HEART_ANIM_DANGER].setToEnd();
        mAnm[HEART_ANIM_DANGER].setAnimEnable(false);
    }

    mLyt.calc();

    return true;
}

s32 dLytMeterHeart_c::getNumDisplayedHearts() const {
    return mStoredHealthCapacity / 4;
}

s32 dLytMeterHeart_c::getDisplayedHealthCapacity() const {
    return getNumDisplayedHearts() * 4;
}

void dLytMeterHeart_c::realizeHeartsState() {
    s32 hearts = getNumDisplayedHearts();
    (void)getDisplayedHealthCapacity();
    s32 health = mStoredHealth;
    s32 healthCapacity = mStoredHealthCapacity;
    s32 numFilledHearts = health / 4;
    s32 partialHeartFill = health % 4; // Wrong
    s32 displayedFilledHearts = healthCapacity / 4;
    s32 mNumTotalHeartsIncludingPartial = (getCurrentHealthCapacity() + 3) / 4;
    if (displayedFilledHearts < mNumTotalHeartsIncludingPartial) {
        mNumTotalHeartsIncludingPartial = displayedFilledHearts;
    }

    mIsLoop = false;
    mIsDanger = false;

    for (int i = 0; i < HEART_NUM_HEARTS; i++) {
        if (i < mNumTotalHeartsIncludingPartial) {
            mpHeartPanes[i]->SetVisible(true);
            if (i < displayedFilledHearts) {
                if (i < hearts) {
                    if (numFilledHearts == 0 && partialHeartFill == 0) {
                        if (i == 0) {
                            mpMainPane->SetVisible(false);
                        }
                        mAnm[i + HEART_ANIM_HEAT_OFFSET].setFrame(1.0f);
                    } else if (i < numFilledHearts) {
                        mAnm[i + HEART_ANIM_HEAT_OFFSET].setFrame(0.0f);
                        if (i == numFilledHearts - 1 && partialHeartFill == 0) {
                            mAnm[i + HEART_ANIM_HEAT_OFFSET].setFrame(2.0f);
                            mpMainPane->SetVisible(true);
                            mpMainPane->SetTranslate(mpHeartPanes[i]->GetTranslate());
                            mAnm[HEART_ANIM_MAIN].setFrame(4.0f);
                            if (dAcPy_c::GetLink2() != nullptr &&
                                dAcPy_c::GetLink2()->hasLessThanQuarterHealth(false)) {
                                mIsDanger = true;
                            } else {
                                mIsLoop = true;
                            }
                        } else {
                            mAnm[i + HEART_ANIM_HEAT_OFFSET].setFrame(0.0f);
                        }
                    } else if (i == numFilledHearts && partialHeartFill != 0) {
                        mAnm[i + HEART_ANIM_HEAT_OFFSET].setFrame(2.0f);
                        mpMainPane->SetVisible(true);
                        mpMainPane->SetTranslate(mpHeartPanes[i]->GetTranslate());
                        mAnm[HEART_ANIM_MAIN].setFrame(partialHeartFill);
                        if (dAcPy_c::GetLink2() != nullptr && dAcPy_c::GetLink2()->hasLessThanQuarterHealth(false)) {
                            mIsDanger = true;
                        } else {
                            mIsLoop = true;
                        }
                    } else {
                        mAnm[i + HEART_ANIM_HEAT_OFFSET].setFrame(1.0f);
                    }
                } else {
                    mAnm[i + HEART_ANIM_HEAT_OFFSET].setFrame(1.0f);
                }
            } else {
                mAnm[i + HEART_ANIM_HEAT_OFFSET].setFrame(1.0f);
            }
        } else {
            mpHeartPanes[i]->SetVisible(false);
        }
    }
}
extern "C" const u16 PARTICLE_RESOURCE_ID_MAPPING_972_;
void dLytMeterHeart_c::executeInternal() {
    if (mEffectHeartIdx >= 0) {
        nw4r::math::MTX34 mtx = mpHeartPanes[mEffectHeartIdx]->GetGlobalMtx();
        mVec3_c pos(mtx._03, mtx._13, 0.0f);
        EffectsStruct::fn_800298C0(PARTICLE_RESOURCE_ID_MAPPING_972_, &pos, nullptr, nullptr, nullptr, nullptr);
        SmallSoundManager::GetInstance()->playSound(SE_S_HEART_ADD);
        mEffectHeartIdx = -1;
    }

    // Redundant
    if (!field_0x794) {
        mCurrentHealth = getCurrentHealth();
        mCurrentHealthCapacity = getCurrentHealthCapacity();
        field_0x794 = 1;
    }

    mCurrentHealthCapacity = getCurrentHealthCapacity();
    mCurrentHealth = getCurrentHealth();

    if (field_0x796) {
        for (int i = 0; i < HEART_NUM_HEARTS; i++) {
            mAnm[i + HEART_ANIM_HEAT_OFFSET].setFrame(1.0f);
        }
        mLyt.calc();
        field_0x796 = 0;
    }

    bool isDanger = dAcPy_c::GetLink2()->hasLessThanQuarterHealth(0);
    bool healthDifferent = mStoredHealth != mCurrentHealth;
    bool healthCapacityDifferent = mStoredHealthCapacity != mCurrentHealthCapacity;
    if (healthCapacityDifferent && field_0x790 < 20) {
        field_0x790 = 20;
    }
    if (healthDifferent || field_0x797 != isDanger) {
        s32 current = mCurrentHealth;
        if (mStoredHealth + 1 <= current) {
            mStoredHealth = mStoredHealth + 1;
            if (mpAlphaPane->IsVisible() && mpAlphaPane->GetGlobalAlpha() != 0 && mStoredHealth % 4 == 0 &&
                (!(dScGame_c::currentSpawnInfo.stageName == "F406") || dScGame_c::currentSpawnInfo.layer != 13) &&
                dLytGameOver_c::GetInstance() == nullptr) {
                SmallSoundManager::GetInstance()->playSound(SE_S_HP_GAUGE_UP);
            }
        } else {
            if (mStoredHealth - 1 >= current) {
                mStoredHealth = mStoredHealth - 1;
                if (EventManager::isInEvent() && !strcmp(EventManager::getCurrentEventName(), ("MoleF202Start")) &&
                    mStoredHealth % 4 == 0) {
                    SmallSoundManager::GetInstance()->playSound(SE_S_HP_GAUGE_DOWN);
                }
            }
        }
        realizeHeartsState();
        if (healthDifferent && field_0x78C < 10) {
            field_0x78C = 10;
        }
        field_0x797 = isDanger;
    } else if (healthCapacityDifferent) {
        if (mStoredHealthCapacity + 1 <= mCurrentHealthCapacity) {
            if ((mStoredHealthCapacity + 1) % 4 == 0) {
                if (mHealthCapacityIncreaseDelayTimer < 15) {
                    mHealthCapacityIncreaseDelayTimer += 1;
                    return;
                }
                mHealthCapacityIncreaseDelayTimer = 0;
            }
            mStoredHealthCapacity++;
            if (mStoredHealthCapacity % 4 == 0) {
                field_0x79A = 1;
            }
        } else {
            if (mStoredHealthCapacity - 1 >= mCurrentHealthCapacity) {
                mStoredHealthCapacity = mStoredHealthCapacity - 1;
            }
        }
        if (field_0x79A && mStoredHealth > mCurrentHealthCapacity - 3) {
            s32 numDisplayed = getNumDisplayedHearts();
            if (numDisplayed >= 1 && numDisplayed <= 20) {
                mEffectHeartIdx = numDisplayed - 1;
                field_0x79A = 0;
            }
        }
        realizeHeartsState();
        if (field_0x78C < 20) {
            field_0x78C = 20;
        }
    } else if (field_0x79A) {
        field_0x79A = 0;
    }
}

u8 dLytMeterHeart_c::getCurrentHealth() const {
    return FileManager::GetInstance()->getCurrentHealth();
}

u8 dLytMeterHeart_c::getCurrentHealthCapacity() const {
    return dAcPy_c::getCurrentHealthCapacity();
}
