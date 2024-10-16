#ifndef D_A_E_SYAKOMAITO_H
#define D_A_E_SYAKOMAITO_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcESyakomaito_c : public dAcEnBase_c {
public:
    dAcESyakomaito_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcESyakomaito_c() {}

    STATE_FUNC_DECLARE(dAcESyakomaito_c, Wait);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, Kyoro);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, Find);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, FindSound);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, Move);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, Search);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, RetMove);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, KnockBack);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, GuardImpact);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, Damage);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, Stun);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, Dead);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, Debt);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, ReverseWait);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, Appear);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, Drive);
    STATE_FUNC_DECLARE(dAcESyakomaito_c, Teni);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcESyakomaito_c);
};

#endif
