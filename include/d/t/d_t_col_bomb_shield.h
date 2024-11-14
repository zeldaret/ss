#ifndef D_T_COL_BOMB_SHIELD_H
#define D_T_COL_BOMB_SHIELD_H

#include "d/t/d_tg.h"
#include "m/m_mtx.h"
#include "s/s_State.hpp"

class dTgColBombShield : public dTg_c {
public:
    dTgColBombShield() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgColBombShield() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dTgColBombShield, Wait);

private:
    /* 0x0FC */ STATE_MGR_DECLARE(dTgColBombShield);
    void dummy_executeState() {
        // Not called from the class but needed for weak function ordering
        mStateMgr.executeState();
    }
    /* 0x138 */ mMtx_c mColMtx;
};

#endif
