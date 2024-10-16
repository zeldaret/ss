#ifndef D_A_NPC_KYUI_WIZARD_H
#define D_A_NPC_KYUI_WIZARD_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcNpcKyuiWizard_c : public dAcNpc_c {
public:
    dAcNpcKyuiWizard_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcKyuiWizard_c() {}

    STATE_FUNC_DECLARE(dAcNpcKyuiWizard_c, SleepFirst);
    STATE_FUNC_DECLARE(dAcNpcKyuiWizard_c, Sleep);
    STATE_FUNC_DECLARE(dAcNpcKyuiWizard_c, SleepRecover);
    STATE_FUNC_DECLARE(dAcNpcKyuiWizard_c, Damage);
    STATE_FUNC_DECLARE(dAcNpcKyuiWizard_c, Damage2);
    STATE_FUNC_DECLARE(dAcNpcKyuiWizard_c, WakeUpDemo1st);
    STATE_FUNC_DECLARE(dAcNpcKyuiWizard_c, WakeUpDemo);
    STATE_FUNC_DECLARE(dAcNpcKyuiWizard_c, DragonHintDemoCheck);
    STATE_FUNC_DECLARE(dAcNpcKyuiWizard_c, DragonHintDemo);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcKyuiWizard_c);
};

#endif
