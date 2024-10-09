#ifndef D_A_OBJ_DIVINER_CRYSTAL_H
#define D_A_OBJ_DIVINER_CRYSTAL_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcODivinerCrystal_c : public dAcObjBase_c { 
public:
	dAcODivinerCrystal_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcODivinerCrystal_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcODivinerCrystal_c);
};

#endif
