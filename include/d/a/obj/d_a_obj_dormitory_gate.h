#ifndef D_A_OBJ_DORMITORY_GATE_H
#define D_A_OBJ_DORMITORY_GATE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcODormitoryGate_c : public dAcObjBase_c { 
public:
	dAcODormitoryGate_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcODormitoryGate_c() {}

	STATE_FUNC_DECLARE(dAcODormitoryGate_c, Wait);
	STATE_FUNC_DECLARE(dAcODormitoryGate_c, End);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcODormitoryGate_c);
};
#endif