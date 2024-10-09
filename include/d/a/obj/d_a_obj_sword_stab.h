#ifndef D_A_OBJ_SWORD_STAB_H
#define D_A_OBJ_SWORD_STAB_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSwordStab_c : public dAcObjBase_c { 
public:
	dAcOSwordStab_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOSwordStab_c() {}

	STATE_FUNC_DECLARE(dAcOSwordStab_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOSwordStab_c);
};
#endif