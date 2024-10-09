#ifndef D_A_INSECT_GRASSHOPPER_H
#define D_A_INSECT_GRASSHOPPER_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcInsectGrasshopper_c : public dAcObjBase_c { 
public:
	dAcInsectGrasshopper_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcInsectGrasshopper_c() {}

	STATE_FUNC_DECLARE(dAcInsectGrasshopper_c, Wait);
	STATE_FUNC_DECLARE(dAcInsectGrasshopper_c, Walk);
	STATE_FUNC_DECLARE(dAcInsectGrasshopper_c, Jump);
	STATE_FUNC_DECLARE(dAcInsectGrasshopper_c, Fly);
	STATE_FUNC_DECLARE(dAcInsectGrasshopper_c, Finalize);
	STATE_FUNC_DECLARE(dAcInsectGrasshopper_c, Dead);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcInsectGrasshopper_c);
};

#endif
