#ifndef D_A_OBJ_GROUND_COVER_H
#define D_A_OBJ_GROUND_COVER_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOgroundCover_c : public dAcObjBase_c {
public:
    dAcOgroundCover_c();
    virtual ~dAcOgroundCover_c();

    STATE_FUNC_DECLARE(dAcOgroundCover_c, Wait);
    STATE_FUNC_DECLARE(dAcOgroundCover_c, Wait2);
    STATE_FUNC_DECLARE(dAcOgroundCover_c, AppearWait);
    STATE_FUNC_DECLARE(dAcOgroundCover_c, AppearOpen);
    STATE_FUNC_DECLARE(dAcOgroundCover_c, AppearClose);
    STATE_FUNC_DECLARE(dAcOgroundCover_c, Fight);
    STATE_FUNC_DECLARE(dAcOgroundCover_c, Sealed);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOgroundCover_c);
};

#endif
