#ifndef D_A_OBJ_RUINED_SAVE_H
#define D_A_OBJ_RUINED_SAVE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOruinedSave_c : public dAcObjBase_c { 
public:
	dAcOruinedSave_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOruinedSave_c() {}

	STATE_FUNC_DECLARE(dAcOruinedSave_c, Wait);
	STATE_FUNC_DECLARE(dAcOruinedSave_c, Vanish);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOruinedSave_c);
};

#endif
