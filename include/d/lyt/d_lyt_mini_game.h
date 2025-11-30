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
    dLytMiniGamePumpkinParts_c() {}
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
    dLytMiniGamePumpkin_c() {}
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
    dLytMiniGameBugs_c() {}
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
    dLytMiniGameTime_c() {}
    virtual ~dLytMiniGameTime_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();
    void init();

private:
    enum TimerVariant_e {
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
    /* 0x7E4 */ u8 mTimerVariant;
    /* 0x7E4 */ s32 mHighestDigitIndex;
    /* 0x7EC */ bool field_0x7EC;
    /* 0x7ED */ bool field_0x7ED;
    /* 0x7EE */ bool field_0x7EE;
    /* 0x7F0 */ UNKWORD field_0x7F0;
    /* 0x7F4 */ u8 field_0x7F4;
    /* 0x7F5 */ bool mIsVisible;
};

/** 2D UI - Mini Game - Start / Finish Popup */
class dLytMiniGameStart_c {
public:
    dLytMiniGameStart_c() {}
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
    dLytMiniGameScore_c() {}
    virtual ~dLytMiniGameScore_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();
    void init();

private:
    void fn_80291410();
    void fn_802915B0();
    void fn_80291640();
    void fn_80291740();
    void startAlphaIn();
    void startAlphaOut();
    void fn_80291960();
    void fn_80291A30();
    void startLoop();
    void fn_80291BA0(s32 score);
    void fn_80291BC0();
    void fn_80291D40(s32 arg);
    bool fn_80291E50() const;
    void fn_80291E90();
    void fn_80291ED0();
    void fn_80291EF0();
    void fn_80292030(s32);
    void fn_80292040();
    void fn_802920B0();
    void fn_80292110();
    void setDigit(s32 digitIndex, s32 number);
    void fn_80292240();
    void fn_80292250();
    void fn_802922F0();
    void fn_80292380();
    void fn_80292400();
    void fn_80292480();
    void fn_80292500();
    void fn_80292590();
    void fn_80292610();
    void fn_80292710();
    void fn_80292880();
    void fn_80292910();
    void fn_80292920();
    bool fn_80292930() const;
    void fn_80292940();
    void fn_80292950();
    void fn_802929C0();
    void fn_80292A40();
    bool fn_80292A50() const;
    bool fn_80292AA0() const;
    bool fn_80292AF0() const;
    bool fn_80292B40() const;
    bool fn_80292B90() const;
    bool fn_80292BE0() const;
    void fn_80292C30();
    void fn_802933A0(s32 variant);
    void fn_80293410();
    void fn_80293450(s32);

    static dLytMiniGameScore_c *sInstance;

    /* 0x004 */ d2d::LytBase_c mLyt;
    /* 0x094 */ d2d::AnmGroup_c mAnm[26];

    /* 0x714 */ nw4r::lyt::Pane *mpPanes[20];

    /* 0x764 */ f32 mFloats[26];

    /* 0x7CC */ dTextBox_c *mpTextBoxes[3];

    /* 0x7D8 */ s32 field_0x7D8;
    
    /* 0x7DC */ UNKWORD field_0x7DC;
    /* 0x7E0 */ s32 field_0x7E0;
    /* 0x7E4 */ UNKWORD field_0x7E4;

    /* 0x7E8 */ f32 field_0x7E8;
    /* 0x7EC */ u8 _0x7EC[0x7FC - 0x7EC];

    /* 0x7FC */ s32 field_0x7FC;
    /* 0x800 */ s32 field_0x800;
    /* 0x804 */ bool field_0x804;
    /* 0x808 */ s32 field_0x808;
    /* 0x80C */ s32 field_0x80C;
    /* 0x810 */ s32 field_0x810;
    /* 0x814 */ bool field_0x814;
    /* 0x815 */ bool field_0x815;
    /* 0x816 */ bool field_0x816;
    /* 0x818 */ s32 field_0x818;
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
    dLytMiniGameScoreSd_c() {}
    virtual ~dLytMiniGameScoreSd_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();
    void init();

private:
    static dLytMiniGameScoreSd_c *sInstance;

    void fn_80293A30();
    void fn_80293A90();
    void fn_80293AF0();
    void fn_80293BB0();
    void fn_80293C60();
    void fn_80293D40();
    void fn_80293DC0();
    void fn_80293E40();
    void fn_80293ED0();
    void fn_80293F50();
    void fn_80293FB0();
    void fn_80294010();
    void fn_80294020();
    void fn_80294030();
    void fn_80294080();

    /* 0x004 */ d2d::LytBase_c mLyt;
    /* 0x094 */ d2d::AnmGroup_c mAnm[5];
};

class dLytMiniGame_c : public dBase_c {
public:
    dLytMiniGame_c()
        : field_0x0068(0),
          field_0x0069(0),
          field_0x006A(0),
          field_0x006B(0),
          field_0x006C(0),
          field_0x006D(0),
          field_0x1510(1) {}
    virtual ~dLytMiniGame_c() {}

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

    /* 0x0068 */ u8 field_0x0068;
    /* 0x0069 */ u8 field_0x0069;
    /* 0x006A */ u8 field_0x006A;
    /* 0x006B */ u8 field_0x006B;
    /* 0x006C */ u8 field_0x006C;
    /* 0x006D */ u8 field_0x006D;

    /* 0x0070 */ d2d::ResAccIf_c mResAcc1;
    /* 0x03E0 */ d2d::ResAccIf_c mResAcc2;
    /* 0x0750 */ d2d::ResAccIf_c mResAcc3;
    /* 0x0AC0 */ d2d::ResAccIf_c mResAcc4;
    /* 0x0E30 */ d2d::ResAccIf_c mResAcc5;
    /* 0x11A0 */ d2d::ResAccIf_c mResAcc6;

    /* 0x1510 */ UNKWORD field_0x1510;

    /* 0x1514 */ dLytMiniGameScore_c mScore;
    /* 0x1D64 */ dLytMiniGameScoreSd_c mScoreSd;
    /* 0x1F40 */ dLytMiniGameStart_c mStart;
    /* 0x209C */ dLytMiniGameTime_c mTime;
    /* 0x2894 */ dLytMiniGameBugs_c mBugs;
    /* 0x33E4 */ dLytMiniGamePumpkin_c mPumpkin;

    // TODO
    u8 _0x___[0xA];

    bool field_0x3866;
};

#endif
