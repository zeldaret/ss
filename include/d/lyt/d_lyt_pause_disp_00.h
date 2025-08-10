#ifndef LYT_PAUSE_DISP_00_H
#define LYT_PAUSE_DISP_00_H

#include "d/d_cursor_hit_check.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "s/s_State.hpp"

class dLytPauseDisp00_c {
public:
    dLytPauseDisp00_c();
    virtual ~dLytPauseDisp00_c() {}

    bool build();
    bool remove();
    bool execute();
    bool draw();

    void drawDirectly();
    void requestIn(bool scroll);
    void requestOut(bool scroll);
    void requestSelect();
    void requestRingToggle();
    void requestUnselect();

    bool requestSelectGuide();
    bool requestSelectMpls();

    STATE_FUNC_DECLARE(dLytPauseDisp00_c, None);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, In);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, Wait);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, Select);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, Ring);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, GetDemo);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, Out);

    bool getField_0xE36E() const {
        return field_0xE36E;
    }

private:
    void setAnm(int idx, f32 value);
    void stopAnm(int idx);
    void playBackwards(d2d::AnmGroup_c &anm);

    // Initial setup
    void setupDisp();
    void setupInventoryWheel();
    void setupHeartPieces();
    void setupWallets();
    void setupMitts();
    void setupSailcloth();
    void setupWaterDragonScale();
    void setupFireshieldEarrings();
    void setupSongsAndLifeTree();
    void setupTabletTriforce();
    void setupSword();
    void setupStoneOfTrials();

    // Dynamic setup
    void setupRingIcons(s32 tab);

    s32 updateSelection();
    s32 getPointerPane() const;
    bool isPointingAtRingIcon();

    void hideItemIcons();
    u16 getPointedItemSpecial(s32 paneIdx, bool *pLocked);

    enum LoadRingTextCmd_e {
        RING_TEXT_RELEASE_TO_CONFIRM = 0,
        RING_TEXT_INITIAL_TAB = 1,
        RING_TEXT_CURRENT_TAB = 2,
    };

    void executeCall();
    void playOnOffTabAnim();
    void loadRingText(u32 cmd);

    /* 0x0004 */ STATE_MGR_DECLARE(dLytPauseDisp00_c);
    /* 0x0040 */ d2d::LytBase_c mLyt;
    /* 0x00D0 */ d2d::AnmGroup_c mAnm[126];
    /* 0x2050 */ dLytCommonIconItem_c mIcons[24];
    /* 0xE110 */ d2d::SubPaneList mSubpaneList;
    /* 0xE11C */ d2d::SubPaneListNode mSubpanes[24];
    /* 0xE29C */ dCursorHitCheckLyt_c mCsHitCheck;
    /* 0xE2C4 */ nw4r::lyt::Bounding *mpBoundings[37];

    /* 0xE358 */ s32 mStep;
    /* 0xE35C */ s32 mPrevNavTarget;
    /* 0xE360 */ s32 mCurrentNavTarget;
    /* 0xE364 */ s32 mCallTimerMaybe;
    /* 0xE368 */ s32 mGetDemoTimer;
    /* 0xE36C */ bool mInRequest;
    /* 0xE36D */ bool mOutRequest;
    /* 0xE36E */ bool field_0xE36E;
    /* 0xE36F */ bool mIsVisible;
    /* 0xE370 */ bool mDoScrollAnim;
    /* 0xE371 */ bool field_0xE371;
    /* 0xE372 */ bool mSelectToggleRequest;
    /* 0xE373 */ bool mSelectGuideRequest;
    /* 0xE374 */ bool mStopCallRequest;
    /* 0xE375 */ bool mSelectMplsRequest;
    /* 0xE376 */ bool mReverseTabChangeAnim;
    /* 0xE377 */ bool mRingToggleRequest;
};

#endif
