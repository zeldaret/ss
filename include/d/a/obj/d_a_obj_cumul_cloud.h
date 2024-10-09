#ifndef D_A_OBJ_CUMUL_CLOUD_H
#define D_A_OBJ_CUMUL_CLOUD_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOcumulCloud_c : public dAcObjBase_c { 
public:
	dAcOcumulCloud_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOcumulCloud_c() {}

	STATE_FUNC_DECLARE(dAcOcumulCloud_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOcumulCloud_c);
};

#endif
