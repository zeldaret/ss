#include <d/a/d_a_shop_sample.h>

SPECIAL_ACTOR_PROFILE(SHOP_SAMPLE, dAcShopSample_c, fProfile::SHOP_SAMPLE, 0X276, 0, 3);

STATE_DEFINE(dAcShopSample_c, Wait);
STATE_DEFINE(dAcShopSample_c, PickUp);
STATE_DEFINE(dAcShopSample_c, PickingUp);
STATE_DEFINE(dAcShopSample_c, PutBack);

void dAcShopSample_c::initializeState_Wait() {}
void dAcShopSample_c::executeState_Wait() {}
void dAcShopSample_c::finalizeState_Wait() {}
void dAcShopSample_c::initializeState_PickUp() {}
void dAcShopSample_c::executeState_PickUp() {}
void dAcShopSample_c::finalizeState_PickUp() {}
void dAcShopSample_c::initializeState_PickingUp() {}
void dAcShopSample_c::executeState_PickingUp() {}
void dAcShopSample_c::finalizeState_PickingUp() {}
void dAcShopSample_c::initializeState_PutBack() {}
void dAcShopSample_c::executeState_PutBack() {}
void dAcShopSample_c::finalizeState_PutBack() {}
