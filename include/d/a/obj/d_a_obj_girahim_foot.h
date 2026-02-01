#ifndef D_A_OBJ_GIRAHIM_FOOT_H
#define D_A_OBJ_GIRAHIM_FOOT_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOgirahimFoot_c : public dAcObjBase_c {
public:
    dAcOgirahimFoot_c() : mStateMgr(*this) {}
    virtual ~dAcOgirahimFoot_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOgirahimFoot_c, Wait);
    STATE_FUNC_DECLARE(dAcOgirahimFoot_c, Appear);

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dShadowCircle_c mShadow;
    /* 0x358 */ STATE_MGR_DECLARE(dAcOgirahimFoot_c);
    /* 0x394 */ bool field_0x394;
};

#endif
