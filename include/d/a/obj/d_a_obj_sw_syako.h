#ifndef D_A_OBJ_SW_SYAKO_H
#define D_A_OBJ_SW_SYAKO_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSwSyako_c : public dAcObjBase_c { 
public:
	dAcOSwSyako_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOSwSyako_c() {}

	STATE_FUNC_DECLARE(dAcOSwSyako_c, Wait);
	STATE_FUNC_DECLARE(dAcOSwSyako_c, Event);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOSwSyako_c);
};

#endif
