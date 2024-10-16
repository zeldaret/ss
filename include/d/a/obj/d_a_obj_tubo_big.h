#ifndef D_A_OBJ_TUBO_BIG_H
#define D_A_OBJ_TUBO_BIG_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTuboBig_c : public dAcObjBase_c {
public:
    dAcOTuboBig_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOTuboBig_c() {}

    STATE_FUNC_DECLARE(dAcOTuboBig_c, Wait);
    STATE_FUNC_DECLARE(dAcOTuboBig_c, Grab);
    STATE_FUNC_DECLARE(dAcOTuboBig_c, Water);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOTuboBig_c);
};

#endif
