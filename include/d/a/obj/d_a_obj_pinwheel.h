#ifndef D_A_OBJ_PINWHEEL_H
#define D_A_OBJ_PINWHEEL_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOpinwheel_c : public dAcObjBase_c {
public:
    dAcOpinwheel_c() : mStateMgr(*this) {}
    virtual ~dAcOpinwheel_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOpinwheel_c, Wait);
    STATE_FUNC_DECLARE(dAcOpinwheel_c, Acceleration);
    STATE_FUNC_DECLARE(dAcOpinwheel_c, RollMaxSpeed);
    STATE_FUNC_DECLARE(dAcOpinwheel_c, Deceleration);

private:
    bool isBeingBlown();

    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dBgW mBgW;
    /* 0x560 */ dCcD_Sph mSph;
    /* 0x6B0 */ STATE_MGR_DECLARE(dAcOpinwheel_c);
    /* 0x6EC */ s16 mStateTimer;
    /* 0x6EE */ s16 mRotationSpeed;

    static dCcD_SrcSph sSphSrc;

    static const u32 unused;

public:
    /* 0x6F0 */ bool mBeingBlown;
};

#endif
