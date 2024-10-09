#ifndef D_A_NPC_SUISEI_NORMAL_H
#define D_A_NPC_SUISEI_NORMAL_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSuiseiNml_c : public dAcNpc_c { 
public:
	dAcNpcSuiseiNml_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcSuiseiNml_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcSuiseiNml_c);
};

#endif
