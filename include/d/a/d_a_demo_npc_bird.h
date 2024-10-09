#ifndef D_A_DEMO_NPC_BIRD_H
#define D_A_DEMO_NPC_BIRD_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcDemoNpcBird_c : public dAcObjBase_c { 
public:
	dAcDemoNpcBird_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcDemoNpcBird_c() {}

	STATE_FUNC_DECLARE(dAcDemoNpcBird_c, Hide);
	STATE_FUNC_DECLARE(dAcDemoNpcBird_c, Call);
	STATE_FUNC_DECLARE(dAcDemoNpcBird_c, Fly1);
	STATE_FUNC_DECLARE(dAcDemoNpcBird_c, Fly2);
	STATE_FUNC_DECLARE(dAcDemoNpcBird_c, Fly3);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcDemoNpcBird_c);
};
#endif