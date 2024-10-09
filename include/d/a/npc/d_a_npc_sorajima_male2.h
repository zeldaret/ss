#ifndef D_A_NPC_SORAJIMA_MALE2_H
#define D_A_NPC_SORAJIMA_MALE2_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSma2_c : public dAcNpc_c { 
public:
	dAcNpcSma2_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcSma2_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcSma2_c);
};
#endif