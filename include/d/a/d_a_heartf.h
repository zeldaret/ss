#ifndef D_A_HEARTF_H
#define D_A_HEARTF_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcHeartf_c : public dAcObjBase_c {
public:
    dAcHeartf_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcHeartf_c() {}

    STATE_FUNC_DECLARE(dAcHeartf_c, Wait);
    STATE_FUNC_DECLARE(dAcHeartf_c, Wind);
    STATE_FUNC_DECLARE(dAcHeartf_c, Break);
    STATE_FUNC_DECLARE(dAcHeartf_c, Sync);
    STATE_FUNC_DECLARE(dAcHeartf_c, Water);
    STATE_FUNC_DECLARE(dAcHeartf_c, Spore);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcHeartf_c);
};

#endif
