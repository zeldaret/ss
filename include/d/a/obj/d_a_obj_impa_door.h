#ifndef D_A_OBJ_IMPA_DOOR_H
#define D_A_OBJ_IMPA_DOOR_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOimpaDoor_c : public dAcObjBase_c {
public:
    dAcOimpaDoor_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOimpaDoor_c() {}

    STATE_FUNC_DECLARE(dAcOimpaDoor_c, Wait);

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ STATE_MGR_DECLARE(dAcOimpaDoor_c);
};

#endif
