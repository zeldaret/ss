#ifndef D_A_OBJ_POOL_COCK_H
#define D_A_OBJ_POOL_COCK_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOPoolCock_c : public dAcObjBase_c {
public:
    dAcOPoolCock_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOPoolCock_c() {}
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dAcOPoolCock_c, Wait);

private:
    nw4r::g3d::ResFile mBrres;
    m3d::smdl_c mModels[2];
    STATE_MGR_DECLARE(dAcOPoolCock_c);
    /* 0x3A8 */ mVec3_c mOpenDirection;
    /* 0x3B4 */ f32 mOpenProgress;
    /* 0x3B8 */ u8 mOpenSceneflag;
    /* 0x3B9 */ bool mHasActivatedVortex;
};

#endif
