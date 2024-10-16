#ifndef D_A_OBJ_BLAST_ROCK_H
#define D_A_OBJ_BLAST_ROCK_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOBlastRock_c : public dAcObjBase_c {
public:
    dAcOBlastRock_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOBlastRock_c() {}

    STATE_FUNC_DECLARE(dAcOBlastRock_c, Wait);
    STATE_FUNC_DECLARE(dAcOBlastRock_c, Piece);
    STATE_FUNC_DECLARE(dAcOBlastRock_c, Broken);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOBlastRock_c);
};

#endif
