#ifndef D_A_OBJ_DECOB_H
#define D_A_OBJ_DECOB_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcODecoB_c : public dAcObjBase_c { 
public:
	dAcODecoB_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcODecoB_c() {}

	STATE_FUNC_DECLARE(dAcODecoB_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcODecoB_c);
};
#endif