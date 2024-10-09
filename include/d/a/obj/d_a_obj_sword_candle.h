#ifndef D_A_OBJ_SWORD_CANDLE_H
#define D_A_OBJ_SWORD_CANDLE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSwordCandle_c : public dAcObjBase_c { 
public:
	dAcOSwordCandle_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOSwordCandle_c() {}

	STATE_FUNC_DECLARE(dAcOSwordCandle_c, Wait);
	STATE_FUNC_DECLARE(dAcOSwordCandle_c, WaitClear);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOSwordCandle_c);
};

#endif
