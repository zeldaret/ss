#ifndef D_A_OBJ_TACKLE_H
#define D_A_OBJ_TACKLE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTackle_c : public dAcObjBase_c { 
public:
	dAcOTackle_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTackle_c() {}

	STATE_FUNC_DECLARE(dAcOTackle_c, Wait);
	STATE_FUNC_DECLARE(dAcOTackle_c, Broken);
	STATE_FUNC_DECLARE(dAcOTackle_c, ResqueMole);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTackle_c);
};
#endif