#ifndef D_A_OBJ_DOOR_H
#define D_A_OBJ_DOOR_H

#include "d/a/obj/d_a_obj_door_base.h"

class dAcOdoor_c : public dAcObjDoor_c {
public:
    dAcOdoor_c() {}
    virtual ~dAcOdoor_c() {}

    bool isLocked();

private:
};

#endif
