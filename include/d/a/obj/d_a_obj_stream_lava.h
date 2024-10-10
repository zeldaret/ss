#ifndef D_A_OBJ_STREAM_LAVA_H
#define D_A_OBJ_STREAM_LAVA_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOstreamLava_c : public dAcObjBase_c {
public:
    dAcOstreamLava_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOstreamLava_c() {}

    STATE_FUNC_DECLARE(dAcOstreamLava_c, Wait);
    STATE_FUNC_DECLARE(dAcOstreamLava_c, Stream);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOstreamLava_c);
};

#endif
