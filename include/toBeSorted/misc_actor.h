#ifndef MISC_ACTOR_H
#define MISC_ACTOR_H

#include "common.h"
#include "d/flag/sceneflag_manager.h"
#include "d/t/d_t_clef_game.h"

// TODO - this header might be unnecessary; a lot of functions in
// the split corresponding to this header clearly are REL functions
// that were moved to the DOL because they need to be called from the DOL.
// So ideally all of these declarations were moved to REL class definitions...

bool checkIsInSkykeepPuzzle();
void restoreSkykeepPuzzleFromGlobal();

extern const u8 sTadtoneSceneflags[];
extern const u8 sTadtoneGroupSizes[];

inline bool isValidTadtoneGroupIndex(s32 i) {
    return 0 <= i && i <= 17 - 1;
}

inline void unkTadtoneInline(s32 i, u8 *result) {
    if (result != nullptr) {
        if (isValidTadtoneGroupIndex(i)) {
            if (SceneflagManager::sInstance->isNotTempOrZoneFlag(sTadtoneSceneflags[i])) {
                if (SceneflagManager::sInstance->checkTempOrSceneflag(sTadtoneSceneflags[i])) {
                    *result = sTadtoneGroupSizes[i];
                } else if (dTgClefGame_c::GetInstance() == nullptr) {
                    *result = 0;
                } else {
                    *result = dTgClefGame_c::GetInstance()->getField_0x14C(i);
                }
            } else {
                *result = sTadtoneGroupSizes[i];
            }
        }
    }
}

inline void unkTadtoneInline2(s32 i, u8 *result) {
    if (result != nullptr) {
        if (isValidTadtoneGroupIndex(i)) {
            *result = sTadtoneGroupSizes[i];
        }
    }
}

inline void unkTadtoneInline3(s32 i, u8 *result) {
    if (result != nullptr) {
        if (isValidTadtoneGroupIndex(i)) {
            if (dTgClefGame_c::GetInstance() != nullptr) {
                *result = dTgClefGame_c::GetInstance()->getField_0x15D(i);
            }
        }
    }
}

#endif
