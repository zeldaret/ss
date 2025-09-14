#ifndef D_A_KNIGHT_LEADER_BIRD_H
#define D_A_KNIGHT_LEADER_BIRD_H

#include "d/a/d_a_base.h"
#include "d/a/d_a_bird_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m_mtx.h"
#include "s/s_State.hpp"

class dAcKnightLeaderBird_c : public dAcBirdBase_c {
public:
    dAcKnightLeaderBird_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcKnightLeaderBird_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int preDraw() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcKnightLeaderBird_c, Wait);
    STATE_MGR_DEFINE_UTIL_GETOLDSTATEID(dAcKnightLeaderBird_c);

private:
    void updateSpineMtx();

    /* 0x1030 */ u8 _0x1030[0x1040 - 0x1030];
    /* 0x1040 */ STATE_MGR_DECLARE(dAcKnightLeaderBird_c);
    /* 0x1064 */ dAcRef_c<dAcObjBase_c> mUnkRef;
    /* 0x1070 */ mMtx_c mSpineWorldMtx;
};

#endif
