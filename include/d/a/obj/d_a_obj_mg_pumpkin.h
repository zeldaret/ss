#ifndef D_A_OBJ_MG_PUMPKIN_H
#define D_A_OBJ_MG_PUMPKIN_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOMgPumpkin_c : public dAcObjBase_c { 
public:
	dAcOMgPumpkin_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOMgPumpkin_c() {}

	STATE_FUNC_DECLARE(dAcOMgPumpkin_c, Wait);
	STATE_FUNC_DECLARE(dAcOMgPumpkin_c, Carry);
	STATE_FUNC_DECLARE(dAcOMgPumpkin_c, Drop);
	STATE_FUNC_DECLARE(dAcOMgPumpkin_c, DropAll);
	STATE_FUNC_DECLARE(dAcOMgPumpkin_c, DropPartial);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOMgPumpkin_c);
};

#endif
