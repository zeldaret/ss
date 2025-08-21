#include "d/a/obj/d_a_obj_water_jar.h"

SPECIAL_ACTOR_PROFILE(OBJ_WATER_JAR, dAcOWaterJar_c, fProfile::OBJ_WATER_JAR, 0x1AE, 0, 3);

STATE_VIRTUAL_DEFINE(dAcOWaterJar_c, DemoThrow);

void dAcOWaterJar_c::initializeState_DemoThrow() {}
void dAcOWaterJar_c::executeState_DemoThrow() {}
void dAcOWaterJar_c::finalizeState_DemoThrow() {}
