#ifndef D_A_OBJ_WATER_JAR_H
#define D_A_OBJ_WATER_JAR_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOWaterJar_c : public dAcObjBase_c {
public:
    dAcOWaterJar_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOWaterJar_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOWaterJar_c);
};

#endif
