#ifndef D_A_OBJ_RAIL_POST_H
#define D_A_OBJ_RAIL_POST_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"

class dAcORailPost : public dAcObjBase_c {
public:
    dAcORailPost();
    virtual ~dAcORailPost();

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x350 */ dBgW mBgW;
};

#endif
