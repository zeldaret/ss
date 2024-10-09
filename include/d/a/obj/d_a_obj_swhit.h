#ifndef D_A_OBJ_SWHIT_H
#define D_A_OBJ_SWHIT_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOswhit_c : public dAcObjBase_c { 
public:
	dAcOswhit_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOswhit_c() {}

	STATE_FUNC_DECLARE(dAcOswhit_c, OnWait);
	STATE_FUNC_DECLARE(dAcOswhit_c, On);
	STATE_FUNC_DECLARE(dAcOswhit_c, OffWait);
	STATE_FUNC_DECLARE(dAcOswhit_c, Off);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOswhit_c);
};
#endif