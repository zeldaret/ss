#ifndef D_A_NPC_BIRD_SOWOMAN_H
#define D_A_NPC_BIRD_SOWOMAN_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcBdsw_c : public dAcObjBase_c { 
public:
	dAcNpcBdsw_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcBdsw_c() {}

	STATE_FUNC_DECLARE(dAcNpcBdsw_c, Wait);
	STATE_FUNC_DECLARE(dAcNpcBdsw_c, Demo);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcBdsw_c);
};

#endif
