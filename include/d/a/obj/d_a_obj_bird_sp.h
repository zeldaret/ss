#ifndef D_A_OBJ_BIRD_SP_H
#define D_A_OBJ_BIRD_SP_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"

class dAcObjBirdSp_c : public dAcObjBase_c {
public:
    dAcObjBirdSp_c() {}
    virtual ~dAcObjBirdSp_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    static dCcD_SrcSph sCcSrc;
    /* 0x330 */ dCcD_Sph mCollider;
};

#endif
