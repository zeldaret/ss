#ifndef D_A_OBJ_BALLISTA_HANDLE_H
#define D_A_OBJ_BALLISTA_HANDLE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOBallistaHandle_c : public dAcObjBase_c { 
public:
	dAcOBallistaHandle_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOBallistaHandle_c() {}

	STATE_FUNC_DECLARE(dAcOBallistaHandle_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOBallistaHandle_c);
};

#endif
