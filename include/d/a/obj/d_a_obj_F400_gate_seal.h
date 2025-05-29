#ifndef D_A_OBJ_F400_GATE_SEAL_H
#define D_A_OBJ_F400_GATE_SEAL_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_door.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_smdl.h"

class dAcOF400GateSeal_c : public dAcObjBase_c {
public:
    dAcOF400GateSeal_c() {}
    virtual ~dAcOF400GateSeal_c() {}

    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ m3d::anmMatClr_c mAnmClr;
    /* 0x378 */ dAcRef_c<dAcOdoor_c> mDoor;
};

#endif
