#include <d/a/d_a_girahimu_sword_beam.h>

SPECIAL_ACTOR_PROFILE(GH_SWORD_BEAM, dAcGirahimuSwordBeam_c, fProfile::GH_SWORD_BEAM, 0x116, 0, 0);

STATE_DEFINE(dAcGirahimuSwordBeam_c, BulletMove);
STATE_DEFINE(dAcGirahimuSwordBeam_c, Damage);

void dAcGirahimuSwordBeam_c::initializeState_BulletMove() {}
void dAcGirahimuSwordBeam_c::executeState_BulletMove() {}
void dAcGirahimuSwordBeam_c::finalizeState_BulletMove() {}
void dAcGirahimuSwordBeam_c::initializeState_Damage() {}
void dAcGirahimuSwordBeam_c::executeState_Damage() {}
void dAcGirahimuSwordBeam_c::finalizeState_Damage() {}
