#ifndef D_A_OBJ_HIMO_H
#define D_A_OBJ_HIMO_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOhimo_c : public dAcObjBase_c { 
public:
	dAcOhimo_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOhimo_c() {}

	STATE_FUNC_DECLARE(dAcOhimo_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOhimo_c);
};
#endif