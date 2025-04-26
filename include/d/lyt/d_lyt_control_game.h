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

    void fn_802CCD40(bool);

    static dLytControlGame_c *getInstance() {
        return sInstance;
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

    static dLytControlGame_c *sInstance;
};

#endif
