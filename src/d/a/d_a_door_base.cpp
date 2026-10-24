#include "d/a/obj/d_a_obj_door_base.h"
#include "d/d_stage.h"

fLiMgBa_c dAcObjDoor_c::sDoorList;

dAcObjDoor_c::dAcObjDoor_c() : mDoorNode(this) {
    sDoorList.append(&mDoorNode);
}

dAcObjDoor_c::~dAcObjDoor_c() {
    sDoorList.remove(&mDoorNode);
}

bool dAcObjDoor_c::hasVisitedRoom() {
    return dStage_c::GetInstance()->hasVisitedRoom(mRoomID);
}

void dAcObjDoor_c::setFencePermission(u8 fenceId) {}

void dAcObjDoor_c::clearFencePermission() {}

dAcObjDoor_c *dAcObjDoor_c::getNextDoor(dAcObjDoor_c *door) {
    fLiNdBa_c *node = getNextNode(door);
    if (node == nullptr) {
        return nullptr;
    }
    return static_cast<dAcObjDoor_c *>(node->p_owner);
}

fLiNdBa_c *dAcObjDoor_c::getNextNode(dAcObjDoor_c *door) {
    if (door == nullptr) {
        return sDoorList.getFirst();
    }
    return door->mDoorNode.getNext();
}
