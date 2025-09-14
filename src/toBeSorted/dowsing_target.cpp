#include "toBeSorted/dowsing_target.h"

#include "c/c_math.h"
#include "d/flag/itemflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_common_icon_item_maps.h"
#include "m/m_mtx.h"
#include "toBeSorted/tlist.h"

typedef TList<DowsingTarget, offsetof(DowsingTarget, mLink)> DowsingList;

DowsingList DOWSING_LISTS[8];

static const int DOWSING_TARGET_STORY_FLAGS[8] = {
    STORYFLAG_ZELDA_DOWSING, STORYFLAG_RUPEE_DOWSING,        STORYFLAG_ZELDA_DOWSING, STORYFLAG_CRYSTAL_DOWSING,
    STORYFLAG_HEART_DOWSING, STORYFLAG_GODDESS_CUBE_DOWSING, STORYFLAG_109,           STORYFLAG_TREASURE_DOWSING,
};

DowsingTarget::~DowsingTarget() {
    doUnregister();
}

void DowsingTarget::initialize(DowsingSlot type, u16 unk, const mVec3_c *offset, f32 unk2) {
    reinitialize(type);
    field_0x0E = unk;
    if (offset != nullptr) {
        mOffset = *offset;
    }
}

static bool insertDowsingTarget(DowsingTarget *target);
static bool removeDowsingTarget(DowsingTarget *target);

bool DowsingTarget::doRegister() {
    return insertDowsingTarget(this);
}

bool DowsingTarget::doUnregister() {
    return removeDowsingTarget(this);
}

void DowsingTarget::reinitialize(DowsingSlot type) {
    bool res = doUnregister();
    mDowsingSlot = type;
    if (res) {
        doRegister();
    }
}

void DowsingTarget::getPosition(mVec3_c &position) {
    mMtx_c mtx;
    mMtx_c mtx2;
    PSMTXTrans(mtx.m, mpActor->position.x, mpActor->position.y, mpActor->position.z);
    mtx.YrotM(mpActor->rotation.y);
    PSMTXScale(mtx2.m, mpActor->mScale.x, mpActor->mScale.y, mpActor->mScale.z);
    PSMTXConcat(mtx.m, mtx2.m, mtx.m);
    PSMTXMultVec(mtx.m, mOffset, position);
}

bool DowsingTarget::hasZeldaDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_ZELDA_DOWSING);
}

bool DowsingTarget::hasKikwiDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_KIKWI_DOWSING) &&
           !StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_KIKWI_FIND_FINISHED);
}

bool DowsingTarget::hasKeyPieceDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_KEYPIECE_DOWSING) &&
           !StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_KEYPIECE_COMPLETE);
}

bool DowsingTarget::hasDesertNodeDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_GENERATOR_DOWSING) &&
           !StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_LMF_RAISED);
}

inline bool checkTrial(u16 itemflag, u16 storyflag) {
    return (ItemflagManager::sInstance->getFlagDirect(itemflag) &&
            !StoryflagManager::sInstance->getCounterOrFlag(storyflag)) ?
               true :
               false;
}

bool DowsingTarget::hasAnyTrialDowsing() {
    return checkTrial(ITEM_FARORES_COURAGE, STORYFLAG_FARON_TRIAL_UNLOCKED) ||
           checkTrial(ITEM_NAYRUS_WISDOM, STORYFLAG_LANAYRU_TRIAL_UNLOCKED) ||
           checkTrial(ITEM_DINS_POWER, STORYFLAG_ELDIN_TRIAL_UNLOCKED) ||
           checkTrial(ITEM_SOTH, STORYFLAG_HYLIA_TRIAL_UNLOCKED);
}

bool DowsingTarget::hasSacredWaterDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_SACRED_WATER_DOWSING) &&
           !StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_WATER_DRAGON_HEALED);
}

bool DowsingTarget::hasSandshipDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_SANDSHIP_DOWSING) &&
           !StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_SANDSHIP_ENTERED);
}

bool DowsingTarget::hasTadtoneDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_TADTONE_DOWSING) &&
           !StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_ALL_TADTONES_GATHERED);
}

bool DowsingTarget::hasPropellerDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_PROPELLER_DOWSING);
}

bool DowsingTarget::hasWaterBasinDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_WATER_BASIN_DOWSING);
}

bool DowsingTarget::hasCrystalBallDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_CRYSTAL_BALL_DOWSING);
}

bool DowsingTarget::hasPumpkinDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_PUMPKIN_DOWSING);
}

bool DowsingTarget::hasNewPlantSpeciesDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_KIKWI_SPECIES_DOWSING);
}

bool DowsingTarget::hasPartyWheelDowsing() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_PARTY_WHEEL_DOWSING);
}

bool DowsingTarget::hasDowsingInSlot(int slot) {
    if (slot >= 8) {
        return false;
    }

    if (slot == 6) {
        return true;
    }

    if (slot == 0) {
        return hasZeldaDowsing() || hasAnyTrialDowsing() || hasSacredWaterDowsing() || hasSandshipDowsing() ||
               hasTadtoneDowsing() || hasPropellerDowsing() || hasWaterBasinDowsing();
    } else if (slot == 2) {
        return hasCrystalBallDowsing() || hasPumpkinDowsing() || hasNewPlantSpeciesDowsing() || hasKikwiDowsing() ||
               hasKeyPieceDowsing() || hasDesertNodeDowsing() || hasPartyWheelDowsing();
    } else {
        u16 flag = DOWSING_TARGET_STORY_FLAGS[slot];
        flag = StoryflagManager::sInstance->getCounterOrFlag(flag);
        if (flag != 0) {
            return true;
        }
    }

    return false;
}

#define MYCLAMP(low, high, x) ((x) < (low) ? (low) : ((x) > (high) ? (high) : (x)))

DowsingTarget *DowsingTarget::getDowsingInfo(
    const mVec3_c &playerPosition, const mVec3_c &dowsingDirection, f32 *p3, f32 *p4, f32 *intensity, int slot
) {
    if (slot >= 8) {
        return nullptr;
    }
    mVec3_c dwsDir = dowsingDirection;
    dwsDir.normalize();
    DowsingList *list = &DOWSING_LISTS[slot];
    DowsingTarget *best = nullptr;

    for (DowsingList::Iterator node = list->GetBeginIter(); node != list->GetEndIter(); ++node) {
        mVec3_c targetPos;
        node->getPosition(targetPos);
        mVec3_c targetDir = mVec3_c(targetPos - playerPosition);
        f32 proximity = nw4r::ut::Max(10000.0f - targetDir.mag(), 0.0f);

        targetDir.normalize();

        f32 dot = dwsDir.dot(targetDir);
        dot = MYCLAMP(-1.0f, 1.0f, dot);
        f32 a = 1.0f - dot * dot <= 0.0f ? 0.0f : nw4r::math::FrSqrt(1.0f - dot * dot) * (1.0f - dot * dot);

        f32 f9 = (5461 - labs(cM::atan2s(a, dot))) * (1.0f / 5461.0f);
        if (!(f9 < 0.0f)) {
            f32 val;
            f32 f10 = MYCLAMP(0.0f, 0.9f, proximity * 0.0001f) * 1.1111112f;
            if (f9 > 0.85f) {
                f9 = 0.85f;
            }

            f9 = f9 * 1.1764705f;
            f9 *= f9;
            if (f9 > 1.0f) {
                f9 = 1.0f;
            }

            val = f9 * f10;
            if (val > *intensity) {
                *intensity = val;
                *p3 = f10;
                *p4 = f9;
                best = &*node;
            }
        }
    }
    return best;
}

void DowsingTarget::init() {}

void DowsingTarget::execute() {}

static bool insertDowsingTarget(DowsingTarget *target) {
    u8 slot = target->getSlot();
    if (slot == DowsingTarget::SLOT_NONE) {
        return false;
    }

    if (DOWSING_LISTS[slot].GetPosition(target) != DOWSING_LISTS[slot].GetEndIter()) {
        return false;
    }
    DOWSING_LISTS[slot].append(target);
    return true;
}

static bool removeDowsingTarget(DowsingTarget *target) {
    u8 slot = target->getSlot();
    if (slot == DowsingTarget::SLOT_NONE) {
        return false;
    }

    if (DOWSING_LISTS[slot].GetPosition(target) != DOWSING_LISTS[slot].GetEndIter()) {
        DOWSING_LISTS[slot].remove(target);
        return true;
    }
    return false;
}

s32 DowsingTarget::getTopDowsingIcon() {
    if (hasZeldaDowsing()) {
        return LYT_CMN_DowsingZelda;
    } else if (hasAnyTrialDowsing()) {
        return LYT_CMN_DowsingTrialGate;
    } else if (hasSacredWaterDowsing()) {
        return LYT_CMN_DowsingHolyWater;
    } else if (hasSandshipDowsing()) {
        return LYT_CMN_DowsingSandship;
    } else if (hasTadtoneDowsing()) {
        return LYT_CMN_DowsingTadtones;
    } else if (hasPropellerDowsing()) {
        return LYT_CMN_DowsingWindmillPropeller;
    } else if (hasWaterBasinDowsing()) {
        return LYT_CMN_DowsingWaterBasin;
    }
    return LYT_CMN_DowsingZelda;
}

s32 DowsingTarget::getLeftDowsingIcon() {
    if (hasCrystalBallDowsing()) {
        return LYT_CMN_DowsingCrystalBall;
    } else if (hasPumpkinDowsing()) {
        return LYT_CMN_DowsingPumpkinPatchPlower;
    } else if (hasNewPlantSpeciesDowsing()) {
        return LYT_CMN_DowsingNewPlantSpecies;
    } else if (hasPartyWheelDowsing()) {
        return LYT_CMN_DowsingPartyWheel;
    } else if (hasKikwiDowsing()) {
        return LYT_CMN_DowsingKikwi;
    } else if (hasKeyPieceDowsing()) {
        return LYT_CMN_DowsingEarthTempleKey;
    } else if (hasDesertNodeDowsing()) {
        return LYT_CMN_DowsingPowerGenerators;
    }
    return LYT_CMN_DowsingCrystalBall;
}
