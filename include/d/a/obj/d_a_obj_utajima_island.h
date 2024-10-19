#ifndef D_A_OBJ_UTAJIMA_ISLAND_H
#define D_A_OBJ_UTAJIMA_ISLAND_H

#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_utajima_main_mecha.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

// Seemingly belongs to mLib / angle related
extern "C" const f32 lbl_8057CDA4;

class dAcOutajimaIsland_c : public dAcObjBase_c {
public:
    dAcOutajimaIsland_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOutajimaIsland_c() {}

    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    virtual int doDelete() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOutajimaIsland_c, Wait);

private:
    static const f32 floats[5];
    static bool sPlatformsSettled;
    void movePlatforms();

    s32 getParm2() {
        return ((params >> 4) & 0xF);
    }

    s32 getRingLayer() {
        return params & 0xF;
    }

    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ nw4r::g3d::ResFile mRes;
    /* 0x350 */ dBgW mBgW;
    /* 0x560 */ STATE_MGR_DECLARE(dAcOutajimaIsland_c);
    /* 0x59C */ dAcRef_c<dAcOutajimaMainMecha_c> mMainMechaRef;
    /* 0x5A8 */ mVec3_c mInitialPos;
    /* 0x5B4 */ mAng3_c mInitialRot;
    /* 0x5BA */ u8 mRingLayer;
    /* 0x5BB */ u8 mParam2;
    /* 0x5BC */ mAng field_0x5BC;
    /* 0x5BE */ mAng3_c field_0x5BE;
    /* 0x5C4 */ bool mIsMoving;
    /* 0x5C5 */ bool mPlaySound;
};

#endif
