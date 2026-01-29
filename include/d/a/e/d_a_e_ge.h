#ifndef D_A_E_GE_H
#define D_A_E_GE_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEge_c : public dAcEnBase_c {
public:
    dAcEge_c() : mStateMgr(*this) {}
    virtual ~dAcEge_c() {}

    STATE_FUNC_DECLARE(dAcEge_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEge_c);
};

#endif
