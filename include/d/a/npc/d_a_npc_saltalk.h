#ifndef D_A_NPC_SALTALK_H
#define D_A_NPC_SALTALK_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSltk_c : public dAcNpc_c { 
public:
	dAcNpcSltk_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcSltk_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcSltk_c);
};

#endif
