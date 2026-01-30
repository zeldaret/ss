#ifndef D_A_OBJ_RUINED_SAVE_H
#define D_A_OBJ_RUINED_SAVE_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"

class dAcOruinedSave_c : public dAcObjBase_c {
public:
    dAcOruinedSave_c() : mStateMgr(*this), mEvent(*this, nullptr) {}
    virtual ~dAcOruinedSave_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOruinedSave_c, Wait);
    STATE_FUNC_DECLARE(dAcOruinedSave_c, Vanish);

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ STATE_MGR_DECLARE(dAcOruinedSave_c);
    /* 0x38C */ ActorEventRelated mEvent;
    /* 0x3DC */ m3d::anmMatClr_c mAnmMatClr;
    /* 0x408 */ f32 field_0x408;
    /* 0x40C */ u32 field_0x40C;
};

#endif
