#ifndef D_T_MOLE_PROHIBIT_H
#define D_T_MOLE_PROHIBIT_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dTgMoleProhibit_c : public dTg_c {
public:
    dTgMoleProhibit_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgMoleProhibit_c() {}
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dTgMoleProhibit_c, Wait);

private:
    STATE_MGR_DECLARE(dTgMoleProhibit_c);
};

#endif
