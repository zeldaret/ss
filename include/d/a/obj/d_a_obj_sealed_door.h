#ifndef D_A_OBJ_SEALED_DOOR_H
#define D_A_OBJ_SEALED_DOOR_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOsealedDoor_c : public dAcObjBase_c { 
public:
	dAcOsealedDoor_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOsealedDoor_c() {}

	STATE_FUNC_DECLARE(dAcOsealedDoor_c, Wait);
	STATE_FUNC_DECLARE(dAcOsealedDoor_c, MarkAppear);
	STATE_FUNC_DECLARE(dAcOsealedDoor_c, Light);
	STATE_FUNC_DECLARE(dAcOsealedDoor_c, AnmWait);
	STATE_FUNC_DECLARE(dAcOsealedDoor_c, Vanish);
	STATE_FUNC_DECLARE(dAcOsealedDoor_c, Appear);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOsealedDoor_c);
};
#endif