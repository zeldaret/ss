#ifndef D_A_OBJ_GIRAHIM_FOOT_H
#define D_A_OBJ_GIRAHIM_FOOT_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOgirahimFoot_c : public dAcObjBase_c {
public:
    dAcOgirahimFoot_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOgirahimFoot_c() {}

    virtual int create() override;       // fn_425_4C0
    virtual int doDelete() override;     // fn_425_5C0
    virtual int draw() override;         // fn_425_610
    virtual bool createHeap() override;  // fn_425_440
    virtual int actorExecute() override; // fn_425_5D0

    STATE_FUNC_DECLARE(dAcOgirahimFoot_c, Wait);
    STATE_FUNC_DECLARE(dAcOgirahimFoot_c, Appear);

private:
    /*  */ nw4r::g3d::ResFile mRes;
    /*  */ m3d::smdl_c mMdl;
    /* 0x350 */ m3d::mShadowCircle_c mShadow;
    /* 0x??? */ STATE_MGR_DECLARE(dAcOgirahimFoot_c);
};

#endif
