#ifndef D_A_OBJ_SUN_LIGHT_H
#define D_A_OBJ_SUN_LIGHT_H

#include <d/a/obj/d_a_obj_base.h>
#include <m/m3d/m_anmtexsrt.h>
#include <m/m3d/m_smdl.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOsunLight_c : public dAcObjBase_c {
public:
    dAcOsunLight_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOsunLight_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    bool isDay();

    STATE_FUNC_DECLARE(dAcOsunLight_c, Wait);

private:
    nw4r::g3d::ResFile mBrres;
    m3d::smdl_c mModel;
    STATE_MGR_DECLARE(dAcOsunLight_c);
    m3d::anmTexSrt_c mTexAnm;
};

#endif
