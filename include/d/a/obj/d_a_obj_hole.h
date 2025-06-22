#ifndef D_A_OBJ_HOLE_H
#define D_A_OBJ_HOLE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOhole_c : public dAcObjBase_c {
public:
    dAcOhole_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOhole_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOhole_c, Wait);

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ STATE_MGR_DECLARE(dAcOhole_c);
    /* 0x38C */ dBgS_AcchCir mAcchCir;
    /* 0x3E8 */ dBgS_ObjAcch mObjAcch;
    /* 0x798 */ u8 field_0x798;
    /* 0x799 */ u8 field_0x799;
};

#endif
