#ifndef D_A_E_GEKO_H
#define D_A_E_GEKO_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEgeko_c : public dAcEnBase_c {
public:
    dAcEgeko_c() : mStateMgr(*this) {}
    virtual ~dAcEgeko_c() {}

    STATE_FUNC_DECLARE(dAcEgeko_c, Wait);
    STATE_FUNC_DECLARE(dAcEgeko_c, Move);
    STATE_FUNC_DECLARE(dAcEgeko_c, Search);
    STATE_FUNC_DECLARE(dAcEgeko_c, Shield);
    STATE_FUNC_DECLARE(dAcEgeko_c, Reflect);
    STATE_FUNC_DECLARE(dAcEgeko_c, Damage);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEgeko_c);
};

#endif
