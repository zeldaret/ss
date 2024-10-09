#ifndef D_A_OBJ_BB_BROKEN_PARTS_H
#define D_A_OBJ_BB_BROKEN_PARTS_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcObbBrokenParts_c : public dAcObjBase_c { 
public:
	dAcObbBrokenParts_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcObbBrokenParts_c() {}

	STATE_FUNC_DECLARE(dAcObbBrokenParts_c, Wait);
	STATE_FUNC_DECLARE(dAcObbBrokenParts_c, Break);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcObbBrokenParts_c);
};

#endif
