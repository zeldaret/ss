#ifndef D_A_OBJ_D3_DUMMY_H
#define D_A_OBJ_D3_DUMMY_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m_mtx.h"
#include "s/s_State.hpp"

class dAcOD3Dummy_c : public dAcObjBase_c {
public:
    dAcOD3Dummy_c() : mStateMgr(*this) {}
    virtual ~dAcOD3Dummy_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual bool createHeap() override;
    virtual int draw() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOD3Dummy_c, Wait);

    void setUpdateTilePositionsSignal(u8 val) {
        mUpdateTilePositions = val;
    }

    s32 getCurrentSkyKeepRoomTileId();
    void updateTileMtx();
    void registerBgs();
    // code is in the dol
    s32 getSpotForTile(s32);

private:
    /* 0x0330 */ dBgW mTileBgWs[8];
    /* 0x13B0 */ mMtx_c mTileMtx[8];
    /* 0x1530 */ STATE_MGR_DECLARE(dAcOD3Dummy_c);
    /* 0x156C */ s32 mCurrentRoomTileId;
    /* 0x1570 */ bool mUpdateTilePositions;
    /* 0x1574 */ void *mIconData;
    /* 0x1578 */ bool mHasBgsRegistered;
};

#endif
