#ifndef D_A_E_BEAMOS_H
#define D_A_E_BEAMOS_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEbeamos : public dAcEnBase_c {
public:
    dAcEbeamos() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEbeamos() {}

    STATE_FUNC_DECLARE(dAcEbeamos, Attack);
    STATE_FUNC_DECLARE(dAcEbeamos, Beetle);
    STATE_FUNC_DECLARE(dAcEbeamos, Blow);
    STATE_FUNC_DECLARE(dAcEbeamos, Damage);
    STATE_FUNC_DECLARE(dAcEbeamos, Dead);
    STATE_FUNC_DECLARE(dAcEbeamos, Rail);
    STATE_FUNC_DECLARE(dAcEbeamos, Shock);
    STATE_FUNC_DECLARE(dAcEbeamos, Stun);
    STATE_FUNC_DECLARE(dAcEbeamos, Wait);
    STATE_FUNC_DECLARE(dAcEbeamos, Watch);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEbeamos);
};

#endif
