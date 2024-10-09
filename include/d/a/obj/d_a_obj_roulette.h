#ifndef D_A_OBJ_ROULETTE_H
#define D_A_OBJ_ROULETTE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcObjRoulette_c : public dAcObjBase_c {
public:
    dAcObjRoulette_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcObjRoulette_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcObjRoulette_c);
};

#endif
