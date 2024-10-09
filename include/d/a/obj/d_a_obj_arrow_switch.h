#ifndef D_A_OBJ_ARROW_SWITCH_H
#define D_A_OBJ_ARROW_SWITCH_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOarrowSwitch_c : public dAcObjBase_c { 
public:
	dAcOarrowSwitch_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOarrowSwitch_c() {}

	STATE_FUNC_DECLARE(dAcOarrowSwitch_c, Off);
	STATE_FUNC_DECLARE(dAcOarrowSwitch_c, On);
	STATE_FUNC_DECLARE(dAcOarrowSwitch_c, FixedOn);
	STATE_FUNC_DECLARE(dAcOarrowSwitch_c, OpenEye);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOarrowSwitch_c);
};

#endif
