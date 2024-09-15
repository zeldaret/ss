#include <d/tg/d_t_map_inst.h>

SPECIAL_ACTOR_PROFILE(TAG_MAP_INST, dTgMapInst_c, fProfile::TAG_MAP_INST, 0x021d, 0, 0);

int dTgMapInst_c::create() {
    field_0xfc = params & 0xff;
    return SUCCEEDED;
}

int dTgMapInst_c::doDelete() {
    return SUCCEEDED;
}

int dTgMapInst_c::actorExecute() {
    return SUCCEEDED;
}

int dTgMapInst_c::draw() {
    return SUCCEEDED;
}
