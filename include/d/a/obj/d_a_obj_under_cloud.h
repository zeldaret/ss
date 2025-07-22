#ifndef D_A_OBJ_UNDER_CLOUD_H
#define D_A_OBJ_UNDER_CLOUD_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/actor_event.h"

class dAcOunderCloud_c : public dAcObjBase_c {
public:
    dAcOunderCloud_c() : mEvent(*this, nullptr) {}
    virtual ~dAcOunderCloud_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ m3d::anmTexSrt_c mAnm;
    /* 0x37C */ ActorEventRelated mEvent;
};

#endif
