#ifndef D_A_OBJ_WATER_SPOUT_H
#define D_A_OBJ_WATER_SPOUT_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmmdl.h"
#include "m/m3d/m_anmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOwaterSpout_c : public dAcObjBase_c {
public:
    dAcOwaterSpout_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOwaterSpout_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOwaterSpout_c, Wait);

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::mdlAnmChr mMdl;
    /* 0x39C */ m3d::anmTexSrt_c mAnmSrt;
    /* 0x3C8 */ m3d::anmMatClr_c mAnmClr;
    /* 0x3F4 */ STATE_MGR_DECLARE(dAcOwaterSpout_c);
};

#endif
