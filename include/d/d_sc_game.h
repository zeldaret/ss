#ifndef D_SC_GAME_H
#define D_SC_GAME_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/d_camera.h"
#include "d/d_dylink.h"
#include "d/d_fader.h"
#include "d/d_scene.h"
#include "egg/gfx/eggScreen.h"
#include "f/f_profile_name.h"
#include "m/m2d.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "s/s_FPhase.h"
#include "s/s_State.hpp"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

class ScGameScreen : public m2d::Base_c {
public:
    ScGameScreen(u8 priority) {
        setPriority(priority);
    }

private:
    /* 0x10 */ EGG::Screen mScreen;
};

class ScGameScreenTargeting : public ScGameScreen {
public:
    ScGameScreenTargeting(u8 priority) : ScGameScreen(priority), mLetterboxAmount(0.0f) {}

    f32 fn_801BBEC0() const;

private:
    /* 0x98 */ f32 mLetterboxAmount;
};

struct SpawnInfo {
    SpawnInfo() {
        reset();
    }

    SpawnInfo(const SpawnInfo &other) {
        *this = other;
    }

    SpawnInfo &operator=(const SpawnInfo &other) {
        setData(
            other.stageName, other.room, other.layer, other.entrance, other.night, other.trial, other.transitionType,
            other.transitionFadeFrames & 0xFF, other.unk
        );
        return *this;
    }

    void reset() {
        setData("", 0, 0, 0, 0, 0, 0, 0xf, 0xff);
    }

    /* 0x00 */ SizedString<32> stageName;
    /* 0x20 */ u16 transitionFadeFrames;
    /* 0x22 */ u8 room;
    /* 0x23 */ u8 layer;
    /* 0x24 */ u8 entrance;
    /* 0x25 */ u8 night;
    /* 0x26 */ u8 trial;
    /* 0x27 */ u8 transitionType;
    /* 0x28 */ s8 unk;

    enum DayNight {
        DAY,
        NIGHT,
        RETAIN_TOD,
    };

    enum Trial {
        NO_TRIAL,
        TRIAL,
        RETAIN_TRIAL,
    };

    DayNight getTimeOfDay() {
        return (DayNight)night;
    }

    Trial getTrial() {
        return (Trial)trial;
    }

    const char *getStageName() const {
        return stageName;
    }

    bool isNight() {
        return getTimeOfDay() == NIGHT;
    }

    void setData(
        const char *i_stageName, u8 i_roomid, u8 i_layer, u8 i_entrance, s32 i_night, s32 i_trial, u8 i_transitionType,
        u16 i_fadeFrames, s8 i_field0x28
    );
};

struct SpawnInfoExt : public SpawnInfo {
    /* 0x2C */ mVec3_c mType0Pos;
    /* 0x38 */ s16 mType0RotY;

    void setSpawnData(const char *i_stageName, u8 i_roomid, u8 i_layer, u8 i_entrance, const mVec3_c &pos, mAng rot);
};

struct LinkReloadInfo {
    LinkReloadInfo() : mType0CsFlag(0) {}
    void set(u8 roomid, const mVec3_c &pos, mAng rot, u32 linkParams);

    /* 0x00 */ mVec3_c mPosition;
    /* 0x0C */ mAng mRotY;
    /* 0x0E */ u8 mRoomId;
    /* 0x0F */ u8 mType0CsFlag;
    /* 0x10 */ u32 mLinkParams;
};

class dScGame_c : public dScene_c {
public:
    dScGame_c();
    virtual ~dScGame_c() {
        sInstance = nullptr;
    }

    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;
    virtual void deleteReady() override;

    void triggerExit(s32 room, u16 exitIndex, s32 forcedNight = 2, s32 forcedTrial = 2);
    void triggerEntrance(
        const char *stageName, u8 roomid, u8 layer, u8 entrance, int forcedNight = SpawnInfo::RETAIN_TOD,
        int forcedTrial = SpawnInfo::RETAIN_TRIAL, u8 transitionType = dFader_c::FADER_BLACK,
        u16 transitionFadeFrames = 15, s8 field0x28 = -1
    );
    // void triggerEntrance(const char *stageName, u8 room, );
    const LinkReloadInfo &getLinkReloadInfo() const;

    static void copySpawnNextToCurrent();

    STATE_VIRTUAL_FUNC_DECLARE(dScGame_c, Stanby); // sic
    STATE_VIRTUAL_FUNC_DECLARE(dScGame_c, Action);

    static bool isHeroMode();
    static bool isCurrentStage(const char *stageName);
    static bool isStageSkyloftLayer20();
    static bool isInCredits();
    static bool isStateLayerWithSeekerStoneHintMenu();

    static SpawnInfo currentSpawnInfo;
    static SpawnInfo nextSpawnInfo;
    static dScGame_c *sInstance;

    static dScGame_c *GetInstance() {
        return sInstance;
    }

    bool setReloadTrigger(fProfile::PROFILE_NAME_e reloadTrigger);
    void setRespawnInfo(const mVec3_c &linkPos, const mAng3_c &linkRot, bool unk);
    void setSpawnInfo(u8 roomid, const mVec3_c &pos, mAng rot);
    void clearSpawnInfo();
    const SpawnInfoExt &getSpawnInfo() const;

    static void resetUpdateFrameCount() {
        sUpdateFrameCount = 0;
    }

    bool savePromptFlag() const {
        return mSavePromptFlag;
    }

    void setSavePromptFlag(bool val) {
        mSavePromptFlag = val;
    }

    u8 getType0CsFlag() const {
        return mReloadInfo.mType0CsFlag;
    }

    void setType0CsFlag(u8 flag) {
        mReloadInfo.mType0CsFlag = flag;
    }

    u8 getType0PosFlag() const {
        return mType0PosFlag;
    }

    void setType0PosFlag(u8 flag) {
        mType0PosFlag = flag;
    }

    static u8 getReloaderType() {
        return sReloaderType;
    }

    static void setReloaderType(u8 type) {
        sReloaderType = type;
    }

    static dCamera_c *getCamera(s32 idx = 0);
    static void setCamera(s32 idx, dCamera_c *);

    bool isFaderSettled() const {
        return mFader.isSettled();
    }

    f32 targetingScreenFn_801BBEC0() const {
        return mScreen1.fn_801BBEC0();
    }

    void setTargetingScreenPrio(u8 prio) {
        mScreen1.setPriority(prio);
    }

    static bool sCopyFileBToCurrentAfterRespawn;
    static bool sDoSomethingWithFileAOnTransition;
    static bool sPreventClearingSomeFlag;

protected:
    static u32 sUpdateFrameCount;
    static u8 sCurrentLayer;
    static u8 sReloaderType;

    static void someGfxThingCallback();
    void somethingWithScreen();
    bool attemptDelete();

    static void actuallyTriggerEntrance(
        const char *stageName, u8 roomid, u8 layer, u8 entrance, int forcedNight, int forcedTrial, u8 transitionType,
        u16 transitionFadeFrames, s8 field0x28
    );

    void setFadeInType(u8 transitionType) {
        mFader.setFadeInType(transitionType);
    }

    void setFadeOutType(u8 transitionType) {
        mFader.setFadeOutType(transitionType);
    }

    void setFaderFrames(u16 transitionFadeFrames) {
        mFader.setFrames(transitionFadeFrames, transitionFadeFrames);
    }

    void setBeedleShopState(u8 state) {
        mBeedleShopState = state;
    }

    void resetLinkState() {
        mInitialSpeedAfterLoadZone = 0.0f;
        mStaminaAmount = -1;
        mItemToUseOnReload = -1;
        mActionIndex = -1;
    }

    sFPhaseBase::sFPhaseState cb1();
    sFPhaseBase::sFPhaseState cb2();
    sFPhaseBase::sFPhaseState cb3();

    static sFPhase<dScGame_c>::phaseCallback sCallbacks[];

    /* 0x068 */ sFPhase<dScGame_c> mPhases;
    /* 0x07C */ STATE_MGR_DECLARE(dScGame_c);
    /* 0x0B8 */ dDynamicModuleControl mRelCtrl;
    /* 0x0C4 */ LayoutArcControl mLayoutCtrl;
    /* 0x0D0 */ dFader_c mFader;

    /* 0x0F4 */ ScGameScreenTargeting mScreen1;
    /* 0x194 */ ScGameScreen mScreen2;
    /* 0x228 */ UNKWORD field_0x228;
    /* 0x22C */ u8 _0x22C[0x230 - 0x22C];
    /* 0x230 */ dAcRefBase_c mStageMgrRef;
    /* 0x23C */ u16 mReloadTrigger;
    /* 0x23E */ u16 field_0x23E;
    /* 0x240 */ SpawnInfoExt mSpawnInfo;
    /* 0x27C */ LinkReloadInfo mReloadInfo;
    /* 0x290 */ f32 mInitialSpeedAfterLoadZone;
    /* 0x294 */ s32 mStaminaAmount;
    /* 0x298 */ s8 mItemToUseOnReload;
    /* 0x299 */ s8 mBeedleShopState;
    /* 0x29A */ u16 mActionIndex;
    /* 0x29C */ s32 mLastAreaType;
    /* 0x2A0 */ u8 mType0PosFlag;
    /* 0x2A1 */ u8 field_0x2A1;
    /* 0x2A2 */ bool mSavePromptFlag;
    /* 0x2A3 */ bool mPreventSaveRespawnInfo;
};

#endif
