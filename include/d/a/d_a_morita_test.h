#ifndef D_A_MORITA_TEST_H
#define D_A_MORITA_TEST_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcJtest_c : public dAcEnBase_c { 
public:
	dAcJtest_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcJtest_c() {}

	STATE_FUNC_DECLARE(dAcJtest_c, FishNormalSwim);
	STATE_FUNC_DECLARE(dAcJtest_c, FishEscapeSwim);
	STATE_FUNC_DECLARE(dAcJtest_c, EelNormalSwim);
	STATE_FUNC_DECLARE(dAcJtest_c, EelEscapeSwim);
	STATE_FUNC_DECLARE(dAcJtest_c, GunkanuoSwim);
	STATE_FUNC_DECLARE(dAcJtest_c, SkyTailFly);
	STATE_FUNC_DECLARE(dAcJtest_c, BirdFly);
	STATE_FUNC_DECLARE(dAcJtest_c, Remly);
	STATE_FUNC_DECLARE(dAcJtest_c, OctaGrass);
	STATE_FUNC_DECLARE(dAcJtest_c, WaterDragon);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcJtest_c);
};
#endif