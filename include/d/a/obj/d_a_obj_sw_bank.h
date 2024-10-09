#ifndef D_A_OBJ_SW_BANK_H
#define D_A_OBJ_SW_BANK_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSwBank_c : public dAcObjBase_c { 
public:
	dAcOSwBank_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOSwBank_c() {}

	STATE_FUNC_DECLARE(dAcOSwBank_c, Wait);
	STATE_FUNC_DECLARE(dAcOSwBank_c, Dial);
	STATE_FUNC_DECLARE(dAcOSwBank_c, Shift);
	STATE_FUNC_DECLARE(dAcOSwBank_c, Correct);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOSwBank_c);
};

#endif
