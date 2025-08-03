#ifndef D_LYT_UTIL_ITEMS_H
#define D_LYT_UTIL_ITEMS_H

#include "common.h"
#include "sized_string.h"
#include "toBeSorted/dowsing_target.h"

void getItemLabel(s32 itemId, SizedString<32> &buf);
void getCaptionItemLabel(s32 itemId, SizedString<32> &buf);
void getDowsingLabel(s32 itemId, SizedString<32> &buf);
void getPauseCategoryLabel(s32 itemId, SizedString<32> &buf);

const wchar_t *getItemText(s32 itemId);
const wchar_t *getCaptionItemText(s32 itemId);
const wchar_t *getDowsingText(s32 itemId);
const wchar_t *getPauseCategoryText(s32 itemId);

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
