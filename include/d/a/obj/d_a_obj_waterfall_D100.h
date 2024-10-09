#ifndef D_A_OBJ_WATERFALL_D100_H
#define D_A_OBJ_WATERFALL_D100_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOwaterfallD100_c : public dAcObjBase_c { 
public:
	dAcOwaterfallD100_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOwaterfallD100_c() {}

	STATE_FUNC_DECLARE(dAcOwaterfallD100_c, Wait);
	STATE_FUNC_DECLARE(dAcOwaterfallD100_c, Spout);
	STATE_FUNC_DECLARE(dAcOwaterfallD100_c, After);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOwaterfallD100_c);
};

#endif
