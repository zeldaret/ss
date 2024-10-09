#ifndef D_A_NPC_SORAJIMA_GIRL_H
#define D_A_NPC_SORAJIMA_GIRL_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSorajimaGirl_c : public dAcNpc_c {
public:
    dAcNpcSorajimaGirl_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcSorajimaGirl_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSorajimaGirl_c);
};

#endif
