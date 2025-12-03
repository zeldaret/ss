#ifndef D_LYT_MINI_GAME_H
#define D_LYT_MINI_GAME_H

#include "common.h"
#include "d/d_base.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"

/** 2D UI - Mini Game - Pumpkin score popup */
class dLytMiniGamePumpkinParts_c {
public:
    dLytMiniGamePumpkinParts_c() : field_0x154(false), mHasPlayedDoubleScoreSound(false) {}
    virtual ~dLytMiniGamePumpkinParts_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();
    void init();

    void setPumpkinPos(const mVec3_c &pos);
    void getPoint(u8 combo, bool doubleScore);
    bool isActive() const;

private:
    void resetPattern();
    void setPattern(s32 pat);
    void startGetPoint();
    void startDoubleScore();
    void stopDoubleScore();
    void resetGetPoint();
    void resetDoubleScore();

    /* 0x004 */ d2d::LytBase_c mLyt;
    /* 0x094 */ d2d::AnmGroup_c mAnm[3];
    /* 0x154 */ bool field_0x154;
    /* 0x158 */ mVec3_c mPumpkinWorldPos;
    /* 0x164 */ mVec3_c mPumpkinScreenPos;
    /* 0x170 */ mVec2_c field_0x170;
    /* 0x178 */ bool mHasPlayedDoubleScoreSound;
};

/** 2D UI - Mini Game - Pumpkin score popups */
class dLytMiniGamePumpkin_c {
public:
    dLytMiniGamePumpkin_c() : field_0x478(10), mCombo(0) {}
    virtual ~dLytMiniGamePumpkin_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();
    void init();

    void getPoint(const mVec3_c &pumpkinPos, s32 unk, bool doubleScore);
    void calcCombo(s32 unk);

private:
    static dLytMiniGamePumpkin_c *sInstance;

    /* 0x004 */ dLytMiniGamePumpkinParts_c mParts[3];
    /* 0x478 */ s32 field_0x478;
    /* 0x47C */ s32 mCombo;
};

/** 2D UI - Mini Game - Bug Heaven */
class dLytMiniGameBugs_c {
public:
    dLytMiniGameBugs_c()
        : mBugIconsInEnded(false),
          mInTriggered(false),
          mBugIconsFlashStarted(false),
          mBugsNumLevel(1),
          mBugsNum(10),
          field_0xB4B(false),
          field_0xB4C(true) {}
    virtual ~dLytMiniGameBugs_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();
    void init();

    enum BugIndex_e {
        DEKU_HORNET,
        BLESSED_BUTTERFLY,
        GERUDO_DRAGONFLY,
        STARRY_FIREFLY,
        WOODLAND_RHINO_BEETLE,
        VOLCANIC_LADYBUG,
        SAND_CICADA,
        SKY_STAG_BEETLE,
        FARON_GRASSHOPPER,
        SKYLOFT_MANTIS,
        LANAYRU_ANT,
        ELDIN_ROLLER,
    };

private:
    void startAllIconsIn();
    void startOut();
    void startIn();
    void startOutForced();
    void collectBug(s32 slot);
    void startBugsFlash();
    void setBug(s32 slot, s32 bugIndex);
    bool isSlotIconOnEndReached(s32 slot) const;
    bool isSlotIconOnEnabled(s32 slot) const;
    bool isSlotBugCollected(s32 slot);
    void resetBugIconsIn();
    void resetIn();
    void resetOut();
    void resetBugIconsOn();
    void resetBugIconsTex();
    void resetBugIconsFlash();
    void stopBugIconsIn();
    void stopIn();
    void stopOut();
    void stopBugIconOn(s32);
    void stopBugIconsFlash();
    bool isIconInEndReached() const;
    bool isInEndReached() const;
    bool isOutEndReached() const;
    bool isSlotOnEndReached(s32 slot) const;
    bool isBug0Flashing() const;
    bool areAllBugIconsOn() const;

    static dLytMiniGameBugs_c *sInstance;

    /* 0x004 */ d2d::LytBase_c mLyt;
    /* 0x094 */ d2d::AnmGroup_c mAnm[42];
    /* 0xB14 */ s32 mBugItemIds[10];
    /* 0xB3C */ bool mBugIconsInEnded;
    /* 0xB3D */ bool mInTriggered;
    /* 0xB3E */ bool mBugCollected[10];
    /* 0xB48 */ bool mBugIconsFlashStarted;
    /* 0xB49 */ u8 mBugsNumLevel; ///< 0 = 5 bugs, 1 = 10 bugs
    /* 0xB4A */ u8 mBugsNum;
    /* 0xB4B */ bool field_0xB4B;
    /* 0xB4C */ bool field_0xB4C;
};

/** 2D UI - Mini Game - Time */
class dLytMiniGameTime_c {
public:
    enum Variant_e {
        TIME_VARIANT_0,
        TIME_VARIANT_1,
        TIME_VARIANT_2,
        TIME_VARIANT_3,
        TIME_VARIANT_4,
        TIME_VARIANT_5,
    };

public:
    dLytMiniGameTime_c()
        : field_0x7D4(0),
          mCurrentTimerValueMilliSeconds(0),
          mCurrentTimerValueSeconds(0),
          mLastTimerValueSeconds(0),
          mTimeFormat(0),
          mHighestDigitIndex(0),
          field_0x7EC(false),
          field_0x7ED(false),
          field_0x7EE(false),
          mVariant(0),
          field_0x7F4(0),
          mIsVisible(false) {}
    virtual ~dLytMiniGameTime_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();
    void init();

    void setVariant(s32 var) {
        mVariant = var;
    }

    void setField_0x7EE(bool v) {
        field_0x7EE = v;
    }

private:
    enum TimeFormat_e {
        TIMER_LONG,
        TIMER_SECONDS,
    };

    void fn_8028DD80();
    void fn_8028DE40();
    void fn_8028DED0();
    void fn_8028DFD0();
    void startAlphaIn();
    void startAlphaOut();
    void startFinish();
    void startLoop();
    void setTimerValueInMilliSeconds(s32 time);
    void updateLongTimer();
    void setTimerSecondsValueInMilliSeconds(s32 time);
    void updateSecondsTimer();
    void saveSecondsTimer();
    void setBestTime(s32 time);
    void setShowBestTime(bool show);
    void resetIn();
    void resetOut();
    void resetAlphaIn();
    void resetAlphaOut();
    void resetDigits();
    void resetPosition();
    void resetShowBestTime();
    void resetFinish();
    void resetLoop();
    void initSignChange();
    void stopIn();
    void stopOut();
    void stopAlphaIn();
    void stopAlphaOut();
    void stopFinish();
    void stopLoop();
    bool isInEndReached() const;
    bool isOutEndReached() const;
    bool isAlphaInEndReached() const;
    bool isAlphaOutEndReached() const;
    bool isFinishEndReached() const;
    void setHighestDigitIndex(u8);
    void setSignChange(u32);
    void setTimePosition(bool);
    void setBestHasSixDigits(bool);
    void setDigit(s32 digitIndex, s32 number);
    void setDigitBest(s32 digitIndex, s32 number);

    static dLytMiniGameTime_c *sInstance;

    /* 0x004 */ d2d::LytBase_c mLyt;
    /* 0x094 */ d2d::AnmGroup_c mAnm[29];
    /* 0x7D4 */ u8 field_0x7D4;
    /* 0x7D8 */ s32 mCurrentTimerValueMilliSeconds;
    /* 0x7DC */ s32 mCurrentTimerValueSeconds;
    /* 0x7E0 */ s32 mLastTimerValueSeconds;
    /* 0x7E4 */ u8 mTimeFormat;
    /* 0x7E4 */ s32 mHighestDigitIndex;
    /* 0x7EC */ bool field_0x7EC;
    /* 0x7ED */ bool field_0x7ED;
    /* 0x7EE */ bool field_0x7EE;
    /* 0x7F0 */ s32 mVariant;
    /* 0x7F4 */ u8 field_0x7F4;
    /* 0x7F5 */ bool mIsVisible;
};

/** 2D UI - Mini Game - Start / Finish Popup */
class dLytMiniGameStart_c {
public:
    dLytMiniGameStart_c() : mIsVisible(false) {}
    virtual ~dLytMiniGameStart_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();
    void init();

private:
    void startCountdown();
    void startCountdown120();
    void startFinish();
    void startTimeup();
    bool isCountdownEndReached() const;
    bool isFinishEndReached() const;
    bool isTimeupEndReached() const;
    void resetPlayedSounds();
    void resetCountdown();
    void resetFinish();
    void resetStart();
    void stopCountdown();
    void stopFinish();
    void stopTimeup();
    bool checkCountdown3();
    bool checkCountdown2();
    bool checkCountdown1();
    bool checkStart();
    bool checkFinish();
    bool checkTimeup();

    static dLytMiniGameStart_c *sInstance;

    /* 0x004 */ d2d::LytBase_c mLyt;
    /* 0x094 */ d2d::AnmGroup_c mAnm[3];
    /* 0x154 */ bool mPlayedSounds[6];
    /* 0x15A */ bool mIsVisible;
};

/** 2D UI - Mini Game - Score counter */
class dLytMiniGameScore_c {
public:
    enum Variant_e {
        SCORE_VARIANT_0,
        SCORE_VARIANT_1,
        SCORE_VARIANT_2,
        SCORE_VARIANT_3,
        SCORE_VARIANT_4,
    };

public:
    dLytMiniGameScore_c()
        : mScore(0),
          mLastScore(0),
          mHighestDigitIndex(0),
          field_0x7E4(0),
          field_0x7E8(0.0f),
          field_0x7EC(-2),
          field_0x7F0(0),
          field_0x7F4(0),
          field_0x7F8(-1),
          field_0x7FC(1),
          field_0x800(0),
          field_0x804(false),
          field_0x808(0),
          field_0x80C(0),
          field_0x810(0),
          field_0x814(false),
          field_0x815(false),
          field_0x816(false),
          mVariant(SCORE_VARIANT_0),
          field_0x81C(0),
          field_0x820(false),
          field_0x824(0),
          field_0x828(false),
          field_0x829(false),
          field_0x82C(0),
          field_0x830(1),
          mIsVisible(false) {}
    virtual ~dLytMiniGameScore_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();
    void init();

    void setVariant(s32 var) {
        mVariant = var;
    }

    void setField_0x816(bool v) {
        field_0x816 = v;
    }

private:
    void fn_80291410();
    void fn_802915B0();
    void fn_80291640();
    void fn_80291740();
    void startAlphaIn();
    void startAlphaOut();
    void startGetScore();
    void startFinish();
    void startLoop();
    void setScore(s32 score);
    void fn_80291BC0();
    void fn_80291D40(s32 arg);
    bool hasIncreasedScore() const;
    bool hasDecreasedScore() const;
    void fn_80291ED0(s32);
    void fn_80291EF0();
    void increaseScore(s32);
    void fn_80292040();
    s32 calcNumDigits();
    void realizePosition();
    void setDigit(s32 digitIndex, s32 number);
    void saveScore();
    void resetIn();
    void resetDigits();
    void resetOut();
    void resetAlphaIn();
    void resetAlphaOut();
    void resetSetPosition();
    void resetRupeeChange();
    void resetGetScore();
    void resetFinish();
    void resetLoop();
    void stopIn();
    void stopOut();
    void stopAlphaIn();
    void stopAlphaOut();
    void stopGetScore();
    void stopFinish();
    void stopLoop();
    bool isInEndReached() const;
    bool isOutEndReached() const;
    bool isAlphaInEndReached() const;
    bool isAlphaOutEndReached() const;
    bool isGetScoreEndReached() const;
    bool isFinishEndReached() const;
    void fn_80292C30();
    void loadTextVariant(s32 variant);
    void fn_80293410();
    void fn_80293450(s32);

    static dLytMiniGameScore_c *sInstance;

    /* 0x004 */ d2d::LytBase_c mLyt;
    /* 0x094 */ d2d::AnmGroup_c mAnm[26];

    /* 0x714 */ nw4r::lyt::Pane *mpPanes[20];

    /* 0x764 */ f32 mFloats[26];

    /* 0x7CC */ dTextBox_c *mpTextBoxes[3];

    /* 0x7D8 */ s32 mScore;
    /* 0x7DC */ s32 mLastScore;
    /* 0x7E0 */ s32 mHighestDigitIndex;
    /* 0x7E4 */ UNKWORD field_0x7E4;

    /* 0x7E8 */ f32 field_0x7E8;
    /* 0x7EC */ s32 field_0x7EC;
    /* 0x7F0 */ s32 field_0x7F0;
    /* 0x7F4 */ s32 field_0x7F4;
    /* 0x7F8 */ s32 field_0x7F8;
    /* 0x7FC */ s32 field_0x7FC;
    /* 0x800 */ s32 field_0x800;
    /* 0x804 */ bool field_0x804;
    /* 0x808 */ s32 field_0x808;
    /* 0x80C */ s32 field_0x80C;
    /* 0x810 */ s32 field_0x810;
    /* 0x814 */ bool field_0x814;
    /* 0x815 */ bool field_0x815;
    /* 0x816 */ bool field_0x816;
    /* 0x818 */ s32 mVariant;
    /* 0x81C */ s32 field_0x81C;
    /* 0x820 */ bool field_0x820;
    /* 0x824 */ s32 field_0x824;
    /* 0x829 */ bool field_0x828;
    /* 0x829 */ bool field_0x829;
    /* 0x82C */ UNKWORD field_0x82C;
    /* 0x830 */ s32 field_0x830;
    /* 0x834 */ f32 field_0x834[3][2];
    /* 0x84C */ bool mIsVisible;
};

/** 2D UI - Mini Game - Fun Fun Island score */
class dLytMiniGameScoreSd_c {
public:
    dLytMiniGameScoreSd_c() : field_0x1D4(0), mIsVisible(false) {}
    virtual ~dLytMiniGameScoreSd_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();
    void init();

private:
    static dLytMiniGameScoreSd_c *sInstance;

    void setScoreUp(s32 score);
    void setScoreDown(s32 score);
    void setScoreInternal(s32 score);
    void setHighestDigitIndex(s32);
    void setDigit(s32 digitIndex, s32 number);
    void resetScoreUp();
    void resetScoreDown();
    void resetDigits();
    void resetPosition();
    void startScoreUp();
    void startScoreDown();
    void stopScoreUp();
    void stopScoreDown();
    bool isScoreUpEndReached() const;
    bool isScoreDownEndReached() const;

    /* 0x004 */ d2d::LytBase_c mLyt;
    /* 0x094 */ d2d::AnmGroup_c mAnm[5];
    /* 0x1D4 */ s32 field_0x1D4;
    /* 0x1D8 */ bool mIsVisible;
    /* 0x1D9 */ bool field_0x1D9;
};

class dLytMiniGame_c : public dBase_c {
public:
    enum Variant_e {
        VARIANT_0,
        VARIANT_1,
        VARIANT_2,
        VARIANT_3,
        VARIANT_4,
        VARIANT_5,
        VARIANT_6,
        VARIANT_7,
        VARIANT_8,
        VARIANT_9,
        VARIANT_10,
        VARIANT_11,
        VARIANT_12,
        VARIANT_13,
        VARIANT_14,
        VARIANT_15,
        VARIANT_16,
    };

private:
    enum Slot_e {
        SLOT_MINI_GAME,
        SLOT_MINI_GAME_SCORE,
        SLOT_MINI_GAME_TIME,
        SLOT_MINI_GAME_BUGS,
        SLOT_MINI_GAME_PUMPKIN,
        SLOT_MINI_GAME_SCORE_SD,
    };

public:
    dLytMiniGame_c()
        : field_0x0068(0),
          field_0x0069(0),
          field_0x006A(0),
          field_0x006B(0),
          field_0x006C(0),
          field_0x006D(0),
          mVariant(1),
          field_0x3864(0),
          field_0x3865(0),
          field_0x3866(false),
          field_0x3867(0),
          field_0x3868(0) {}
    virtual ~dLytMiniGame_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

    static dLytMiniGame_c *GetInstance() {
        return sInstance;
    }

    /** Updates some window  */
    void fn_80295940();
    /** ??? */
    void fn_80295980();
    /** Checks to see if in BAMBOO_CUTTING .. BOSS_RUSH and returns some boolean (start?) */
    bool fn_80295AB0();
    /** Checks to see if in BAMBOO_CUTTING .. BOSS_RUSH and returns some boolean (end?) */
    bool fn_80295AD0();

    void setHighscore(s32 time);

    /** GUESS: sets the start for the high score? */
    void fn_80295CB0(bool);

    void timeRelatedExecute();
    void scoreRelatedExecute();

    void setDisplayedTime(s32 time);
    void setDisplayedPoints(s32 time);

    void timeRelated();
    void scoreRelated();

    /** GUESS: Assumption based on function caller */
    void setComplete() {
        field_0x3866 = true;
    }

private:
    static dLytMiniGame_c *sInstance;

    bool isLoading(const char *name) const;
    bool loadData(const char *name, s32 slot);
    void unloadData(const char *name);
    void attachLoadedData(const char *name, d2d::ResAccIf_c &resAcc);
    

    void init();

    /* 0x0068 */ u8 field_0x0068;
    /* 0x0069 */ u8 field_0x0069;
    /* 0x006A */ u8 field_0x006A;
    /* 0x006B */ u8 field_0x006B;
    /* 0x006C */ u8 field_0x006C;
    /* 0x006D */ u8 field_0x006D;

    /* 0x0070 */ d2d::ResAccIf_c mResAccStart;
    /* 0x03E0 */ d2d::ResAccIf_c mResAccScore;
    /* 0x0750 */ d2d::ResAccIf_c mResAccTime;
    /* 0x0AC0 */ d2d::ResAccIf_c mResAccBugs;
    /* 0x0E30 */ d2d::ResAccIf_c mResAccPumpkin;
    /* 0x11A0 */ d2d::ResAccIf_c mResAccScoreSd;

    /* 0x1510 */ s32 mVariant;

    /* 0x1514 */ dLytMiniGameScore_c mScore;
    /* 0x1D64 */ dLytMiniGameScoreSd_c mScoreSd;
    /* 0x1F40 */ dLytMiniGameStart_c mStart;
    /* 0x209C */ dLytMiniGameTime_c mTime;
    /* 0x2894 */ dLytMiniGameBugs_c mBugs;
    /* 0x33E4 */ dLytMiniGamePumpkin_c mPumpkin;
    /* 0x3864 */ u8 field_0x3864;
    /* 0x3865 */ u8 field_0x3865;
    /* 0x3866 */ bool field_0x3866;
    /* 0x3867 */ u8 field_0x3867;
    /* 0x3868 */ u8 field_0x3868;
};

#endif
