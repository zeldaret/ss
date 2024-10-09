#include <d/a/npc/d_a_npc_kyui_wizard.h>

SPECIAL_ACTOR_PROFILE(NPC_KYUI_WIZARD, dAcNpcKyuiWizard_c, fProfile::NPC_KYUI_WIZARD, 0X42, 0, 2);

STATE_DEFINE(dAcNpcKyuiWizard_c, SleepFirst);
STATE_DEFINE(dAcNpcKyuiWizard_c, Sleep);
STATE_DEFINE(dAcNpcKyuiWizard_c, SleepRecover);
STATE_DEFINE(dAcNpcKyuiWizard_c, Damage);
STATE_DEFINE(dAcNpcKyuiWizard_c, Damage2);
STATE_DEFINE(dAcNpcKyuiWizard_c, WakeUpDemo1st);
STATE_DEFINE(dAcNpcKyuiWizard_c, WakeUpDemo);
STATE_DEFINE(dAcNpcKyuiWizard_c, DragonHintDemoCheck);
STATE_DEFINE(dAcNpcKyuiWizard_c, DragonHintDemo);

void dAcNpcKyuiWizard_c::initializeState_SleepFirst() {}
void dAcNpcKyuiWizard_c::executeState_SleepFirst() {}
void dAcNpcKyuiWizard_c::finalizeState_SleepFirst() {}
void dAcNpcKyuiWizard_c::initializeState_Sleep() {}
void dAcNpcKyuiWizard_c::executeState_Sleep() {}
void dAcNpcKyuiWizard_c::finalizeState_Sleep() {}
void dAcNpcKyuiWizard_c::initializeState_SleepRecover() {}
void dAcNpcKyuiWizard_c::executeState_SleepRecover() {}
void dAcNpcKyuiWizard_c::finalizeState_SleepRecover() {}
void dAcNpcKyuiWizard_c::initializeState_Damage() {}
void dAcNpcKyuiWizard_c::executeState_Damage() {}
void dAcNpcKyuiWizard_c::finalizeState_Damage() {}
void dAcNpcKyuiWizard_c::initializeState_Damage2() {}
void dAcNpcKyuiWizard_c::executeState_Damage2() {}
void dAcNpcKyuiWizard_c::finalizeState_Damage2() {}
void dAcNpcKyuiWizard_c::initializeState_WakeUpDemo1st() {}
void dAcNpcKyuiWizard_c::executeState_WakeUpDemo1st() {}
void dAcNpcKyuiWizard_c::finalizeState_WakeUpDemo1st() {}
void dAcNpcKyuiWizard_c::initializeState_WakeUpDemo() {}
void dAcNpcKyuiWizard_c::executeState_WakeUpDemo() {}
void dAcNpcKyuiWizard_c::finalizeState_WakeUpDemo() {}
void dAcNpcKyuiWizard_c::initializeState_DragonHintDemoCheck() {}
void dAcNpcKyuiWizard_c::executeState_DragonHintDemoCheck() {}
void dAcNpcKyuiWizard_c::finalizeState_DragonHintDemoCheck() {}
void dAcNpcKyuiWizard_c::initializeState_DragonHintDemo() {}
void dAcNpcKyuiWizard_c::executeState_DragonHintDemo() {}
void dAcNpcKyuiWizard_c::finalizeState_DragonHintDemo() {}
