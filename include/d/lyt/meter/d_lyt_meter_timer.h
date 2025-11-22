#ifndef D_LYT_METER_TIMER_H
#define D_LYT_METER_TIMER_H

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/lyt/d2d.h"
#include "m/m_color.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class LytMeterTimerPart1_c : public d2d::dSubPane {
public:
    LytMeterTimerPart1_c()
        : mActualTearCount(0), mDisplayedTearCount(0), field_0x760(0), mTrial(dAcItem_c::TRIAL_NONE), field_0x780(0) {
        sInstance = this;
    }
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    virtual const char *getName() const override {
        return mLyt.getName();
    }

    virtual ~LytMeterTimerPart1_c() {
        sInstance = nullptr;
    }

    static LytMeterTimerPart1_c *GetInstance() {
        return sInstance;
    }

    void init();
    void startInAnim();
    void realizeTrial();

    void startOutAnim();
    void stopInAnim();
    void stopFinishedFruit();
    void stopOutAnim();
    bool isInAnimFinished();
    bool isCurrentFruitAnimFinished();
    bool isAnyFruitAnimFinished();
    bool isLastFruitAnimFinished();
    bool isOutAnimFinished();
    void startFruitAnim(s32 index);
    void resetBowlNuts();
    void updateDropLine(nw4r::lyt::Pane *pane);

    void startEffect(s32 fruitIndex);
    bool incrementTearCount();
    bool isAnyFruitAnimAtFrame(f32 f) const;

    s32 getActualTearCount() const {
        return mActualTearCount;
    }

    s32 getDisplayedTearCount() const {
        return mDisplayedTearCount;
    }

    s32 getField0x780() const {
        return field_0x780;
    }

    void setField0x780(s32 val) {
        field_0x780 = val;
    }

private:
    void initLoopAnim();
    void initBowlNuts();
    void initOutAnim();
    void initColors();
    void initInAnim();
    void startNextFruitAnim();
    void disableCurrentFruitAnim();

    static LytMeterTimerPart1_c *sInstance;

    /* 0x008 */ d2d::dLytSub mLyt;
    /* 0x09C */ d2d::AnmGroup_c mAnm[26];
    /* 0x71C */ s32 mActualTearCount;
    /* 0x720 */ s32 mDisplayedTearCount;
    /* 0x724 */ nw4r::lyt::Pane *mpPanes[15];
    /* 0x760 */ s32 field_0x760;
    /* 0x764 */ dAcItem_c::Trial_e mTrial;
    /* 0x768 */ mColor mColors1[3];
    /* 0x774 */ mColor mColors2[3];
    /* 0x780 */ s32 field_0x780;
};

class LytMeterTimerPart2_c : public d2d::dSubPane {
public:
    LytMeterTimerPart2_c(): mNumPetals(0), mTrial(dAcItem_c::TRIAL_NONE), mVisible(true), mFlowerLoopFrame(0.0f) {
        sInstance = this;
    }
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    virtual const char *getName() const override {
        return mLyt.getName();
    }

    virtual ~LytMeterTimerPart2_c() {
        sInstance = nullptr;
    }

    static LytMeterTimerPart2_c *GetInstance() {
        return sInstance;
    }

    void setNumPetals(s32 num) {
        mNumPetals = num;
    }

    s32 getNumPetals() const {
        return mNumPetals;
    }

    nw4r::lyt::Pane *i_getPane() const {
        return mpPane;
    }

    void setVisible(bool bVisible) {
        mVisible = bVisible;
    }

    void calc();

    void init();
    void initDyingAnims();
    void initFallOutAnims();
    void realizePetalsOnOff();
    void syncPetalsTime();
    void updatePetalsRate();
    void restartPetals();
    void finishSingleAnim(u8 idx);
    void resetSingleAnim(u8 idx);

    void startInAnim();
    void startOutAnim();
    void stopInAnim();
    void stopBloomAnim();
    void stopBeforeFallAnim();
    void stopChangeSirenAnim();
    void stopSirenLoopAnim();
    void stopSirenSafeAnim();
    void stopFlowerLoopAnim();

    void resetFlowerLoopAnim();
    void stopDyingAnims();
    void stopFinishedDyingAnims();
    void stopFallOutAnims();
    void stopFinishedFallOutAnims();
    void stopOutAnim();
    void stopChangeFruitAnim();
    bool isInAnimFinished();
    bool isBloomAnimFinished();
    bool isBeforeFallAnimAtStart();
    bool isChangeSirenAnimFinished();
    bool isSirenSafeAnimFinished();
    bool isFlowerLoopAnimFinished();
    bool isOutAnimFinished();
    bool isChangeFruitAnimFinished();
    void realizeFruitsColor();
    void finishDyingAnims();
    void setPetalsToStart();

    void enableChangeFruitAnim();

    void enableBloomAnim();
    void enableBeforeFallAnim();
    void enableChangeSirenAnim();
    void enableSirenLoopAnim();
    void enableSafeAnim();
    void resumeFlowerLoop();
    void startFlowerLoop();
    void syncTime();
    void syncTimeWithSafe();
    /** Played when the finish animation is played and each tear hits the flower */
    void createSingleFruitEffect();
    /** Played when the whole fruit is finished */
    void createFruitCompleteEffect();

    void initBeforeFallAnim();

    bool isPetalDyingAnimFinished();
    bool isFirstFallOutAnimFinished();
    void initChangeSirenAnim();

private:
    void initBloomAnim();
    void initSirenLoopAnim();
    void initSirenSafeAnim();
    void initFlowerLoopAnim();
    void initOutAnim();
    void initInAnim();
    void initChangeFruitAnim();
    void initColors();

    static LytMeterTimerPart2_c *sInstance;

    /* 0x008 */ d2d::dLytSub mLyt;
    /* 0x09C */ d2d::AnmGroup_c mAnm[30];
    /* 0x81C */ s32 mNumPetals;
    /* 0x820 */ nw4r::lyt::Pane *mpPane;
    /* 0x824 */ dAcItem_c::Trial_e mTrial;
    /* 0x828 */ mColor mColors1[2];
    /* 0x830 */ mColor mColors2[2];
    /* 0x838 */ u8 mVisible;
    /* 0x83C */ f32 mFlowerLoopFrame;
};

class dLytMeterTimer_c {
public:
    dLytMeterTimer_c()
        : mStateMgr(*this, sStateID::null),
          mpPart1(nullptr),
          mpPart2(nullptr),
          field_0x48(0),
          field_0x49(0),
          mActualTime(0),
          mLastTime(0),
          field_0x54(0),
          field_0x55(0),
          field_0x56(8),
          field_0x57(8),
          field_0x58(0),
          field_0x59(0),
          field_0x5C(0),
          field_0x60(0) {}
    virtual ~dLytMeterTimer_c() {}
    bool build();
    bool remove();
    bool execute();
    bool startIn2();
    bool startOut2();

    bool getField_0x54() const {
        return field_0x54;
    }

private:
    void startIn();
    void gotoChangeSiren();
    void gotoSiren();
    void gotoChangeSafeBloom();
    void gotoChangeSafe();
    void gotoSafe();
    void gotoChangeFruits6();
    void gotoFruits();
    void startOut();
    void gotoChangeFruits3();
    void startSafe();
    void doPickup();
    bool isInSiren();
    s32 getPetalForTimerMaybe(s32 time);
    bool checkForPetalChangeMaybe();

    STATE_FUNC_DECLARE(dLytMeterTimer_c, ChangeSiren);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, Siren);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, ChangeSafeBloom);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, ChangeSafe);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, Safe);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, ChangeFruits);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, Fruits);

    static bool sDoExit;
    static bool sDoFinishAnim;
    static bool sFinished;

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterTimer_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dLytMeterTimer_c);

    /* 0x04 */ UI_STATE_MGR_DECLARE(dLytMeterTimer_c);
    /* 0x40 */ LytMeterTimerPart1_c *mpPart1;
    /* 0x44 */ LytMeterTimerPart2_c *mpPart2;
    /* 0x48 */ u8 field_0x48;
    /* 0x49 */ u8 field_0x49;
    /* 0x4C */ s32 mActualTime;
    /* 0x50 */ s32 mLastTime;
    /* 0x54 */ bool field_0x54;
    /* 0x55 */ u8 field_0x55;
    /* 0x56 */ u8 field_0x56;
    /* 0x57 */ u8 field_0x57;
    /* 0x58 */ u8 field_0x58;
    /* 0x59 */ u8 field_0x59;
    /* 0x5C */ s32 field_0x5C;
    /* 0x60 */ s32 field_0x60;
};

#endif
