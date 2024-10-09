#ifndef D_A_OBJ_FORTUNE_RING_H
#define D_A_OBJ_FORTUNE_RING_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOfortuneRing_c : public dAcObjBase_c { 
public:
	dAcOfortuneRing_c();
	virtual ~dAcOfortuneRing_c();

	STATE_FUNC_DECLARE(dAcOfortuneRing_c, Wait);
	STATE_FUNC_DECLARE(dAcOfortuneRing_c, Play);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOfortuneRing_c);
};
#endif