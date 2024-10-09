#ifndef D_A_OBJ_LOTUS_FLOWER_H
#define D_A_OBJ_LOTUS_FLOWER_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOLotusFlower_c : public dAcObjBase_c { 
public:
	dAcOLotusFlower_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOLotusFlower_c() {}

	STATE_FUNC_DECLARE(dAcOLotusFlower_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOLotusFlower_c);
};

#endif
