#ifndef D_A_E_GUMARM_H
#define D_A_E_GUMARM_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcGumarm_c : public dAcEnBase_c {
public:
    dAcGumarm_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcGumarm_c() {}

    STATE_FUNC_DECLARE(dAcGumarm_c, Hide);
    STATE_FUNC_DECLARE(dAcGumarm_c, HideWait);
    STATE_FUNC_DECLARE(dAcGumarm_c, HideWalk);
    STATE_FUNC_DECLARE(dAcGumarm_c, HideMove);
    STATE_FUNC_DECLARE(dAcGumarm_c, Wait);
    STATE_FUNC_DECLARE(dAcGumarm_c, Walk);
    STATE_FUNC_DECLARE(dAcGumarm_c, Appear);
    STATE_FUNC_DECLARE(dAcGumarm_c, KeepRock);
    STATE_FUNC_DECLARE(dAcGumarm_c, AttackMove);
    STATE_FUNC_DECLARE(dAcGumarm_c, Attack);
    STATE_FUNC_DECLARE(dAcGumarm_c, Lost);
    STATE_FUNC_DECLARE(dAcGumarm_c, Find);
    STATE_FUNC_DECLARE(dAcGumarm_c, FreezeStart);
    STATE_FUNC_DECLARE(dAcGumarm_c, Freeze);
    STATE_FUNC_DECLARE(dAcGumarm_c, FreezeEnd);
    STATE_FUNC_DECLARE(dAcGumarm_c, Sink);
    STATE_FUNC_DECLARE(dAcGumarm_c, KeepSink);
    STATE_FUNC_DECLARE(dAcGumarm_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcGumarm_c);
};

#endif
