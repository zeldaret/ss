#ifndef D_A_NPC_BIRD_H
#define D_A_NPC_BIRD_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dNpc_Bird_c : public dAcObjBase_c { 
public:
	dNpc_Bird_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dNpc_Bird_c() {}

	STATE_FUNC_DECLARE(dNpc_Bird_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dNpc_Bird_c);
};

#endif
