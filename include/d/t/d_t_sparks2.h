#ifndef D_T_SPARKS2_H
#define D_T_SPARKS2_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dSparks2Tag_c : public dTg_c {
public:
    dSparks2Tag_c() : mStateMgr(*this) {}
    virtual ~dSparks2Tag_c() {}

    STATE_FUNC_DECLARE(dSparks2Tag_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dSparks2Tag_c);
};

#endif
