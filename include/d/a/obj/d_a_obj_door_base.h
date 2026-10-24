#ifndef D_A_OBJ_DOOR_BASE_H
#define D_A_OBJ_DOOR_BASE_H

#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "f/f_list_mg.h"

// Ghidra: ActorDoorBase
//   size: 0x33C
// non-official name
class dAcObjDoor_c : public dAcObjBase_c {
public:
    dAcObjDoor_c();
    ~dAcObjDoor_c();

    virtual bool canOpen() = 0;
    virtual bool hasVisitedRoom();
    virtual void setFencePermission(u8 fenceId);
    virtual void clearFencePermission();

    static dAcObjDoor_c *getNextDoor(dAcObjDoor_c *door);
    static fLiNdBa_c *getNextNode(dAcObjDoor_c *door);

protected:
    /* 0x330 */ fLiNdBa_c mDoorNode;

private:
    static fLiMgBa_c sDoorList;
};

#endif
