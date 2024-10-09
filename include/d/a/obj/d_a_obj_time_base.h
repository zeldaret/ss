#ifndef D_A_OBJ_TIME_BASE_H
#define D_A_OBJ_TIME_BASE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTimeBase_c : public dAcObjBase_c { 
public:
	dAcOTimeBase_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTimeBase_c() {}

	STATE_FUNC_DECLARE(dAcOTimeBase_c, Wait);
	STATE_FUNC_DECLARE(dAcOTimeBase_c, Event);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTimeBase_c);
};
#endif