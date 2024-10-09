#ifndef D_A_OBJ_SAND_D301_H
#define D_A_OBJ_SAND_D301_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSandD301_c : public dAcObjBase_c { 
public:
	dAcOSandD301_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOSandD301_c() {}

	STATE_FUNC_DECLARE(dAcOSandD301_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOSandD301_c);
};

#endif
