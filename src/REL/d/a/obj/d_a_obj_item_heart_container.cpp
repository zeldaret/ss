#include <d/a/obj/d_a_obj_item_heart_container.h>

SPECIAL_ACTOR_PROFILE(OBJ_ITEM_HEART_CONTAINER, dAcOItemHeartContainer_c, fProfile::OBJ_ITEM_HEART_CONTAINER, 0x2C, 0, 3);

STATE_DEFINE(dAcOItemHeartContainer_c, Hide);
STATE_DEFINE(dAcOItemHeartContainer_c, RequestAppearEvent);
STATE_DEFINE(dAcOItemHeartContainer_c, AppearEffect);
STATE_DEFINE(dAcOItemHeartContainer_c, Appear);
STATE_DEFINE(dAcOItemHeartContainer_c, WaitFall);
STATE_DEFINE(dAcOItemHeartContainer_c, Fall);
STATE_DEFINE(dAcOItemHeartContainer_c, WaitGet);
STATE_DEFINE(dAcOItemHeartContainer_c, WaitGetQuake);

dAcOItemHeartContainer_c::dAcOItemHeartContainer_c() : mStateMgr(*this, sStateID::null) {}
dAcOItemHeartContainer_c::~dAcOItemHeartContainer_c() {}

void dAcOItemHeartContainer_c::initializeState_Hide() {}
void dAcOItemHeartContainer_c::executeState_Hide() {}
void dAcOItemHeartContainer_c::finalizeState_Hide() {}
void dAcOItemHeartContainer_c::initializeState_RequestAppearEvent() {}
void dAcOItemHeartContainer_c::executeState_RequestAppearEvent() {}
void dAcOItemHeartContainer_c::finalizeState_RequestAppearEvent() {}
void dAcOItemHeartContainer_c::initializeState_AppearEffect() {}
void dAcOItemHeartContainer_c::executeState_AppearEffect() {}
void dAcOItemHeartContainer_c::finalizeState_AppearEffect() {}
void dAcOItemHeartContainer_c::initializeState_Appear() {}
void dAcOItemHeartContainer_c::executeState_Appear() {}
void dAcOItemHeartContainer_c::finalizeState_Appear() {}
void dAcOItemHeartContainer_c::initializeState_WaitFall() {}
void dAcOItemHeartContainer_c::executeState_WaitFall() {}
void dAcOItemHeartContainer_c::finalizeState_WaitFall() {}
void dAcOItemHeartContainer_c::initializeState_Fall() {}
void dAcOItemHeartContainer_c::executeState_Fall() {}
void dAcOItemHeartContainer_c::finalizeState_Fall() {}
void dAcOItemHeartContainer_c::initializeState_WaitGet() {}
void dAcOItemHeartContainer_c::executeState_WaitGet() {}
void dAcOItemHeartContainer_c::finalizeState_WaitGet() {}
void dAcOItemHeartContainer_c::initializeState_WaitGetQuake() {}
void dAcOItemHeartContainer_c::executeState_WaitGetQuake() {}
void dAcOItemHeartContainer_c::finalizeState_WaitGetQuake() {}
