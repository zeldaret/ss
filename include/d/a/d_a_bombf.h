#ifndef D_A_BOMBF_H
#define D_A_BOMBF_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcBombf_c : public dAcObjBase_c {
public:
    dAcBombf_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcBombf_c() {}

    STATE_FUNC_DECLARE(dAcBombf_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcBombf_c);
};

#endif
