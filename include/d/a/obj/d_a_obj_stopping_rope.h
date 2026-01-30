#ifndef D_A_OBJ_STOPPING_ROPE_H
#define D_A_OBJ_STOPPING_ROPE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateInterfaces.hpp"

class dAcOStoppingRope_c : public dAcObjBase_c {
public:
    dAcOStoppingRope_c() : mStateMgr(*this) {}
    virtual ~dAcOStoppingRope_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    void changeState(const sStateIDIf_c &value) {
        mStateMgr.changeState(value);
    }

    STATE_FUNC_DECLARE(dAcOStoppingRope_c, Wait);
    STATE_FUNC_DECLARE(dAcOStoppingRope_c, SwitchOn);

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dBgW mBgW;
    /* 0x360 */ dCcD_Sph mCollider;
    /* 0x6B0 */ STATE_MGR_DECLARE(dAcOStoppingRope_c);
    /* 0x6EC */ u8 padding_0x6EC[16];
};

#endif
