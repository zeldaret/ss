#ifndef D_A_OBJ_STAGE_KRAKEN_PARTS_H
#define D_A_OBJ_STAGE_KRAKEN_PARTS_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOstageKrakenParts_c : public dAcObjBase_c {
public:
    dAcOstageKrakenParts_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOstageKrakenParts_c() {}

    STATE_FUNC_DECLARE(dAcOstageKrakenParts_c, Shake);
    STATE_FUNC_DECLARE(dAcOstageKrakenParts_c, Fall);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOstageKrakenParts_c);
};

#endif
