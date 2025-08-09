#ifndef D_LYT_UTIL_ITEMS_H
#define D_LYT_UTIL_ITEMS_H

#include "common.h"
#include "sized_string.h"
#include "toBeSorted/dowsing_target.h"

enum LytItemNumberColor_e {
    LYT_ITEM_COLOR_GOLD = 0,
    LYT_ITEM_COLOR_RED = 1,
    LYT_ITEM_COLOR_GREEN = 2,
    LYT_ITEM_COLOR_NONE = 3,
};

enum SongLifeTreeSeedStatus_e {
    SONG_LIFETREE_NONE = 0,
    SONG_LIFETREE_HAS_SOTH_PARTS_OR_SEED = 1,
    SONG_LIFETREE_HAS_SOTH = 2,
};

u8 getSongLifeTreeStatus();

/** 0: no sword, 6: TMS */
s32 getCurrentSwordLevel();
u16 getCurrentSwordItemId();
bool isSwordRestrictedBokoBase();

u16 getCurrentGoddessHarpItemId();
bool isHarpRestrictedBokoBase();

u16 getCurrentEarringsItemId();

u16 getTabletItemIdForIndex(s32 index);

u16 getTriforceItemIdForIndex(s32 index);

u16 getSongItemIdForIndex(s32 index);

bool hasGoddessHarp();

bool hasSong(s32 index);

bool hasWaterDragonScale();
u16 getCurrentWaterDragonScaleItemId();
bool isWaterDragonScaleRestricted();

bool hasSailcloth();

s32 getCurrentMittsLevel();
u16 getCurrentMittsItemId();
bool isMittsRestricted();

u16 getCurrentSailclothItemId();

u16 getCurrentWalletItemId();

u16 getCurrentHeartPieceItemId();

void getItemLabel(s32 itemId, SizedString<32> &buf);
void getCaptionItemLabel(s32 itemId, SizedString<32> &buf);
void getDowsingLabel(s32 itemId, SizedString<32> &buf);
void getPauseCategoryLabel(s32 itemId, SizedString<32> &buf);

const wchar_t *getItemText(s32 itemId);
const wchar_t *getCaptionItemText(s32 itemId);
const wchar_t *getDowsingText(s32 itemId);
const wchar_t *getPauseCategoryText(s32 itemId);

bool isPouchBocoburinLocked();

// TODO - these return types may require some work

u8 getPouchItemForSlot(s32 slot, bool unk);
bool hasPouchSlot(s32 slot, bool unk);
s32 getPouchItemAmount(s32 slot, bool unk);
u8 getPouchItemNumberColor(s32 slot, bool unk);
f32 getShieldDurability(s32 slot, bool unk);
u32 getPouchItemIdForIndex(s32 slot, bool unk);

// TODO - rename and make sense of these
DowsingTarget::DowsingSlot dowsingSlotForIndex(s32 index);
u8 getLytIndexForDowsingIndex(s32 index);
u16 getDowsingItemIdForIndex(s32 index);
bool hasDowsingInIndex(s32 index);

// B-Wheel
s32 getBWheelSlotForIndex(s32 index);
bool isBWheelIndexWithNumber(s32 index);
s32 getNumberForBWheelIndex(s32 index);
s32 getItemLevelForBWheelIndex(s32 index);
u8 getLytItemIdForBWheelIndex(s32 index);
bool isBWheelIndexBocoburinLocked(s32 index, bool unk);
u8 getNumberColorForBWheelIndex(s32 index);
u16 getBWheelItemIdForIndex(s32 index);

/**
 * FR: -1, 0, 1 singular, else plural
 * Otherwise: -1, 1 singular, else plural
 */
bool shouldUseSingular(s32 amount);

#endif
