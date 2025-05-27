#ifndef D_A_OBJ_DISTANT_SHIP_H
#define D_A_OBJ_DISTANT_SHIP_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOdistantShip_c : public dAcObjBase_c {
public:
    dAcOdistantShip_c() {}
    virtual ~dAcOdistantShip_c() {}
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;
    char *getModelName();

private:
    /* 0x330 */ nw4r::g3d::ResFile mBrres;
    /* 0x334 */ m3d::smdl_c mModel;
    /* 0x350 */ u8 mSubtype;
    /* 0x351 */ bool mIsVisible;
};

#endif
