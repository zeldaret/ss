#ifndef D_LYT_UTIL_ITEMS_H
#define D_LYT_UTIL_ITEMS_H

#include "common.h"
#include "toBeSorted/dowsing_target.h"

bool isPouchBocoburinLocked();

u8 getPouchItemForSlot(s32 slot, bool unk);
bool hasPouchSlot(s32 slot, bool unk);
s32 getPouchItemAmount(s32 slot, bool unk);
u8 getPouchItemNumberColor(s32 slot, bool unk);
f32 getShieldDurability(s32 slot, bool unk);

DowsingTarget::DowsingSlot dowsingSlotForIndex(s32 index);
s32 getLytIndexForDowsingIndex(s32 index);
u16 getDowsingItemIdForIndex(s32 index);

/**
 * FR: -1, 0, 1 singular, else plural
 * Otherwise: -1, 1 singular, else plural
 */
bool shouldUseSingular(s32 amount);

#endif
