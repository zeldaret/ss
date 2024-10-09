#ifndef D_A_OBJ_ROULETTE_ISLAND_R_H
#define D_A_OBJ_ROULETTE_ISLAND_R_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOrouletteIslandR_c : public dAcObjBase_c { 
public:
	dAcOrouletteIslandR_c();
	virtual ~dAcOrouletteIslandR_c();

	STATE_FUNC_DECLARE(dAcOrouletteIslandR_c, Wait);
	STATE_FUNC_DECLARE(dAcOrouletteIslandR_c, Move);
	STATE_FUNC_DECLARE(dAcOrouletteIslandR_c, Reduce);
	STATE_FUNC_DECLARE(dAcOrouletteIslandR_c, End);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOrouletteIslandR_c);
};

#endif
