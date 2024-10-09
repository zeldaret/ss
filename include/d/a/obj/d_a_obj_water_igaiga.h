#ifndef D_A_OBJ_WATER_IGAIGA_H
#define D_A_OBJ_WATER_IGAIGA_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOwaterIgaiga_c : public dAcObjBase_c { 
public:
	dAcOwaterIgaiga_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOwaterIgaiga_c() {}

	STATE_FUNC_DECLARE(dAcOwaterIgaiga_c, Wait);
	STATE_FUNC_DECLARE(dAcOwaterIgaiga_c, Stick);
	STATE_FUNC_DECLARE(dAcOwaterIgaiga_c, Away);
	STATE_FUNC_DECLARE(dAcOwaterIgaiga_c, Explode);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOwaterIgaiga_c);
};

#endif
