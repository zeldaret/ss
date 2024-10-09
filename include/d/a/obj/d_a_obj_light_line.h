#ifndef D_A_OBJ_LIGHT_LINE_H
#define D_A_OBJ_LIGHT_LINE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOlightLine_c : public dAcObjBase_c { 
public:
	dAcOlightLine_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOlightLine_c() {}

	STATE_FUNC_DECLARE(dAcOlightLine_c, OutSide);
	STATE_FUNC_DECLARE(dAcOlightLine_c, InSide);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOlightLine_c);
};
#endif