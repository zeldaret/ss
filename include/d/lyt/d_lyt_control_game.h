#ifndef D_LYT_CONTROL_GAME_H
#define D_LYT_CONTROL_GAME_H

// clang-format off
// vtable order
#include "common.h"
#include "d/d_base.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/lyt/msg_window/d_lyt_simple_window.h"
#include "d/lyt/d_lyt_wipe.h"
#include "d/lyt/d_lyt_help.h"
#include "d/lyt/d_lyt_map.h"
#include "s/s_State.hpp"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/raii_ptr.h"
// clang-format on

class dLytDrawMark_c;

class dLytControlGame_c : public dBase_c {
public:
    dLytControlGame_c();
    virtual ~dLytControlGame_c();

    enum Cmd_e {
        CMD_NONE = 0,
        CMD_MAP = 1,
        CMD_PAUSE = 2,
        CMD_HELP = 3,
    };

    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

    bool isInSomeMapState() const;
    bool isNotInStateMap() const;
    bool isStateNotNormalOrInEvent() const;
    bool fn_802CFF60() const;
    
    void setMapEventDone(bool);
    void fn_802D04F0();
    void checkForBeaconNextToMark();
    
    void hideHelp();
    void changeHelpText(s32 textIndex);
    void openPause();
    void openPauseDemo();
    void openHelp(s32 textIndex);
    void openMap();
    void openMapEvent(s32 mapEvent, s32 arg1, s32 arg2);

    void resetFootsteps();

    const dMapFootPrintsQueue_c *getFootprintsQueue() const {
        return mFootprintsMgr.getQueue();
    }

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
    bool checkStoryflag(s32 flag) const;
    void fn_802CFA90();
    bool canOpenPause();
    bool releaseRes();
    bool decrementRes();

    /* 0x00068 */ UI_STATE_MGR_DECLARE(dLytControlGame_c);
    /* 0x000A4 */ RaiiPtr<dLytMap_c> mpMap;
    /* 0x000A8 */ dLytMeter_c mMeter;
    /* 0x13C10 */ dLytMsgWindow_c mMsgWindow;
    /* 0x14E34 */ dLytSimpleWindow_c mSimpleWindow;
    /* 0x152A0 */ dLytWipe_c mWipe;
    /* 0x15848 */ dLytHelp_c *mpHelp;
    /* 0x1584C */ dLytDrawMark_c *mpDrawMark;
    /* 0x15850 */ dMapFootPrintsMgr_c mFootprintsMgr;

    /* 0x15C24 */ s32 mCommand;
    /* 0x15C28 */ s32 mHelpTextIndex;
    /* 0x15C2C */ s32 mCurrentPauseDisp;
    /* 0x15C30 */ s32 mPauseDisp00Tab;
    /* 0x15C34 */ u8 mIsEvent;
    /* 0x15C38 */ s32 mMapEvent;

    /* 0x15C3C */ u8 _0x15C3C[0x15C40 - 0x15C3C];

    /* 0x15C40 */ LayoutArcControl mLytArcControl;

    /* 0x15C4C */ s32 mMapEventArg1;
    /* 0x15C50 */ s32 mMapEventArg2;

    /* 0x15C54 */ UNKWORD field_0x15C54;
    /* 0x15C58 */ UNKWORD field_0x15C58;

    /* 0x15C5C */ u8 _0x15C5C[0x15C60 - 0x15C5C];

    /* 0x15C60 */ u16 mItemForPauseDemo;
    /* 0x15C62 */ u16 mPickupItemCount;
    /* 0x15C64 */ u16 mItemCountForPauseDemo;
    /* 0x15C66 */ u8 field_0x15C66;
    /* 0x15C67 */ bool mIsPauseDemo;
    /* 0x15C68 */ u8 mPauseDemoDisp;

    static dLytControlGame_c *sInstance;
};

#endif
