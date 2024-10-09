#ifndef D_A_BIRD_NPC_H
#define D_A_BIRD_NPC_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dBird_Npc_c : public dAcObjBase_c { 
public:
	dBird_Npc_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dBird_Npc_c() {}

	STATE_FUNC_DECLARE(dBird_Npc_c, Hide);
	STATE_FUNC_DECLARE(dBird_Npc_c, Fly);
	STATE_FUNC_DECLARE(dBird_Npc_c, Chase);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dBird_Npc_c);
};
#endif