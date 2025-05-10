#ifndef D_A_OBJ_LAVA_F200_H
#define D_A_OBJ_LAVA_F200_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOlavaF200_c : public dAcObjBase_c {
public:
    dAcOlavaF200_c() {}
    virtual ~dAcOlavaF200_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

private:
    nw4r::g3d::ResFile mBrres;
    m3d::smdl_c mModels[2];
    m3d::anmTexSrt_c mTexAnms[2];
    m3d::anmMatClr_c mClrAnms[2];
};

#endif
