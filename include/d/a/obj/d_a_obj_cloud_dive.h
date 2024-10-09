#ifndef D_A_OBJ_CLOUD_DIVE_H
#define D_A_OBJ_CLOUD_DIVE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOcloudDive_c : public dAcObjBase_c { 
public:
	dAcOcloudDive_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOcloudDive_c() {}

	STATE_FUNC_DECLARE(dAcOcloudDive_c, Wait);
	STATE_FUNC_DECLARE(dAcOcloudDive_c, Dead);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOcloudDive_c);
};

#endif
