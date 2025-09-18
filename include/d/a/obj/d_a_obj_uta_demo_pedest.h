#ifndef D_A_OBJ_UTA_DEMO_PEDEST_H
#define D_A_OBJ_UTA_DEMO_PEDEST_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOutaDemoPedest_c : public dAcObjBase_c {
public:
    dAcOutaDemoPedest_c() {}
    virtual ~dAcOutaDemoPedest_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    u8 getModelType() {
        return mParams & 0xF;
    }

    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ u8 mModelType;
};

#endif
