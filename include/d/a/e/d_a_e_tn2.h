#ifndef D_A_E_TN2_H
#define D_A_E_TN2_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEtn2_c : public dAcEnBase_c {
public:
    dAcEtn2_c() : mStateMgr(*this) {}
    virtual ~dAcEtn2_c() {}

    STATE_FUNC_DECLARE(dAcEtn2_c, ArmorAttack);
    STATE_FUNC_DECLARE(dAcEtn2_c, ArmorWait);
    STATE_FUNC_DECLARE(dAcEtn2_c, ArmorWalk);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEtn2_c);
};

#endif
