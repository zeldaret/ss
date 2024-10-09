#ifndef D_A_NPC_SENPAI_H
#define D_A_NPC_SENPAI_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSenpai_c : public dAcNpc_c { 
public:
	dAcNpcSenpai_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcSenpai_c() {}

	STATE_FUNC_DECLARE(dAcNpcSenpai_c, ReadyWatch1);
	STATE_FUNC_DECLARE(dAcNpcSenpai_c, ReadyWatch2);
	STATE_FUNC_DECLARE(dAcNpcSenpai_c, Watch);
	STATE_FUNC_DECLARE(dAcNpcSenpai_c, Think);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcSenpai_c);
};

#endif
