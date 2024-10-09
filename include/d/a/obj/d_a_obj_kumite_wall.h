#ifndef D_A_OBJ_KUMITE_WALL_H
#define D_A_OBJ_KUMITE_WALL_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOkumiteWall_c : public dAcObjBase_c { 
public:
	dAcOkumiteWall_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOkumiteWall_c() {}

	STATE_FUNC_DECLARE(dAcOkumiteWall_c, Wait);
	STATE_FUNC_DECLARE(dAcOkumiteWall_c, Appear);
	STATE_FUNC_DECLARE(dAcOkumiteWall_c, Exist);
	STATE_FUNC_DECLARE(dAcOkumiteWall_c, Vanish);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOkumiteWall_c);
};
#endif