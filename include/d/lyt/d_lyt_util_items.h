#ifndef D_LYT_UTIL_ITEMS_H
#define D_LYT_UTIL_ITEMS_H

#include "common.h"

bool isPouchBocoburinLocked();

u8 getPouchItemForSlot(s32 slot, bool unk);
bool hasPouchSlot(s32 slot, bool unk);
s32 getPouchItemAmount(s32 slot, bool unk);
u8 getPouchItemNumberColor(s32 slot, bool unk);
f32 getShieldDurability(s32 slot, bool unk);

#endif
