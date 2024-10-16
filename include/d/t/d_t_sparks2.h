#ifndef D_T_SPARKS2_H
#define D_T_SPARKS2_H

#include "d/a/d_a_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dSparks2Tag_c : public dAcBase_c {
public:
    dSparks2Tag_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dSparks2Tag_c() {}

    STATE_FUNC_DECLARE(dSparks2Tag_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dSparks2Tag_c);
};

#endif
