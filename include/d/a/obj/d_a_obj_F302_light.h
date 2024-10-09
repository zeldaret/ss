#ifndef D_A_OBJ_F302_LIGHT_H
#define D_A_OBJ_F302_LIGHT_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOF302Light_c : public dAcObjBase_c { 
public:
	dAcOF302Light_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOF302Light_c() {}

	STATE_FUNC_DECLARE(dAcOF302Light_c, Wait);
	STATE_FUNC_DECLARE(dAcOF302Light_c, SwitchOn);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOF302Light_c);
};

#endif
