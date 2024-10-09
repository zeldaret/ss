#ifndef D_A_OBJ_HARP_HINT_H
#define D_A_OBJ_HARP_HINT_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOHarpHint_c : public dAcObjBase_c { 
public:
	dAcOHarpHint_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOHarpHint_c() {}

	STATE_FUNC_DECLARE(dAcOHarpHint_c, Wait);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, Rotate);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, Shift);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, TalkStart);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, Talk);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, Beetle);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, Bomb);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, Whip);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, WhipRot);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, Water);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, Wind);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, HorizonHit);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, VerticalHit);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, Appear);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, AppearShiekah);
	STATE_FUNC_DECLARE(dAcOHarpHint_c, ItemGen);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOHarpHint_c);
};
#endif