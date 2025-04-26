#ifndef D_POUCH_H
#define D_POUCH_H

#include "common.h"

u32 convertFilePouchSlot(s32 fileSlot);
s32 convertLytPouchSlot(s32 fileSlot);
s32 getPouchSlotCount(bool unk);
bool isItemExtraAmmo(s32 item);
bool isItemMedal(s32 item);

#endif
