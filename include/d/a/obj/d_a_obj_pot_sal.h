#ifndef D_A_OBJ_POT_SAL_H
#define D_A_OBJ_POT_SAL_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOpotSal_c : public dAcObjBase_c { 
public:
	dAcOpotSal_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOpotSal_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOpotSal_c);
};

#endif
