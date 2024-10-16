#include <c/c_math.h>
#include <m/m_mtx.h>
#include <nw4r/ut/ut_LinkList.h>
#include <toBeSorted/dowsing_target.h>
#include <toBeSorted/item_story_flag_manager.h>
#include <toBeSorted/tlist.h>

typedef TList<DowsingTarget, offsetof(DowsingTarget, mLink)> DowsingList;

DowsingList DOWSING_LISTS[8];

static const int DOWSING_TARGET_STORY_FLAGS[8] = {
    100, 105, 100, 104, 108, 110, 109, 102,
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
    return StoryFlagManager::sInstance->getCounterOrFlag(100);
}

bool DowsingTarget::hasKikwiDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(56) && !StoryFlagManager::sInstance->getCounterOrFlag(57);
}

bool DowsingTarget::hasKeyPieceDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(106) && !StoryFlagManager::sInstance->getCounterOrFlag(120);
}

bool DowsingTarget::hasDesertNodeDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(107) && !StoryFlagManager::sInstance->getCounterOrFlag(8);
}

bool DowsingTarget::hasAnyTrialDowsing() {
    // TODO more complicated code
    return ItemFlagManager::sInstance->getFlag(187) && !StoryFlagManager::sInstance->getCounterOrFlag(93) ||
           ItemFlagManager::sInstance->getFlag(188) && !StoryFlagManager::sInstance->getCounterOrFlag(97) ||
           ItemFlagManager::sInstance->getFlag(189) && !StoryFlagManager::sInstance->getCounterOrFlag(98) ||
           ItemFlagManager::sInstance->getFlag(193) && !StoryFlagManager::sInstance->getCounterOrFlag(99);
}

bool DowsingTarget::hasSacredWaterDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(64) && !StoryFlagManager::sInstance->getCounterOrFlag(65);
}

bool DowsingTarget::hasSandshipDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(271) && !StoryFlagManager::sInstance->getCounterOrFlag(14);
}

bool DowsingTarget::hasTadtoneDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(81) && !StoryFlagManager::sInstance->getCounterOrFlag(18);
}

bool DowsingTarget::hasPropellerDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(668);
}

bool DowsingTarget::hasWaterBasinDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(669);
}

bool DowsingTarget::hasCrystalBallDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(670);
}

bool DowsingTarget::hasPumpkinDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(680);
}

bool DowsingTarget::hasNewPlantSpeciesDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(681);
}

bool DowsingTarget::hasPartyWheelDowsing() {
    return StoryFlagManager::sInstance->getCounterOrFlag(682);
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
    } else if (StoryFlagManager::sInstance->getCounterOrFlag(DOWSING_TARGET_STORY_FLAGS[slot])) {
        // TODO small instruction shuffle
        return true;
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

// Not sure what this is
inline static TListNode<DowsingTarget> *getNode(u8 slot, DowsingTarget *t) {
    if (t->mLink.mpNext == nullptr || t->mLink.mpPrev == nullptr) {
        return &DOWSING_LISTS[slot].mStartEnd;
    } else {
        return &t->mLink;
    }
}

static bool insertDowsingTarget(DowsingTarget *target) {
    u8 slot = target->getSlot();
    if (slot == DowsingTarget::SLOT_NONE) {
        return false;
    }

    if (getNode(slot, target) != &DOWSING_LISTS[slot].mStartEnd) {
        return false;
    }
    DOWSING_LISTS[slot].insert(target);
    return true;
}

static bool removeDowsingTarget(DowsingTarget *target) {
    u8 slot = target->getSlot();
    if (slot == DowsingTarget::SLOT_NONE) {
        return false;
    }

    if (getNode(slot, target) != &DOWSING_LISTS[slot].mStartEnd) {
        DOWSING_LISTS[slot].remove(target);
        return true;
    }
    return false;
}

u8 DowsingTarget::getTopDowsingIcon() {
    if (hasZeldaDowsing()) {
        return 19;
    } else if (hasAnyTrialDowsing()) {
        return 20;
    } else if (hasSacredWaterDowsing()) {
        return 5;
    } else if (hasSandshipDowsing()) {
        return 18;
    } else if (hasTadtoneDowsing()) {
        return 17;
    } else if (hasPropellerDowsing()) {
        return 10;
    } else if (hasWaterBasinDowsing()) {
        return 14;
    }
    return 19;
}

u8 DowsingTarget::getLeftDowsingIcon() {
    if (hasCrystalBallDowsing()) {
        return 15;
    } else if (hasPumpkinDowsing()) {
        return 12;
    } else if (hasNewPlantSpeciesDowsing()) {
        return 21;
    } else if (hasPartyWheelDowsing()) {
        return 13;
    } else if (hasKikwiDowsing()) {
        return 8;
    } else if (hasKeyPieceDowsing()) {
        return 11;
    } else if (hasDesertNodeDowsing()) {
        return 7;
    }
    return 15;
}
