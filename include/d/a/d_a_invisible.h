#ifndef D_A_INVISIBLE_H
#define D_A_INVISIBLE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "s/s_State.hpp"

class dAcInvisible : public dAcObjBase_c {
public:
    dAcInvisible() : mStateMgr(*this) {}
    virtual ~dAcInvisible() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcInvisible, Wait);

private:
    /* 0x330 */ dCcD_Sph mSph;
    /* 0x480 */ dCcD_Cps mCps;
    /* 0x5F0 */ dBgS_AcchCir mAcchCir;
    /* 0x64C */ dBgS_ObjAcch mObjAcch;
    /* 0x9FC */ STATE_MGR_DECLARE(dAcInvisible);
};

#endif
