#ifndef D_A_OBJ_SAVE_H
#define D_A_OBJ_SAVE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOSave_c : public dAcObjBase_c {
public:
    dAcOSave_c();
    virtual ~dAcOSave_c();

    STATE_FUNC_DECLARE(dAcOSave_c, Moss);
    STATE_FUNC_DECLARE(dAcOSave_c, Shine);
    STATE_FUNC_DECLARE(dAcOSave_c, Wait);
    STATE_FUNC_DECLARE(dAcOSave_c, Query);
    STATE_FUNC_DECLARE(dAcOSave_c, LightShaft);
    STATE_FUNC_DECLARE(dAcOSave_c, Escape);

    bool checkStatueFlag() const;
    u32 getStatueType() const;
    u32 getStatueNameId() const;

    enum StatueType {
        TYPE_NORMAL = 0,
        TYPE_OVERWORLD = 1,
        TYPE_DUNGEON = 2,
    };

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSave_c);
};

#endif
