#ifndef D_A_OBJ_WATER_SHIELD_H
#define D_A_OBJ_WATER_SHIELD_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/stage_render_stuff.h"

class dAcOwaterShield_c : public dAcObjBase_c {
public:
    dAcOwaterShield_c() : mScnCallback(this) {}
    virtual ~dAcOwaterShield_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dBgW mBgW;
    /* 0x560 */ m3d::anmTexSrt_c mAnmSrt;
    /* 0x58C */ m3d::anmMatClr_c mAnmClr;
    /* 0x5B8 */ dScnCallback_c mScnCallback;
};

#endif
