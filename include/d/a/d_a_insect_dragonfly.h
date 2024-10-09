#ifndef D_A_INSECT_DRAGONFLY_H
#define D_A_INSECT_DRAGONFLY_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcInsectDragonfly_c : public dAcObjBase_c { 
public:
	dAcInsectDragonfly_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcInsectDragonfly_c() {}

	STATE_FUNC_DECLARE(dAcInsectDragonfly_c, Wait);
	STATE_FUNC_DECLARE(dAcInsectDragonfly_c, Move);
	STATE_FUNC_DECLARE(dAcInsectDragonfly_c, Escape);
	STATE_FUNC_DECLARE(dAcInsectDragonfly_c, Fly);
	STATE_FUNC_DECLARE(dAcInsectDragonfly_c, Finalize);
	STATE_FUNC_DECLARE(dAcInsectDragonfly_c, Dead);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcInsectDragonfly_c);
};
#endif