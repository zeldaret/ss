#include <d/a/obj/d_a_obj_dungeon_ship.h>

SPECIAL_ACTOR_PROFILE(OBJ_DUNGEON_SHIP, dAcODungeonShip_c, fProfile::OBJ_DUNGEON_SHIP, 0X17B, 0, 3);

STATE_DEFINE(dAcODungeonShip_c, Wait);
STATE_DEFINE(dAcODungeonShip_c, Transparency);
STATE_DEFINE(dAcODungeonShip_c, AppearEvent);
STATE_DEFINE(dAcODungeonShip_c, Appear);
STATE_DEFINE(dAcODungeonShip_c, End);

void dAcODungeonShip_c::initializeState_Wait() {}
void dAcODungeonShip_c::executeState_Wait() {}
void dAcODungeonShip_c::finalizeState_Wait() {}
void dAcODungeonShip_c::initializeState_Transparency() {}
void dAcODungeonShip_c::executeState_Transparency() {}
void dAcODungeonShip_c::finalizeState_Transparency() {}
void dAcODungeonShip_c::initializeState_AppearEvent() {}
void dAcODungeonShip_c::executeState_AppearEvent() {}
void dAcODungeonShip_c::finalizeState_AppearEvent() {}
void dAcODungeonShip_c::initializeState_Appear() {}
void dAcODungeonShip_c::executeState_Appear() {}
void dAcODungeonShip_c::finalizeState_Appear() {}
void dAcODungeonShip_c::initializeState_End() {}
void dAcODungeonShip_c::executeState_End() {}
void dAcODungeonShip_c::finalizeState_End() {}
