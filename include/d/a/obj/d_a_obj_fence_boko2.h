#ifndef D_A_OBJ_FENCE_BOKO2_H
#define D_A_OBJ_FENCE_BOKO2_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"

class dAcOfenceBoko2_c : public dAcObjBase_c {
public:
    dAcOfenceBoko2_c() {}
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual ~dAcOfenceBoko2_c() {}
    virtual int actorExecute() override;

    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mModel;
    dBgW mCollision;

private:
};

#endif
