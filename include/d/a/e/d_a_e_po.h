#ifndef D_A_E_PO_H
#define D_A_E_PO_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEPo_c : public dAcEnBase_c {
public:
    dAcEPo_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEPo_c() {}

    STATE_FUNC_DECLARE(dAcEPo_c, Stop);
    STATE_FUNC_DECLARE(dAcEPo_c, Revival);
    STATE_FUNC_DECLARE(dAcEPo_c, Wait);
    STATE_FUNC_DECLARE(dAcEPo_c, Move);
    STATE_FUNC_DECLARE(dAcEPo_c, PathMove);
    STATE_FUNC_DECLARE(dAcEPo_c, Find);
    STATE_FUNC_DECLARE(dAcEPo_c, Event);
    STATE_FUNC_DECLARE(dAcEPo_c, SearchMove);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEPo_c);
};

#endif
