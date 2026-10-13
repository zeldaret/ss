#ifndef D_A_OBJ_ROULETTE_ISLAND_C_H
#define D_A_OBJ_ROULETTE_ISLAND_C_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOrouletteIslandC_c : public dAcObjBase_c {
public:
    dAcOrouletteIslandC_c();
    virtual ~dAcOrouletteIslandC_c();

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOrouletteIslandC_c, Wait);
    STATE_FUNC_DECLARE(dAcOrouletteIslandC_c, Play);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcOrouletteIslandC_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcOrouletteIslandC_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcOrouletteIslandC_c);

    static dAcOrouletteIslandC_c *getInstance();
    static void rideCallback(dBgW *pBgW, dAcObjBase_c *pThis, dAcObjBase_c *pActor);

    void setStartPlay(bool start = true) {
        mbStartPlay = start;
    }

private:
    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ m3d::smdl_c mMdl_s;
    /* 0x368 */ nw4r::g3d::ResFile mRes;
    /* 0x36C */ dBgW mBgW;
    /* 0x57C */ STATE_MGR_DECLARE(dAcOrouletteIslandC_c);
    /* 0x5B8 */ bool mbPlayerOnIsland;
    /* 0x5B9 */ bool mbStartPlay;
    /* 0x5BA */ bool field_0x5BA;

    static dAcOrouletteIslandC_c *sInstance;
};

#endif
