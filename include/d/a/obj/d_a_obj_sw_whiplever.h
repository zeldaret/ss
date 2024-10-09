#ifndef D_A_OBJ_SW_WHIPLEVER_H
#define D_A_OBJ_SW_WHIPLEVER_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSwWhipLever_c : public dAcObjBase_c { 
public:
	dAcOSwWhipLever_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOSwWhipLever_c() {}

	STATE_FUNC_DECLARE(dAcOSwWhipLever_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOSwWhipLever_c);
};
#endif