#ifndef D_A_OBJ_FENCE_BOKO_H
#define D_A_OBJ_FENCE_BOKO_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"

class dAcOfenceBoko_c : public dAcObjBase_c {
public:
    dAcOfenceBoko_c() {}
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual ~dAcOfenceBoko_c() {}
    virtual int actorExecute() override;

    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mModel;
    dBgW mCollision;
    bool field_0x560;

private:
};

#endif
