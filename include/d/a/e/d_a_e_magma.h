#ifndef D_A_E_MAGMA_H
#define D_A_E_MAGMA_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEmagma_c : public dAcObjBase_c {
public:
    dAcEmagma_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEmagma_c() {}

    STATE_FUNC_DECLARE(dAcEmagma_c, Birth);
    STATE_FUNC_DECLARE(dAcEmagma_c, Move);
    STATE_FUNC_DECLARE(dAcEmagma_c, BulletMove);
    STATE_FUNC_DECLARE(dAcEmagma_c, Frame);
    STATE_FUNC_DECLARE(dAcEmagma_c, Damage);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEmagma_c);
};

#endif
