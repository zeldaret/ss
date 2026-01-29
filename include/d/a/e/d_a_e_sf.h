#ifndef D_A_E_SF_H
#define D_A_E_SF_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEsf_c : public dAcEnBase_c {
public:
    dAcEsf_c() : mStateMgr(*this) {}
    virtual ~dAcEsf_c() {}

    STATE_FUNC_DECLARE(dAcEsf_c, Standby);
    STATE_FUNC_DECLARE(dAcEsf_c, Home);
    STATE_FUNC_DECLARE(dAcEsf_c, Fight);
    STATE_FUNC_DECLARE(dAcEsf_c, Attack);
    STATE_FUNC_DECLARE(dAcEsf_c, AttackB);
    STATE_FUNC_DECLARE(dAcEsf_c, Attack2);
    STATE_FUNC_DECLARE(dAcEsf_c, Guard);
    STATE_FUNC_DECLARE(dAcEsf_c, Guard2);
    STATE_FUNC_DECLARE(dAcEsf_c, GuardJust);
    STATE_FUNC_DECLARE(dAcEsf_c, Damage);
    STATE_FUNC_DECLARE(dAcEsf_c, Fail);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEsf_c);
};

#endif
