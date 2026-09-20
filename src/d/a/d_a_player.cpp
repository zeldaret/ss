#include "d/a/d_a_player.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bomb.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "toBeSorted/minigame_mgr.h"

#include <stddef.h>

dAcPy_c *dAcPy_c::LINK;

// TODO: Disable this pragma when the ctors are matching
#pragma dont_inline on
dAcPy_c::dAcPy_c() {}
#pragma dont_inline reset

dAcPy_c::~dAcPy_c() {}

namespace {
bool isProfile(dAcBase_c *pActor, ProfileName profile) {
    return pActor && pActor->mProfileName == profile;
}
} // namespace

bool dAcPy_c::ObjectArray::insert(dAcObjBase_c *pObj) {
    for (size_t i = 0; i < sNumObjects; ++i) {
        if (mObjs[i].get() != pObj) {
            continue;
        }

        if (isProfile(pObj, fProfile::BOMB)) {
            mCounter[i] = MAX(0, static_cast<dAcBomb_c *>(pObj)->get0xA44()) + 3;
        } else {
            mCounter[i] = 90;
        }
        return true;
    }

    for (size_t i = 0; i < sNumObjects; ++i) {
        if (mObjs[i].get() != nullptr) {
            continue;
        }

        mObjs[i].link(pObj);

        if (isProfile(pObj, fProfile::BOMB)) {
            mCounter[i] = MAX(0, static_cast<dAcBomb_c *>(pObj)->get0xA44()) + 3;
        } else {
            mCounter[i] = 90;
        }
        return true;
    }

    return false;
}

void dAcPy_c::ObjectArray::execute() {
    // Unlink Object as needed
    for (int i = 0; i < sNumObjects; ++i) {
        dAcObjBase_c *pObj = mObjs[i].get();
        if (isProfile(pObj, fProfile::BOMB)) {
            if (static_cast<dAcBomb_c *>(pObj)->isState(dAcBomb_c::StateID_Explode)) {
                mObjs[i].unlink();
                mCounter[i] = 0;
            }
        } else {
            if (--mCounter[i] == 0) {
                mObjs[i].unlink();
            }
        }
    }

    // Try to bubble objects left
    // This seems buggy if multiple objects arent link consecutively
    int lastOccupiedObject = sNumObjects;
    for (int i = sNumObjects - 1; i >= 0; --i) {
        // Skip Occupied objects
        if (mObjs[i].isLinked()) {
            lastOccupiedObject = i;
            continue;
        }

        if (lastOccupiedObject != sNumObjects) {
            mObjs[i].link(mObjs[lastOccupiedObject].get());
            mCounter[i] = mCounter[lastOccupiedObject];
            mObjs[lastOccupiedObject].unlink();

            lastOccupiedObject = sNumObjects;
        }
    }
}

dAcObjBase_c *dAcPy_c::ObjectArray::get(s32 index) {
    if (index >= sNumObjects) {
        return nullptr;
    }

    return mObjs[index].get();
}

void dAcPy_c::ObjectArray::removeBombs() {
    for (int i = 0; i < sNumObjects; ++i) {
        dAcObjBase_c *pObj = get(i);
        if (pObj != nullptr) {
            if (pObj->mProfileName == fProfile::BOMB) {
                dAcBomb_c *pBomb = static_cast<dAcBomb_c *>(pObj);
                pBomb->deleteRequest();
                pBomb->mEffect1.remove(true);
                pBomb->mEffect2.remove(true);
            }
        }
    }
}

bool dAcPy_c::isInMinigameInsectCapture() {
    return MinigameManager::GetInstance()->checkInInsectCapture();
}
bool dAcPy_c::isInMinigamePumpkinCarry() {
    return MinigameManager::GetInstance()->checkInPumpkinCarry();
}
bool dAcPy_c::isInMinigameBambooCutting() {
    return MinigameManager::GetInstance()->checkInBambooCutting();
}
bool dAcPy_c::isInMinigameThrillDigger() {
    return MinigameManager::GetInstance()->checkInThrillDigger();
}
bool dAcPy_c::isInMinigameFunFunIsland() {
    return MinigameManager::GetInstance()->checkInFunFunIsland();
}
bool dAcPy_c::isInMinigameHouseCleaning() {
    return MinigameManager::GetInstance()->checkInHouseCleaning();
}
bool dAcPy_c::isInMinigameTrialTimeAttack() {
    return MinigameManager::GetInstance()->checkInTrialTimeAttack();
}
bool dAcPy_c::isInMinigameBossRush() {
    return MinigameManager::GetInstance()->checkInBossRush();
}
bool dAcPy_c::isInMinigamePumpkinArchery() {
    return MinigameManager::GetInstance()->checkInPumpkinArchery();
}
bool dAcPy_c::isInMinigameHarpPlaying() {
    return MinigameManager::GetInstance()->checkInHarpPlaying();
}
bool dAcPy_c::isInMinigameRollerCoaster() {
    return MinigameManager::GetInstance()->checkInRollerCoaster();
}
bool dAcPy_c::isInMinigameSprialChargeTutorial() {
    return MinigameManager::GetInstance()->checkInSprialChargeTutorial();
}
