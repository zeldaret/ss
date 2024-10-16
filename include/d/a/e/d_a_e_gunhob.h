#ifndef D_A_E_GUNHOB_H
#define D_A_E_GUNHOB_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEgunhob_c : public dAcEnBase_c {
public:
    dAcEgunhob_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEgunhob_c() {}

    STATE_FUNC_DECLARE(dAcEgunhob_c, BulletMove);
    STATE_FUNC_DECLARE(dAcEgunhob_c, BulletReflection);
    STATE_FUNC_DECLARE(dAcEgunhob_c, BombWait);
    STATE_FUNC_DECLARE(dAcEgunhob_c, BombMove);
    STATE_FUNC_DECLARE(dAcEgunhob_c, BombDelete);
    STATE_FUNC_DECLARE(dAcEgunhob_c, BombBomb);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEgunhob_c);
};

#endif
