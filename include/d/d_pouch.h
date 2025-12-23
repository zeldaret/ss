#ifndef D_POUCH_H
#define D_POUCH_H

#include "common.h"
#include "d/a/d_a_itembase.h"

s32 summarizePouchContents(s32 *, s32 *, s32 *);

u32 convertFilePouchSlot(s32 fileSlot);
s32 convertLytPouchSlot(s32 fileSlot);
s32 getPouchSlotCount(bool unk);
bool isItemExtraAmmo(s32 item);
bool isItemMedal(s32 item);
bool hasAnyShields();
bool isItemShield(s32 item);

#define POUCH_SLOT_NONE 8

u8 adventurePouchFindItemSlot(ITEM_ID item);
u32 adventurePouchCountItem(ITEM_ID itemId);

bool swapAdventurePouchItems(s32 slot1, s32 slot2);
bool swapStockAndPouchItems(s32 pouchSlot, s32 stockSlot);
bool swapStockItems(s32 slot1, s32 slot2);

#define ITEM_CHECK_SLOT_NONE 60

u16 itemCheckFindItemSlot(ITEM_ID item);

#endif
