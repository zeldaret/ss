#ifndef D_A_OBJ_BB_BRIDGE_H
#define D_A_OBJ_BB_BRIDGE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcObbBridge_c : public dAcObjBase_c { 
public:
	dAcObbBridge_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcObbBridge_c() {}

	STATE_FUNC_DECLARE(dAcObbBridge_c, Wait);
	STATE_FUNC_DECLARE(dAcObbBridge_c, BrokenDemo);
	STATE_FUNC_DECLARE(dAcObbBridge_c, Broken);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcObbBridge_c);
};
#endif