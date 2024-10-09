#ifndef D_A_OBJ_WOOD_BOARD_H
#define D_A_OBJ_WOOD_BOARD_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOwoodBoard_c : public dAcObjBase_c { 
public:
	dAcOwoodBoard_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOwoodBoard_c() {}

	STATE_FUNC_DECLARE(dAcOwoodBoard_c, Wait);
	STATE_FUNC_DECLARE(dAcOwoodBoard_c, Break);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOwoodBoard_c);
};

#endif
