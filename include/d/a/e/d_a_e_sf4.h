#ifndef D_A_E_SF4_H
#define D_A_E_SF4_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEsf4_c : public dAcEnBase_c {
public:
    dAcEsf4_c() : mStateMgr(*this) {}
    virtual ~dAcEsf4_c() {}

    STATE_FUNC_DECLARE(dAcEsf4_c, Standby);
    STATE_FUNC_DECLARE(dAcEsf4_c, Fight);
    STATE_FUNC_DECLARE(dAcEsf4_c, Attack);
    STATE_FUNC_DECLARE(dAcEsf4_c, Guard);
    STATE_FUNC_DECLARE(dAcEsf4_c, Guard2);
    STATE_FUNC_DECLARE(dAcEsf4_c, GuardJust);
    STATE_FUNC_DECLARE(dAcEsf4_c, Counter);
    STATE_FUNC_DECLARE(dAcEsf4_c, Change4Sword);
    STATE_FUNC_DECLARE(dAcEsf4_c, Damage);
    STATE_FUNC_DECLARE(dAcEsf4_c, End);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEsf4_c);
};

#endif
