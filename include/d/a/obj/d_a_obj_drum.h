#ifndef D_A_OBJ_DRUM_H
#define D_A_OBJ_DRUM_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOdrum_c : public dAcObjBase_c { 
public:
	dAcOdrum_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOdrum_c() {}

	STATE_FUNC_DECLARE(dAcOdrum_c, Wait);
	STATE_FUNC_DECLARE(dAcOdrum_c, Move);
	STATE_FUNC_DECLARE(dAcOdrum_c, MoveEnd);
	STATE_FUNC_DECLARE(dAcOdrum_c, Fall);
	STATE_FUNC_DECLARE(dAcOdrum_c, Init);
	STATE_FUNC_DECLARE(dAcOdrum_c, BeltMove);
	STATE_FUNC_DECLARE(dAcOdrum_c, TeniN);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOdrum_c);
};

#endif
