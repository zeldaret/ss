#ifndef D_A_OBJ_TORNADO_H
#define D_A_OBJ_TORNADO_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTornado_c : public dAcObjBase_c { 
public:
	dAcOTornado_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTornado_c() {}

	STATE_FUNC_DECLARE(dAcOTornado_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTornado_c);
};

#endif
