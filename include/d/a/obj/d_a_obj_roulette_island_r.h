#ifndef D_A_OBJ_ROULETTE_ISLAND_R_H
#define D_A_OBJ_ROULETTE_ISLAND_R_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmtexsrt.h"
#include "s/s_State.hpp"

class dAcOrouletteIslandR_c : public dAcObjBase_c {
public:
    dAcOrouletteIslandR_c();
    virtual ~dAcOrouletteIslandR_c();

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOrouletteIslandR_c, Wait);
    STATE_FUNC_DECLARE(dAcOrouletteIslandR_c, Move);
    STATE_FUNC_DECLARE(dAcOrouletteIslandR_c, Reduce);
    STATE_FUNC_DECLARE(dAcOrouletteIslandR_c, End);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcOrouletteIslandR_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcOrouletteIslandR_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcOrouletteIslandR_c);

    static dAcOrouletteIslandR_c *getInstance();
    static void rideCallback(dBgW *pBgW, dAcObjBase_c *pThis, dAcObjBase_c *pActor);

    void setReduceSpeed(bool reduce = true) {
        mbReduceSpeed = reduce;
    }
    void setMove(bool move = true) {
        mbMove = move;
    }

private:
    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ m3d::smdl_c mMdl_s;
    /* 0x368 */ nw4r::g3d::ResFile mRes;
    /* 0x36C */ m3d::anmTexSrt_c mAnmTexSrt;
    /* 0x398 */ dBgW mBgW;
    /* 0x5A8 */ STATE_MGR_DECLARE(dAcOrouletteIslandR_c);
    /* 0x5E4 */ bool mbPlayerOnIsland;
    /* 0x5E5 */ bool mbReduceSpeed;
    /* 0x5E6 */ bool mbMove;
    /* 0x5E7 */ bool field_0x5E7;
    /* 0x5E8 */ s16 mRotationSpeed;
    /* 0x5E8 */ s16 mRotationSpeedTarget;

    static dAcOrouletteIslandR_c *sInstance;
};

#endif
