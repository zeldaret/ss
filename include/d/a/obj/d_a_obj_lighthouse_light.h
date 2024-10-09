#ifndef D_A_OBJ_LIGHTHOUSE_LIGHT_H
#define D_A_OBJ_LIGHTHOUSE_LIGHT_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOlighthouseLight_c : public dAcObjBase_c {
public:
    dAcOlighthouseLight_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOlighthouseLight_c() {}

    STATE_FUNC_DECLARE(dAcOlighthouseLight_c, Wait);
    STATE_FUNC_DECLARE(dAcOlighthouseLight_c, Move);
    STATE_FUNC_DECLARE(dAcOlighthouseLight_c, Appear);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOlighthouseLight_c);
};

#endif
