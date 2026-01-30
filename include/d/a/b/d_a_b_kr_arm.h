#ifndef D_A_B_KR_ARM_H
#define D_A_B_KR_ARM_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcBkra_c : public dAcEnBase_c {
public:
    dAcBkra_c() : mStateMgr(*this) {}
    virtual ~dAcBkra_c() {}

    STATE_FUNC_DECLARE(dAcBkra_c, Wait);
    STATE_FUNC_DECLARE(dAcBkra_c, InRoom);
    STATE_FUNC_DECLARE(dAcBkra_c, InRoomDemo);
    STATE_FUNC_DECLARE(dAcBkra_c, Escape);
    STATE_FUNC_DECLARE(dAcBkra_c, AttackA);
    STATE_FUNC_DECLARE(dAcBkra_c, AttackB);
    STATE_FUNC_DECLARE(dAcBkra_c, Damage);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcBkra_c);
};

#endif
