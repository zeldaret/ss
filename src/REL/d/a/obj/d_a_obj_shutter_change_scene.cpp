#include <d/a/obj/d_a_obj_shutter_change_scene.h>

SPECIAL_ACTOR_PROFILE(OBJ_SHUTTER_CHANGE_SCENE, dAcOShutterChangeScene_c, fProfile::OBJ_SHUTTER_CHANGE_SCENE, 0X18A, 0, 6);

STATE_DEFINE(dAcOShutterChangeScene_c, WaitOpenShutter);
STATE_DEFINE(dAcOShutterChangeScene_c, WaitOpenShutterForEvent);
STATE_DEFINE(dAcOShutterChangeScene_c, OpenShutter);
STATE_DEFINE(dAcOShutterChangeScene_c, WaitCloseShutter);
STATE_DEFINE(dAcOShutterChangeScene_c, CloseShutter);
STATE_DEFINE(dAcOShutterChangeScene_c, WaitOpenFence);
STATE_DEFINE(dAcOShutterChangeScene_c, OpenFence);
STATE_DEFINE(dAcOShutterChangeScene_c, CloseFence);
STATE_DEFINE(dAcOShutterChangeScene_c, ObserveSwEye);

void dAcOShutterChangeScene_c::initializeState_WaitOpenShutter() {}
void dAcOShutterChangeScene_c::executeState_WaitOpenShutter() {}
void dAcOShutterChangeScene_c::finalizeState_WaitOpenShutter() {}
void dAcOShutterChangeScene_c::initializeState_WaitOpenShutterForEvent() {}
void dAcOShutterChangeScene_c::executeState_WaitOpenShutterForEvent() {}
void dAcOShutterChangeScene_c::finalizeState_WaitOpenShutterForEvent() {}
void dAcOShutterChangeScene_c::initializeState_OpenShutter() {}
void dAcOShutterChangeScene_c::executeState_OpenShutter() {}
void dAcOShutterChangeScene_c::finalizeState_OpenShutter() {}
void dAcOShutterChangeScene_c::initializeState_WaitCloseShutter() {}
void dAcOShutterChangeScene_c::executeState_WaitCloseShutter() {}
void dAcOShutterChangeScene_c::finalizeState_WaitCloseShutter() {}
void dAcOShutterChangeScene_c::initializeState_CloseShutter() {}
void dAcOShutterChangeScene_c::executeState_CloseShutter() {}
void dAcOShutterChangeScene_c::finalizeState_CloseShutter() {}
void dAcOShutterChangeScene_c::initializeState_WaitOpenFence() {}
void dAcOShutterChangeScene_c::executeState_WaitOpenFence() {}
void dAcOShutterChangeScene_c::finalizeState_WaitOpenFence() {}
void dAcOShutterChangeScene_c::initializeState_OpenFence() {}
void dAcOShutterChangeScene_c::executeState_OpenFence() {}
void dAcOShutterChangeScene_c::finalizeState_OpenFence() {}
void dAcOShutterChangeScene_c::initializeState_CloseFence() {}
void dAcOShutterChangeScene_c::executeState_CloseFence() {}
void dAcOShutterChangeScene_c::finalizeState_CloseFence() {}
void dAcOShutterChangeScene_c::initializeState_ObserveSwEye() {}
void dAcOShutterChangeScene_c::executeState_ObserveSwEye() {}
void dAcOShutterChangeScene_c::finalizeState_ObserveSwEye() {}
