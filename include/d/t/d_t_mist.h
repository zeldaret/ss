#ifndef D_T_MIST_H
#define D_T_MIST_H

#include "d/a/d_a_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dMistTag_c : public dAcBase_c {
public:
    dMistTag_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dMistTag_c() {}

    STATE_FUNC_DECLARE(dMistTag_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dMistTag_c);
};

#endif
