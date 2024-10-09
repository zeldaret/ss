#ifndef D_A_OBJ_UNDERGROUND_SWITCH_H
#define D_A_OBJ_UNDERGROUND_SWITCH_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOUgSwitch_c : public dAcObjBase_c { 
public:
	dAcOUgSwitch_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOUgSwitch_c() {}

	STATE_FUNC_DECLARE(dAcOUgSwitch_c, Off);
	STATE_FUNC_DECLARE(dAcOUgSwitch_c, Active);
	STATE_FUNC_DECLARE(dAcOUgSwitch_c, On);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOUgSwitch_c);
};

#endif
