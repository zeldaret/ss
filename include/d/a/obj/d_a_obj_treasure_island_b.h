#ifndef D_A_OBJ_TREASURE_ISLAND_B_H
#define D_A_OBJ_TREASURE_ISLAND_B_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOtreasureIslandB_c : public dAcObjBase_c {
public:
    dAcOtreasureIslandB_c() : mStateMgr(*this) {}
    virtual ~dAcOtreasureIslandB_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOtreasureIslandB_c, Wait);
    STATE_FUNC_DECLARE(dAcOtreasureIslandB_c, CoverBreak);

    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcOtreasureIslandB_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcOtreasureIslandB_c);
    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcOtreasureIslandB_c);
    STATE_MGR_DEFINE_UTIL_GETSTATEID(dAcOtreasureIslandB_c);

    enum Type_e {
        ISLAND,
        ROCK
    };

    bool shouldBreak();

private:
    /* 0x330 */ m3d::smdl_c mMdl[2];
    /* 0x368 */ nw4r::g3d::ResFile mRes;
    /* 0x36C */ dBgW mBgW[2];
    /* 0x78C */ STATE_MGR_DECLARE(dAcOtreasureIslandB_c);
    /* 0x7C8 */ dCcD_Sph mCcRock;
    /* 0x918 */ u8 mRockBrokenFlag;
};

#endif
