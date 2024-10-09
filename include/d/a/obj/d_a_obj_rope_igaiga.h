#ifndef D_A_OBJ_ROPE_IGAIGA_H
#define D_A_OBJ_ROPE_IGAIGA_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOropeIgaiga_c : public dAcObjBase_c {
public:
    dAcOropeIgaiga_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOropeIgaiga_c() {}

    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Init);
    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Rope);
    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Ground);
    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Tree);
    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Stick);
    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Water);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOropeIgaiga_c);
};

#endif
