#ifndef D_A_OBJ_MUSHROOM_H
#define D_A_OBJ_MUSHROOM_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOMushRoom_c : public dAcObjBase_c { 
public:
	dAcOMushRoom_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOMushRoom_c() {}

	STATE_FUNC_DECLARE(dAcOMushRoom_c, Wait);
	STATE_FUNC_DECLARE(dAcOMushRoom_c, Init);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOMushRoom_c);
};
#endif