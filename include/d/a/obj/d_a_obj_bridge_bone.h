#ifndef D_A_OBJ_BRIDGE_BONE_H
#define D_A_OBJ_BRIDGE_BONE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOBridgeBone_c : public dAcObjBase_c { 
public:
	dAcOBridgeBone_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOBridgeBone_c() {}

	STATE_FUNC_DECLARE(dAcOBridgeBone_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOBridgeBone_c);
};

#endif
