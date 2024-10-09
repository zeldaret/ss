#ifndef D_A_OBJ_STAGE_SINK_H
#define D_A_OBJ_STAGE_SINK_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOstageSink_c : public dAcObjBase_c { 
public:
	dAcOstageSink_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOstageSink_c() {}

	STATE_FUNC_DECLARE(dAcOstageSink_c, Wait);
	STATE_FUNC_DECLARE(dAcOstageSink_c, Move);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOstageSink_c);
};
#endif