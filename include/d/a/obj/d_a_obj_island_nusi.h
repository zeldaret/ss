#ifndef D_A_OBJ_ISLAND_NUSI_H
#define D_A_OBJ_ISLAND_NUSI_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOislandNusi_c : public dAcObjBase_c { 
public:
	dAcOislandNusi_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOislandNusi_c() {}

	STATE_FUNC_DECLARE(dAcOislandNusi_c, Wait);
	STATE_FUNC_DECLARE(dAcOislandNusi_c, NusiFight);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOislandNusi_c);
};
#endif