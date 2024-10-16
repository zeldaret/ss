#ifndef D_A_OBJ_SMOKE_H
#define D_A_OBJ_SMOKE_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOsmoke_c : public dAcObjBase_c {
public:
    dAcOsmoke_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOsmoke_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dAcOsmoke_c, Wait);

private:
    nw4r::g3d::ResFile mBrres;
    m3d::smdl_c mModel;
    m3d::anmTexSrt_c mTexAnm;
    STATE_MGR_DECLARE(dAcOsmoke_c);
    u8 mType;
};

#endif
