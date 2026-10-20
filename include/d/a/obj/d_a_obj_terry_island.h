#ifndef D_A_OBJ_TERRY_ISLAND_H
#define D_A_OBJ_TERRY_ISLAND_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_light_env.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"

class dAcOterryIsland_c : public dAcObjBase_c {
public:
    dAcOterryIsland_c() : mStateMgr(*this), mEmiter(this) {}
    virtual ~dAcOterryIsland_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOterryIsland_c, Wait);
    STATE_FUNC_DECLARE(dAcOterryIsland_c, Vanish);
    STATE_FUNC_DECLARE(dAcOterryIsland_c, Vacuum);
    STATE_FUNC_DECLARE(dAcOterryIsland_c, Smoke);

    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcOterryIsland_c);
    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcOterryIsland_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcOterryIsland_c);
    STATE_MGR_DEFINE_UTIL_GETSTATEID(dAcOterryIsland_c);

    bool isHitBomb();
    bool isHitBellows();
    bool isHitWater();

private:
    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ m3d::anmTexSrt_c mAnmTexSrt;
    /* 0x378 */ nw4r::g3d::ResFile mRes;
    /* 0x37C */ dBgW mBgW;
    /* 0x58C */ dCcD_Sph mCcFire;
    /* 0x6DC */ STATE_MGR_DECLARE(dAcOterryIsland_c);
    /* 0x718 */ dEmitter_c mEmiter;
    /* 0x74C */ LIGHT_INFLUENCE mLight;
    /* 0x768 */ mVec3_c mCampfirePos;
    /* 0x774 */ s16 mStateTimer;
};

#endif
