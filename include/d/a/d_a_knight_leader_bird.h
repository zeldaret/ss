#ifndef D_A_KNIGHT_LEADER_BIRD_H
#define D_A_KNIGHT_LEADER_BIRD_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcKnightLeaderBird_c : public dAcObjBase_c {
public:
    dAcKnightLeaderBird_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcKnightLeaderBird_c() {}

    STATE_FUNC_DECLARE(dAcKnightLeaderBird_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcKnightLeaderBird_c);
};

#endif
