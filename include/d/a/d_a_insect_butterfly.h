#ifndef D_A_INSECT_BUTTERFLY_H
#define D_A_INSECT_BUTTERFLY_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcInsectButterfly_c : public dAcObjBase_c { 
public:
	dAcInsectButterfly_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcInsectButterfly_c() {}

	STATE_FUNC_DECLARE(dAcInsectButterfly_c, Move);
	STATE_FUNC_DECLARE(dAcInsectButterfly_c, Escape);
	STATE_FUNC_DECLARE(dAcInsectButterfly_c, Fly);
	STATE_FUNC_DECLARE(dAcInsectButterfly_c, Finalize);
	STATE_FUNC_DECLARE(dAcInsectButterfly_c, Dead);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcInsectButterfly_c);
};

#endif
