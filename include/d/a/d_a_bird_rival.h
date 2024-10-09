#ifndef D_A_BIRD_RIVAL_H
#define D_A_BIRD_RIVAL_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dBird_Rival_c : public dAcObjBase_c {
public:
    dBird_Rival_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dBird_Rival_c() {}

    STATE_FUNC_DECLARE(dBird_Rival_c, Wait);
    STATE_FUNC_DECLARE(dBird_Rival_c, Chase);
    STATE_FUNC_DECLARE(dBird_Rival_c, FrontAttack);
    STATE_FUNC_DECLARE(dBird_Rival_c, BodyAttack);
    STATE_FUNC_DECLARE(dBird_Rival_c, LevelUpDemo);
    STATE_FUNC_DECLARE(dBird_Rival_c, CatchDemo);
    STATE_FUNC_DECLARE(dBird_Rival_c, PathMove);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dBird_Rival_c);
};

#endif
