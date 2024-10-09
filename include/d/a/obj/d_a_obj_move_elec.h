#ifndef D_A_OBJ_MOVE_ELEC_H
#define D_A_OBJ_MOVE_ELEC_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOMoveElec_c : public dAcObjBase_c { 
public:
	dAcOMoveElec_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOMoveElec_c() {}

	STATE_FUNC_DECLARE(dAcOMoveElec_c, WaitOff);
	STATE_FUNC_DECLARE(dAcOMoveElec_c, WaitOn);
	STATE_FUNC_DECLARE(dAcOMoveElec_c, Move);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOMoveElec_c);
};
#endif