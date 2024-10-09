#ifndef D_A_CONTROL_H
#define D_A_CONTROL_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEnCl_c : public dAcObjBase_c { 
public:
	dAcEnCl_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEnCl_c() {}

	STATE_FUNC_DECLARE(dAcEnCl_c, pathMove);
	STATE_FUNC_DECLARE(dAcEnCl_c, pathRoof);
	STATE_FUNC_DECLARE(dAcEnCl_c, pathStop);
	STATE_FUNC_DECLARE(dAcEnCl_c, rangeCheck_All);
	STATE_FUNC_DECLARE(dAcEnCl_c, infinite);
	STATE_FUNC_DECLARE(dAcEnCl_c, rangeMove);
	STATE_FUNC_DECLARE(dAcEnCl_c, rangeSearch);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEnCl_c);
};
#endif