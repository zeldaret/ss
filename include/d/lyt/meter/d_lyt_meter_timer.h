#ifndef D_LYT_METER_TIMER_H
#define D_LYT_METER_TIMER_H

#include "d/a/d_a_item.h"
#include "d/lyt/d2d.h"
#include "m/m_color.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class LytMeterTimerPart1_c : public d2d::dSubPane {
public:
    LytMeterTimerPart1_c() {
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

    void createEffect(s32 fruitIndex);

private:
    void initLoopAnim();
    void initBowlNuts();
    void resetBowlNuts();
    void initOutAnim();
    void initColors();
    void initInAnim();

    static LytMeterTimerPart1_c *sInstance;

    /* 0x008 */ d2d::dLytSub mLyt;
    /* 0x09C */ d2d::AnmGroup_c mAnm[26];
    /* 0x71C */ s32 mActualTearCount;
    /* 0x720 */ u8 _0x720[0x724 - 0x720];
    /* 0x724 */ nw4r::lyt::Pane *mpPanes[15];
    /* 0x760 */ s32 field_0x760;
    /* 0x764 */ dAcItem_c::Trial_e mTrial;
    /* 0x768 */ mColor mColors1[3];
    /* 0x774 */ mColor mColors2[3];
    /* 0x780 */ s32 field_0x780;
};

class LytMeterTimerPart2_c : public d2d::dSubPane {
public:
    LytMeterTimerPart2_c() {
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

    void init();
    void initDyingAnims();
    void initFallOutAnims();
    void realizePetalsOnOff();
    void syncPetalsTime();
    void updatePetalsRate();
    void restartPetals();
    void resetSingleAnim(u8 idx);

    void startInAnim();
    void startOutAnim();

    void enableChangeFruitAnim();

    void enableBloomAnim();
    void enableBeforeFallAnim();
    void enableChangeSirenAnim();
    void enableSirenLoopAnim();
    void enableSafeAnim();
    void resumeFlowerLoop();
    void startFlowerLoop();

private:
    void initBloomAnim();
    void initBeforeFallAnim();
    void initChangeSirenAnim();
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
    dLytMeterTimer_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeterTimer_c() {}
    bool build();
    bool remove();

private:
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

    /* 0x04 */ UI_STATE_MGR_DECLARE(dLytMeterTimer_c);
    /* 0x40 */ LytMeterTimerPart1_c *mpPart1;
    /* 0x44 */ LytMeterTimerPart2_c *mpPart2;
    /* 0x48 */ u8 _0x48[0x60 - 0x48];
    /* 0x60 */ s32 field_0x60;
};

#endif
