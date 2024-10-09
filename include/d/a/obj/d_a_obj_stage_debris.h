#ifndef D_A_OBJ_STAGE_DEBRIS_H
#define D_A_OBJ_STAGE_DEBRIS_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOstageDebris_c : public dAcObjBase_c { 
public:
	dAcOstageDebris_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOstageDebris_c() {}

	STATE_FUNC_DECLARE(dAcOstageDebris_c, Wait);
	STATE_FUNC_DECLARE(dAcOstageDebris_c, Break);
	STATE_FUNC_DECLARE(dAcOstageDebris_c, After);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOstageDebris_c);
};

#endif
