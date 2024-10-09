#ifndef D_A_OBJ_WARP_H
#define D_A_OBJ_WARP_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOWarp_c : public dAcObjBase_c { 
public:
	dAcOWarp_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOWarp_c() {}

	STATE_FUNC_DECLARE(dAcOWarp_c, Wait);
	STATE_FUNC_DECLARE(dAcOWarp_c, SirenFinish);
	STATE_FUNC_DECLARE(dAcOWarp_c, GetItem);
	STATE_FUNC_DECLARE(dAcOWarp_c, GateWait);
	STATE_FUNC_DECLARE(dAcOWarp_c, GateOpen);
	STATE_FUNC_DECLARE(dAcOWarp_c, GateIn);
	STATE_FUNC_DECLARE(dAcOWarp_c, GateClear);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOWarp_c);
};
#endif