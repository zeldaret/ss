#ifndef D_T_SHIP_FLOOD_H
#define D_T_SHIP_FLOOD_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgShipFlood_c : public dAcBase_c { 
public:
	dTgShipFlood_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dTgShipFlood_c() {}

	STATE_FUNC_DECLARE(dTgShipFlood_c, Wait);
	STATE_FUNC_DECLARE(dTgShipFlood_c, Stop);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dTgShipFlood_c);
};
#endif