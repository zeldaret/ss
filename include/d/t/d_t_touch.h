#ifndef D_T_TOUCH_H
#define D_T_TOUCH_H

#include "d/d_area.h"
#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgTouchTag : public dTg_c {
public:
    dTgTouchTag() : mStateMgr(*this) {}
    virtual ~dTgTouchTag() {}
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dTgTouchTag, Wait);

private:
    u8 getActivatorIndex() {
        return mParams & 0xF;
    }

    u8 getZoneFlag() {
        return mParams >> 0x4;
    }

    u8 getChkFlag() {
        return mParams >> 0xC;
    }

    STATE_MGR_DECLARE(dTgTouchTag);
    dArea_c mAreaOfEffect;
    u16 mFlagTimer;
    u8 mActivatorIndex;
    u8 mZoneFlag;
    u8 mChkFlag;
};

#endif
