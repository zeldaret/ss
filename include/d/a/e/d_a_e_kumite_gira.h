#ifndef D_A_E_KUMITE_GIRA_H
#define D_A_E_KUMITE_GIRA_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEKumiteGira_c : public dAcEnBase_c {
public:
    dAcEKumiteGira_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEKumiteGira_c() {}

    STATE_FUNC_DECLARE(dAcEKumiteGira_c, Wait);
    STATE_FUNC_DECLARE(dAcEKumiteGira_c, PlayAnm);
    STATE_FUNC_DECLARE(dAcEKumiteGira_c, Damage);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEKumiteGira_c);
};

#endif
