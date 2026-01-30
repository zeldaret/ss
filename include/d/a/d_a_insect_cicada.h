#ifndef D_A_INSECT_CICADA_H
#define D_A_INSECT_CICADA_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcInsectCicada_c : public dAcObjBase_c {
public:
    dAcInsectCicada_c() : mStateMgr(*this) {}
    virtual ~dAcInsectCicada_c() {}

    STATE_FUNC_DECLARE(dAcInsectCicada_c, Wait);
    STATE_FUNC_DECLARE(dAcInsectCicada_c, Fly);
    STATE_FUNC_DECLARE(dAcInsectCicada_c, Finalize);
    STATE_FUNC_DECLARE(dAcInsectCicada_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcInsectCicada_c);
};

#endif
