#ifndef D_A_INSECT_BUTTERFLY_H
#define D_A_INSECT_BUTTERFLY_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"
#include "d/a/d_a_insect.h"

class dAcInsectButterfly_c : public dAcOInsect_c {
public:
    dAcInsectButterfly_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcInsectButterfly_c() {}

    STATE_FUNC_DECLARE(dAcInsectButterfly_c, Move);
    STATE_FUNC_DECLARE(dAcInsectButterfly_c, Escape);
    STATE_FUNC_DECLARE(dAcInsectButterfly_c, Fly);
    STATE_FUNC_DECLARE(dAcInsectButterfly_c, Finalize);
    STATE_FUNC_DECLARE(dAcInsectButterfly_c, Dead);

    void setKillSignal() {
        killSignal = 1;
    }
private:
    /* 0x468 */ u8 field_0x468[0xA34-0x468];
    /* 0xA34 */ STATE_MGR_DECLARE(dAcInsectButterfly_c);
    /* 0xA70 */ u8 field_0xA70[0xAC6-0xA70];
    /* 0xAC6 */ u8 killSignal;
};

#endif
