#ifndef D_A_BIRD_KOBUNA_H
#define D_A_BIRD_KOBUNA_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dBird_KobunA_c : public dAcObjBase_c {
public:
    dBird_KobunA_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dBird_KobunA_c() {}

    STATE_FUNC_DECLARE(dBird_KobunA_c, Wait);
    STATE_FUNC_DECLARE(dBird_KobunA_c, Chase);
    STATE_FUNC_DECLARE(dBird_KobunA_c, FrontAttack);
    STATE_FUNC_DECLARE(dBird_KobunA_c, PathMove);
    STATE_FUNC_DECLARE(dBird_KobunA_c, LevelUpDemo);
    STATE_FUNC_DECLARE(dBird_KobunA_c, BodyAttack);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dBird_KobunA_c);
};

#endif
