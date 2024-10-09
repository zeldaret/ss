#ifndef D_A_OBJ_SW_DIR_H
#define D_A_OBJ_SW_DIR_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSwDir_c : public dAcObjBase_c { 
public:
	dAcOSwDir_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOSwDir_c() {}

	STATE_FUNC_DECLARE(dAcOSwDir_c, Wait);
	STATE_FUNC_DECLARE(dAcOSwDir_c, Done);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOSwDir_c);
};

#endif
