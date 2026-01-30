#ifndef D_A_E_BS_H
#define D_A_E_BS_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEbs_c : public dAcEnBase_c {
public:
    dAcEbs_c() : mStateMgr(*this) {}
    virtual ~dAcEbs_c() {}

    STATE_FUNC_DECLARE(dAcEbs_c, Standby);
    STATE_FUNC_DECLARE(dAcEbs_c, StandbyB);
    STATE_FUNC_DECLARE(dAcEbs_c, Fight);
    STATE_FUNC_DECLARE(dAcEbs_c, FightB);
    STATE_FUNC_DECLARE(dAcEbs_c, Attack);
    STATE_FUNC_DECLARE(dAcEbs_c, Damage);
    STATE_FUNC_DECLARE(dAcEbs_c, Fire);
    STATE_FUNC_DECLARE(dAcEbs_c, Disappear);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEbs_c);
};

#endif
