#ifndef D_A_OBJ_MUSASABI_H
#define D_A_OBJ_MUSASABI_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOmusasabi_c : public dAcObjBase_c {
public:
    dAcOmusasabi_c() : mStateMgr(*this) {}
    virtual ~dAcOmusasabi_c() {}

    STATE_FUNC_DECLARE(dAcOmusasabi_c, Wait);
    STATE_FUNC_DECLARE(dAcOmusasabi_c, Approach);
    STATE_FUNC_DECLARE(dAcOmusasabi_c, Near);
    STATE_FUNC_DECLARE(dAcOmusasabi_c, Far);
    STATE_FUNC_DECLARE(dAcOmusasabi_c, Formation);
    STATE_FUNC_DECLARE(dAcOmusasabi_c, Away);
    STATE_FUNC_DECLARE(dAcOmusasabi_c, Disappear);
    STATE_FUNC_DECLARE(dAcOmusasabi_c, FormNotPlayer);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOmusasabi_c);
};

#endif
