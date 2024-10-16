#ifndef D_A_OBJ_STAGE_COVER_H
#define D_A_OBJ_STAGE_COVER_H

#include <d/a/obj/d_a_obj_base.h>
#include <m/m3d/m_smdl.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOstageCover_c : public dAcObjBase_c {
public:
    dAcOstageCover_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOstageCover_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dAcOstageCover_c, Wait);

private:
    nw4r::g3d::ResFile mBrres;
    m3d::smdl_c mModel;
    STATE_MGR_DECLARE(dAcOstageCover_c);
    bool mDeletionRequested;
};

#endif
