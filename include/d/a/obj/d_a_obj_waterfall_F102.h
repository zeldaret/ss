#ifndef D_A_OBJ_WATERFALL_F102_H
#define D_A_OBJ_WATERFALL_F102_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOwaterfallF102_c : public dAcObjBase_c { 
public:
	dAcOwaterfallF102_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOwaterfallF102_c() {}

	STATE_FUNC_DECLARE(dAcOwaterfallF102_c, Wait);
	STATE_FUNC_DECLARE(dAcOwaterfallF102_c, Vanish);
	STATE_FUNC_DECLARE(dAcOwaterfallF102_c, Stop);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOwaterfallF102_c);
};
#endif