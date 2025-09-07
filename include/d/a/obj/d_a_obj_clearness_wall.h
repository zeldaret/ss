#ifndef D_A_OBJ_CLEARNESS_WALL_H
#define D_A_OBJ_CLEARNESS_WALL_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"

class dAcOclearnessWall_c : public dAcObjBase_c {
public:
    dAcOclearnessWall_c(): field_0x543(true) {}
    virtual ~dAcOclearnessWall_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    const char *getArcName();
    const char *getModelName();

    /* 0x330 */ dBgW mBgW;
    /* 0x540 */ u8 mSceneflag;
    /* 0x541 */ u8 mVariant;
    /* 0x542 */ u8 field_0x542;
    /* 0x543 */ bool field_0x543;
};

#endif
