#ifndef D_A_OBJ_F302_LIGHT_H
#define D_A_OBJ_F302_LIGHT_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/time_area_mgr.h"

class dAcOF302Light_c : public dAcObjBase_c {
public:
    dAcOF302Light_c() : mEvent(*this, nullptr), mStateMgr(*this) {}
    virtual ~dAcOF302Light_c() {}

    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOF302Light_c, Wait);
    STATE_FUNC_DECLARE(dAcOF302Light_c, SwitchOn);

private:
    /* 0x330 */ ActorEventRelated mEvent;
    /* 0x380 */ nw4r::g3d::ResFile mResFile;
    /* 0x384 */ m3d::smdl_c mMdl;
    /* 0x3A0 */ STATE_MGR_DECLARE(dAcOF302Light_c);
    /* 0x3DC */ m3d::anmMatClr_c mAnmMatClr[2];
    /* 0x434 */ TimeAreaStruct mTimeArea;
    /* 0x440 */ u8 field_0x440;

    static const char *sResAnmClrNames[2];
};

#endif
