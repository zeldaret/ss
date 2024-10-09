#ifndef D_A_OBJ_GIRAHIMU_FLOOR_H
#define D_A_OBJ_GIRAHIMU_FLOOR_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOGirahimuFloor_c : public dAcObjBase_c { 
public:
	dAcOGirahimuFloor_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOGirahimuFloor_c() {}

	STATE_FUNC_DECLARE(dAcOGirahimuFloor_c, Wait);
	STATE_FUNC_DECLARE(dAcOGirahimuFloor_c, Return);
	STATE_FUNC_DECLARE(dAcOGirahimuFloor_c, Generate);
	STATE_FUNC_DECLARE(dAcOGirahimuFloor_c, Broken);
	STATE_FUNC_DECLARE(dAcOGirahimuFloor_c, Move);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOGirahimuFloor_c);
};

#endif
