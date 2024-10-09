#ifndef D_A_OBJ_IMPA_DOOR_H
#define D_A_OBJ_IMPA_DOOR_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOimpaDoor_c : public dAcObjBase_c { 
public:
	dAcOimpaDoor_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOimpaDoor_c() {}

	STATE_FUNC_DECLARE(dAcOimpaDoor_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOimpaDoor_c);
};
#endif