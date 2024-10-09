#ifndef D_A_INVISIBLE_H
#define D_A_INVISIBLE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcInvisible : public dAcObjBase_c {
public:
    dAcInvisible() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcInvisible() {}

    STATE_FUNC_DECLARE(dAcInvisible, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcInvisible);
};

#endif
