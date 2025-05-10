#ifndef D_A_OBJ_UTAJIMA_H
#define D_A_OBJ_UTAJIMA_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOutajima_c : public dAcObjBase_c {
public:
    dAcOutajima_c() {}
    virtual ~dAcOutajima_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    static void rideCallback(dBgW *, dAcObjBase_c *, dAcObjBase_c *);

    m3d::smdl_c mMdls[2];
    nw4r::g3d::ResFile mRes;
    dBgW mBgW;
    mVec3_c mVec;
};

#endif
