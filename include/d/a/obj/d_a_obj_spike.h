#ifndef D_A_O_SPIKE_H
#define D_A_O_SPIKE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateID.hpp"

class dAcOspike_c : public dAcObjBase_c {
public:
    dAcOspike_c() : mStateMgr(*this) {}
    virtual ~dAcOspike_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dAcOspike_c, Wait);

    static const mVec3_c sVec1;
    static const mVec3_c sVec2;

private:
    static dCcD_SrcUnk sCcSrc;

    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mMdl;
    dCcD_Unk mCollision;

    STATE_MGR_DECLARE(dAcOspike_c);
};

#endif
