#ifndef D_A_OBJ_SW_BANK_SMALL_H
#define D_A_OBJ_SW_BANK_SMALL_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSwBankSmall_c : public dAcObjBase_c {
public:
    dAcOSwBankSmall_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOSwBankSmall_c() {}

    STATE_FUNC_DECLARE(dAcOSwBankSmall_c, Wait);
    STATE_FUNC_DECLARE(dAcOSwBankSmall_c, Dial);
    STATE_FUNC_DECLARE(dAcOSwBankSmall_c, Shift);
    STATE_FUNC_DECLARE(dAcOSwBankSmall_c, Correct);
    STATE_FUNC_DECLARE(dAcOSwBankSmall_c, CorrectEnd);
    STATE_FUNC_DECLARE(dAcOSwBankSmall_c, BcDemo);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSwBankSmall_c);
};

#endif
