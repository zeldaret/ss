#ifndef D_A_OBJ_PUMPKIN_BAR_H
#define D_A_OBJ_PUMPKIN_BAR_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"

class dAcOpumpkinBar_c : public dAcObjBase_c {
public:
    static void barCollisionRideCallback(dBgW *collider, dAcObjBase_c *actor, dAcObjBase_c *interactor);
    dAcOpumpkinBar_c() {}
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual ~dAcOpumpkinBar_c() override;
    virtual int actorExecute() override;

private:
    /* 0x330 */ m3d::smdl_c mModel;
    /* 0x34C */ nw4r::g3d::ResFile mResFile;
    /* 0x350 */ dBgW mBarCollision;
    /* 0x560 */ dBgW mBarBirdCollision;
};

#endif
