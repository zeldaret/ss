#ifndef D_A_ITEM_BASE_H
#define D_A_ITEM_BASE_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m_vec.h"

// Ghidra: AcItem
//   Size: 0xd68
// official name
class dAcItem_c : public dAcObjBase_c {
public:
    static u32 createItemParams(u16 itemId, u32 subType, u32 unk, u8 unk2, u8, u8);

    static void spawnItem(u16 item, u32 room, const mVec3_c &pos, const mAng3_c &rot, u32 params, u32 arg);
    static void spawnDrop(u16 item, u32 room, const mVec3_c &pos, const mAng3_c &rot);

    static u32 checkFlag(u32 flag);
    void setItemPosition(const mVec3_c &);
    void getItemFromBWheelItem();
    bool isStateWait();

    static fLiMgBa_c sItemList;
};

#endif
