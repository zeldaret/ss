#ifndef D_A_BOMBF_H
#define D_A_BOMBF_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bomb.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "s/s_State.hpp"
#include "toBeSorted/time_area_mgr.h"

class dAcBombf_c : public dAcObjBase_c {
public:
    dAcBombf_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcBombf_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    virtual int actorCreate() override;
    virtual int actorPostCreate() override;

    STATE_FUNC_DECLARE(dAcBombf_c, Wait);

private:
    void regrowBomb();

    /* 0x330 */ m3d::smdl_c mModel;
    /* 0x34C */ STATE_MGR_DECLARE(dAcBombf_c);
    /* 0x388 */ dAcRef_c<dAcBomb_c> mBombRef;
    /* 0x394 */ UNKWORD field_0x394;
    /* 0x398 */ cBgS_PolyInfo field_0x398;
    /* 0x3C4 */ TimeAreaStruct mTimeAreaStruct;
    /* 0x3D0 */ bool field_0x3D0;
    /* 0x3D1 */ u8 mDespawnSceneFlag;
    /* 0x3D2 */ u8 field_0x3D2;
    /* 0x3D3 */ u8 field_0x3D3;
    /* 0x3D4 */ u8 field_0x3D4;
};

#endif
