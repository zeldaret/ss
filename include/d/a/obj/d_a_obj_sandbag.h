#ifndef D_A_OBJ_SANDBAG_H
#define D_A_OBJ_SANDBAG_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSandbag_c : public dAcObjBase_c { 
public:
	dAcOSandbag_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOSandbag_c() {}

	STATE_FUNC_DECLARE(dAcOSandbag_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOSandbag_c);
};

#endif
