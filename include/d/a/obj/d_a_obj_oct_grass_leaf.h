#ifndef D_A_OBJ_OCT_GRASS_LEAF_H
#define D_A_OBJ_OCT_GRASS_LEAF_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOOctGrassLeaf_c : public dAcObjBase_c { 
public:
	dAcOOctGrassLeaf_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOOctGrassLeaf_c() {}

	STATE_FUNC_DECLARE(dAcOOctGrassLeaf_c, Wait);
	STATE_FUNC_DECLARE(dAcOOctGrassLeaf_c, Shake);
	STATE_FUNC_DECLARE(dAcOOctGrassLeaf_c, Touch);
	STATE_FUNC_DECLARE(dAcOOctGrassLeaf_c, Grab);
	STATE_FUNC_DECLARE(dAcOOctGrassLeaf_c, Broken);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOOctGrassLeaf_c);
};

#endif
