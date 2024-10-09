#ifndef D_A_OBJ_GIRAHIM_FOOT_H
#define D_A_OBJ_GIRAHIM_FOOT_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOgirahimFoot_c : public dAcObjBase_c { 
public:
	dAcOgirahimFoot_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOgirahimFoot_c() {}

	STATE_FUNC_DECLARE(dAcOgirahimFoot_c, Wait);
	STATE_FUNC_DECLARE(dAcOgirahimFoot_c, Appear);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOgirahimFoot_c);
};
#endif