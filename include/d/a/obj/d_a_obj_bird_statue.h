#ifndef D_A_OBJ_BIRD_STATUE_H
#define D_A_OBJ_BIRD_STATUE_H

#include "d/a/obj/d_a_obj_base.h"

class dAcOBirdStatue_c : public dAcObjBase_c {
public:
    dAcOBirdStatue_c() {}
    virtual ~dAcOBirdStatue_c()   {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int draw() override;

private:
    /* 0x330 */ m3d::smdl_c mMdl;
};

#endif
