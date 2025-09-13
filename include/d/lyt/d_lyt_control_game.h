#ifndef D_LYT_CONTROL_GAME_H
#define D_LYT_CONTROL_GAME_H

#include "d/d_base.h"
#include "s/s_State.hpp"

class dLytControlGame_c : public dBase_c {
public:
    dLytControlGame_c();
    virtual ~dLytControlGame_c();

    bool isInSomeMapState() const;
    bool isNotInStateMap() const;
    void openCollectionScreenDemo();
    bool isStateNormalOrNotInEvent() const;
    void somehowRelatedToEnteringLightPillars(s32, s32, s32);

    void fn_802CCD40(bool);
    void fn_802D04F0();

    static dLytControlGame_c *getInstance() {
        return sInstance;
    }

    s32 getCurrentPauseDisp() const {
        return mCurrentPauseDisp;
    }

    void setCurrentPauseDisp(s32 value) {
        mCurrentPauseDisp = value;
    }

    void setPauseDisp00Tab(s32 tab) {
        mPauseDisp00Tab = tab;
    }

    s32 getPauseDisp00Tab() const {
        return mPauseDisp00Tab;
    }

    u16 getItemForPauseDemo() const {
        return mItemForPauseDemo;
    }

    u16 getItemCountForPauseDemo() const {
        return mItemCountForPauseDemo;
    }

    bool isPauseDemo() const {
        return mIsPauseDemo;
    }

    u8 getPauseDemoDisp() const {
        return mPauseDemoDisp;
    }

    STATE_FUNC_DECLARE(dLytControlGame_c, Normal);
    STATE_FUNC_DECLARE(dLytControlGame_c, Pause_SetRes);
    STATE_FUNC_DECLARE(dLytControlGame_c, Pause_SyncRes);
    STATE_FUNC_DECLARE(dLytControlGame_c, Pause);
    STATE_FUNC_DECLARE(dLytControlGame_c, Pause_DelRes);
    STATE_FUNC_DECLARE(dLytControlGame_c, Map_SetRes);
    STATE_FUNC_DECLARE(dLytControlGame_c, Map_SyncRes);
    STATE_FUNC_DECLARE(dLytControlGame_c, Map);
    STATE_FUNC_DECLARE(dLytControlGame_c, Map_DelRes);

    bool isStateNormal() const {
        return *mStateMgr.getStateID() == StateID_Normal;
    }

private:
    /* 0x00068 */ UI_STATE_MGR_DECLARE(dLytControlGame_c);
    /* 0x000A4 */ u8 _0x000A4[0x15C2C - 0x000A4];

    /* 0x15C2C */ s32 mCurrentPauseDisp;
    /* 0x15C30 */ s32 mPauseDisp00Tab;

    /* 0x15C34 */ u8 _0x15C34[0x15C60 - 0x15C34];

    /* 0x15C60 */ u16 mItemForPauseDemo;

    /* 0x15C62 */ u8 _0x15C62[0x15C64 - 0x15C62];

    /* 0x15C64 */ u16 mItemCountForPauseDemo;
    /* 0x15C66 */ u8 field_0x15C66;
    /* 0x15C67 */ bool mIsPauseDemo;
    /* 0x15C68 */ u8 mPauseDemoDisp;

    static dLytControlGame_c *sInstance;
};

#endif
