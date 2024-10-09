#ifndef D_A_OBJ_DUNGEON_SHIP_H
#define D_A_OBJ_DUNGEON_SHIP_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcODungeonShip_c : public dAcObjBase_c {
public:
    dAcODungeonShip_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcODungeonShip_c() {}

    STATE_FUNC_DECLARE(dAcODungeonShip_c, Wait);
    STATE_FUNC_DECLARE(dAcODungeonShip_c, Transparency);
    STATE_FUNC_DECLARE(dAcODungeonShip_c, AppearEvent);
    STATE_FUNC_DECLARE(dAcODungeonShip_c, Appear);
    STATE_FUNC_DECLARE(dAcODungeonShip_c, End);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcODungeonShip_c);
};

#endif
