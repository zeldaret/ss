
#include "toBeSorted/salvage_mgr.h"

#include "common.h"
#include "d/d_sc_game.h"
#include "d/flag/storyflag_manager.h"
#include "toBeSorted/event_manager.h"

dSalvageMgr_c *dSalvageMgr_c::sInstance;

void dSalvageMgr_c::create() {
    if (sInstance == nullptr) {
        sInstance = new dSalvageMgr_c();
        if (sInstance != nullptr) {
            sInstance->init();
        }
    }
}

void dSalvageMgr_c::remove() {
    delete sInstance;
    sInstance = nullptr;
}

void dSalvageMgr_c::init() {
    mCurrentPickupIdx = -1;
    mCurrentPickupMaskBit = 0;
    for (int i = 0; i < SALVAGE_OBJ_MAX; i++) {
        s32 it = i;
        if (!checkDeliveredStoryflag(it) && checkPickupStoryflag(it)) {
            mCurrentPickupIdx = it;
            mCurrentPickupMaskBit |= 1 << it;
        }
    }
    field_0x2C = 10;
    setupDeliverToTarget();
}

void dSalvageMgr_c::execute() {
    handlePickupIndex();
    setupDeliverToTarget();
}

void dSalvageMgr_c::handlePickupIndex() {
    if (mCurrentPickupIdx != -1 && checkDeliveredStoryflag(mCurrentPickupIdx)) {
        mCurrentPickupIdx = -1;
    }

    s32 target = -1;
    for (int i = 0; i < SALVAGE_OBJ_MAX; i++) {
        s32 it = i;
        if (!checkDeliveredStoryflag(it) && checkPickupStoryflag(it)) {
            // TODO instruction swap
            if (target == -1 && it != mCurrentPickupIdx && !checkPickupMask(it)) {
                target = it;
            }
            mCurrentPickupMaskBit |= 1 << it;
        } else {
            mCurrentPickupMaskBit &= ~(1 << it);
        }
    }

    if (target != -1) {
        setPickupIndex(target);
    }
}

void dSalvageMgr_c::setupDeliverToTarget() {
    if (checkIsOnDeliveryStage2() && field_0x2C > 0) {
        // TODO wrong registers
        if (!mDeliverToRef.isLinked() || mDeliverToRef.get()->mProfileName != getDeliverToActorId(mCurrentPickupIdx)) {
            dAcObjBase_c *deliverTo = findDeliveryTarget();
            if (deliverTo == nullptr) {
                mDeliverToRef.unlink();
                field_0x2C = field_0x2C - 1 < 0 ? 0 : field_0x2C - 1;
            } else {
                mDeliverToRef.link(deliverTo);
                field_0x2C = 10;
            }
        }
    } else {
        mDeliverToRef.unlink();
    }
}

struct SalvageMgrObjTransform {
    ~SalvageMgrObjTransform() {}

    /* 0x00 */ mVec3_c offset;
    /* 0x0C */ mAng3_c rotation;
};

#define deg_ang(x, y, z) mAng3_c(mAng::fromDeg(x), mAng::fromDeg(y), mAng::fromDeg(z))

static const SalvageMgrObjTransform sTransforms[SALVAGE_OBJ_MAX] = {
    { mVec3_c(29.22f,    4.51f, -46.36f),  deg_ang(81.83f,  56.65f, -12.683f)},
    { mVec3_c(37.05f,  -65.41f, -10.07f),  deg_ang(4.149f, 21.983f, 134.444f)},
    {mVec3_c(-28.28f,  -27.73f,  -5.12f), deg_ang(16.224f,  5.528f, -73.192f)},
    { mVec3_c(-44.5f,  -37.32f,  -7.17f),  deg_ang(4.237f,  15.61f, -90.026f)},
    {mVec3_c(-26.64f,   -8.95f, -95.21f), deg_ang(83.418f, 19.082f, -73.081f)},
    {mVec3_c(-97.98f, -194.72f,  0.119f), deg_ang(-1.184f,   1.26f, -52.992f)},
    {mVec3_c(-97.98f, -194.72f,  0.119f), deg_ang(-1.184f,   1.26f, -52.992f)},
};

#undef deg_ang

struct SalvageMgrObjDef {
    /* 0x00 */ u16 profileId;
    /* 0x02 */ u16 deliverToProfileId;
    /* 0x04 */ const char *fayCallEvnt;
    /* 0x08 */ const char *deliveryEvnt;
    /* 0x0C */ const char *postDeliveryEvnt;
    /* 0x10 */ const char *stage;
    /* 0x14 */ s32 questStartedStoryflag;
    /* 0x18 */ s32 pickedUpStoryFlag;
    /* 0x1C */ s32 additionalPickupUnsetFlag;
    /* 0x20 */ s32 deliveredStoryflag;
};

// TODO - the docs for the plant species flags might be wrong
// clang-format off
static const SalvageMgrObjDef sDefs[SALVAGE_OBJ_MAX] = {
    {        fProfile::OBJ_PROPERA, fProfile::NPC_SORAJIMA_FATHER, "SalbageFayCall",  "DelivaryPropera", "RepearPropera",  "F000",           STORYFLAG_SCRAPPER_UNLOCKED,            STORYFLAG_PROPERA_PICKUP,  -1,       STORYFLAG_WINDMILL_REPAIRED },
    {       fProfile::OBJ_ROULETTE, fProfile::NPC_DIVE_GAME_JUDGE, "SalbageFayCall", "DelivaryRoulette",   "DefaultTalk",  "F020",   STORYFLAG_PARTY_WHEEL_QUEST_STARTED,        STORYFLAG_PARTY_WHEEL_PICKUP,  -1,   STORYFLAG_PARTY_WHEEL_DELIVERED },
    {fProfile::OBJ_DIVINER_CRYSTAL,             fProfile::NPC_SHA, "SalbageFayCall",   "DelivarySuisho",   "DefaultTalk", "F013r",       STORYFLAG_CRYSTAL_QUEST_STARTED,            STORYFLAG_CRYSTAL_PICKUP,  -1,       STORYFLAG_CRYSTAL_DELIVERED },
    {  fProfile::NPC_SALBAGE_MORRY,             fProfile::NPC_SKN, "SalbageFayCall",     "DelivaryKyui", "RepearPropera", "F001r", STORYFLAG_PLANT_SPECIES_QUEST_STARTED,                       STORYFLAG_480,  -1,                     STORYFLAG_481 },
    {     fProfile::NPC_MOLE_ELDER,             fProfile::NPC_PDU, "SalbageFayCall",     "DelivaryMole",   "DefaultTalk",  "F020",    STORYFLAG_MOLE_ELDER_QUEST_STARTED,         STORYFLAG_MOLE_ELDER_PICKUP,  -1,    STORYFLAG_MOLE_ELDER_DELIVERED },
    {      fProfile::OBJ_WATER_JAR, fProfile::NPC_SORAJIMA_FATHER, "SalbageFayCall",      "DelivaryPot",   "DefaultTalk",  "F000",   STORYFLAG_WATER_BASIN_QUEST_STARTED,  STORYFLAG_SCRAPPER_HAS_WATER_BASIN,  -1, STORYFLAG_SUMMIT_BIG_FROG_WATERED },
    {        fProfile::OBJ_POT_SAL, fProfile::NPC_SORAJIMA_FATHER, "SalbageFayCall",  "DelivaryPumpPot",   "DefaultTalk",  "F023",      STORYFLAG_SPIRAL_CHARGE_UNLOCKED,       STORYFLAG_PUMPKIN_SOUP_PICKUP,  -1,    STORYFLAG_LEVIAS_FIGHT_STARTED },
};
// clang-format on

struct SalvageMgrObjEntrypoints {
    /* 0x00 */ u16 talkEntrypoint1;
    /* 0x02 */ u16 talkEntrypoint2;
    /* 0x04 */ u16 talkEntrypoint3;
    /* 0x06 */ u16 talkEntrypoint4; // unused
};

// clang-format off
static const SalvageMgrObjEntrypoints sEntrypoints[SALVAGE_OBJ_MAX] = {
    {
        51001, // 510_01: This is the Windmill Propeller...
        51004, // 510_04: Master, I suggest that we also return...
        51005, // 510_05: Hey! Master Shortpants! You're in my may...
        11818, // 118_18: Here's that windmill propeller, zzzazt!
    },
    {
        45101, // 451_01: I have identified the item that the... highly unusual man...
        45104, // 451_04: Master, I suggest that we also return...
        45105, // 451_05: OK, OK, zrrft... You just sit tight there
        11005, // 110_05: Here's your wheel thing, bzzzrrrt!
    },
    {
        35101, // 351_01: I have located a substance that closely resembles...
        35104, // 351_04: Master, I suggest we also return...
        35105, // 351_05: Hey, Master Shortpants! How come...
        11820, // 118_20: Here's that bright and shiny crystal ball, BZZAT!
    },
    {
        25106, // 251_06: I can verify with only 40% accuracy that this person is a plant...
        25104, // 251_04: Master, I suggest we also return...
        25105, // 251_05: Move it! Move it! Zrrppt! Forcing me...
        11821, // 118_21: I have come bearing the unusual Kikwi...
    },
    {
        51001, // (?) 510_01: This is the Windmill Propeller...
        35111, // 351_11: Master, I suggest that we also return...
        35112, // 351_12: You don't have to tell me twice, zrrbt!
        11820, // (?) 118_20: Here's that bright and shiny crystal ball, BZZAT!
    },
    {
        51001, // (?) 510_01: This is the Windmill Propeller...
        -1,    // <nothing>
        -1,    // <nothing>
        11790, // Does not exist?
    },
    {
        51001, // (?) 510_01: This is the Windmill Propeller...
        -1,    // <nothing>
        -1,    // <nothing>
        12020, // 120_20: I'm unloading this cargo of pumpkin soup...
    },
};
// clang-format off

mMtx_c dSalvageMgr_c::getCarryRotMtx(const s32 &id) const {
    SalvageMgrObjTransform tf = sTransforms[id];
    mMtx_c result;
    result.transS(tf.offset);
    result.ZYXrotM(tf.rotation);
    return result;
}

void dSalvageMgr_c::setPickupIndex(const s32 &id) {
    for (int i = 0; i < SALVAGE_OBJ_MAX; i++) {
        if (i != id) {
            unsetPickupStoryflag(i);
        }
    }

    if (!checkDeliveredStoryflag(id)) {
        mCurrentPickupIdx = id;
    }
}

void dSalvageMgr_c::deleteObjAndSlb() {
    dAcObjBase_c *slb = mSlbRef.get();
    dAcObjBase_c *obj = mObjRef.get();

    if (obj != nullptr) {
        obj->deleteRequest();
    }
    if (slb != nullptr) {
        slb->deleteRequest();
    }
}

bool dSalvageMgr_c::checkIsOnDeliveryStage2() const {
    if (mCurrentPickupIdx == -1) {
        return false;
    }
    return checkIsOnDeliveryStage();
}

bool dSalvageMgr_c::hasStartedQuestForSalvageObj(const dAcSalbageObj_c *obj) const {
    s32 id = obj->getSalvageIf()->getSalvageObjId();
    if (id < 0) {
        return false;
    }
    s32 it = id;
    return hasStartedQuestForId(it);
}

bool dSalvageMgr_c::hasStartedQuestForId(const s32 &id) const {
    if (id == -1) {
        return false;
    } else if (sDefs[id].questStartedStoryflag == -1) {
        return false;
    } else {
        return StoryflagManager::sInstance->getFlag(sDefs[id].questStartedStoryflag) != 0;
    }
}

bool dSalvageMgr_c::hasDowsingForSalvageObj(const dAcSalbageObj_c *obj) const {
    s32 id = obj->getSalvageIf()->getSalvageObjId();
    if (id < 0) {
        return false;
    }
    s32 it = id;
    return hasDowsingForId(it);
}

bool dSalvageMgr_c::hasDowsingForId(const s32 &id) const {
    if (id == -1) {
        return false;
    } else if (checkDeliveredStoryflag(id)) {
        return false;
    } else {
        bool ret = false;
        switch (id) {
            case SALVAGE_OBJ_PROPERA: {
                ret = DowsingTarget::hasPropellerDowsing();
                break;
            }
            case SALVAGE_OBJ_ROULETTE: {
                ret = DowsingTarget::hasPartyWheelDowsing();
                break;
            }
            case SALVAGE_OBJ_DIVINER_CRYSTAL: {
                ret = DowsingTarget::hasCrystalBallDowsing();
                break;
            }
            case SALVAGE_OBJ_SALVAGE_MORRY: {
                ret = DowsingTarget::hasNewPlantSpeciesDowsing();
                break;
            }
            case SALVAGE_OBJ_MOLE_ELDER: {
                ret = DowsingTarget::hasPumpkinDowsing();
                break;
            }
            case SALVAGE_OBJ_WATER_JAR: {
                ret = DowsingTarget::hasWaterBasinDowsing();
                break;
            }
        }
        return ret;
    }
}

u16 dSalvageMgr_c::getDeliverToActorId(s32 id) {
    return sDefs[id].deliverToProfileId;
}

void dSalvageMgr_c::noop() {}

s32 dSalvageMgr_c::getCurrentSalvageObjId() const {
    return mCurrentPickupIdx;
}

bool dSalvageMgr_c::checkDeliveredStoryflag(const s32 &id) const {
    if (id == -1) {
        return false;
    } else if (sDefs[id].deliveredStoryflag == -1) {
        return false;
    } else {
        return StoryflagManager::sInstance->getFlag(sDefs[id].deliveredStoryflag) != 0;
    }
}

void dSalvageMgr_c::unsetPickupStoryflag(const s32 &id) {
    if (id == -1) {
        return;
    } else if (checkDeliveredStoryflag(id)) {
        return;
    } else {
        // TODO - ???
        s32 alsoUnsetFlag = sDefs[id].additionalPickupUnsetFlag;
        if (sDefs[id].pickedUpStoryFlag != -1) {
            StoryflagManager::sInstance->unsetFlag(sDefs[id].pickedUpStoryFlag);
        }
        if (alsoUnsetFlag != -1) {
            StoryflagManager::sInstance->unsetFlag(sDefs[id].additionalPickupUnsetFlag);
        }
    }
}

bool dSalvageMgr_c::checkPickupStoryflag(const s32 &id) const {
    if (id == -1) {
        return false;
    } else if (sDefs[id].pickedUpStoryFlag == -1) {
        return false;
    } else {
        return StoryflagManager::sInstance->getFlag(sDefs[id].pickedUpStoryFlag) != 0;
    }
}

bool dSalvageMgr_c::checkIsOnDeliveryStage() const {
    if (mCurrentPickupIdx >= 0 && dScGame_c::currentSpawnInfo.stageName == sDefs[mCurrentPickupIdx].stage) {
        return true;
    } else {
        return false;
    }
}

dAcObjBase_c *dSalvageMgr_c::findDeliveryTarget() {
    if (mDeliverToRef.isLinked()) {
        return mDeliverToRef.get();
    } else if (mCurrentPickupIdx == -1) {
        return nullptr;
    } else {
        return (dAcObjBase_c *)fManager_c::searchBaseByProfName(
            (ProfileName)sDefs[mCurrentPickupIdx].deliverToProfileId, nullptr
        );
    }
}

bool dSalvageMgr_c::triggerDeliveryEvent() {
    if (mCurrentPickupIdx == -1) {
        return false;
    } else {
        dAcNpcSlb_c *slb = mSlbRef.get();
        dAcSalbageObj_c *obj = mObjRef.get();
        dAcObjBase_c *deliverTo = mDeliverToRef.get();
        if (slb == nullptr || obj == nullptr || deliverTo == nullptr) {
            return false;
        } else {
            Event ev(sDefs[mCurrentPickupIdx].deliveryEvnt, 100, 0x100001, nullptr, nullptr);
            EventManager::alsoSetAsCurrentEvent(slb, &ev, deliverTo);
            return true;
        }
    }
}

u16 dSalvageMgr_c::getTalkEntryPoint1ForPickup(dAcObjBase_c *obj) {
    SalvageObj_e id = SALVAGE_OBJ_NONE;
    if (obj != nullptr) {
        if (obj->mProfileName == fProfile::OBJ_PROPERA) {
            id = SALVAGE_OBJ_PROPERA;
        } else if (obj->mProfileName == fProfile::OBJ_ROULETTE) {
            id = SALVAGE_OBJ_ROULETTE;
        } else if (obj->mProfileName == fProfile::OBJ_DIVINER_CRYSTAL) {
            id = SALVAGE_OBJ_DIVINER_CRYSTAL;
        } else if (obj->mProfileName == fProfile::NPC_SALBAGE_MORRY) {
            id = SALVAGE_OBJ_SALVAGE_MORRY;
        } else if (obj->mProfileName == fProfile::NPC_MOLE_ELDER) {
            id = SALVAGE_OBJ_MOLE_ELDER;
        } else if (obj->mProfileName == fProfile::OBJ_WATER_JAR) {
            id = SALVAGE_OBJ_WATER_JAR;
        } else {
            id = SALVAGE_OBJ_PROPERA;
        }
    }

    if (id == SALVAGE_OBJ_NONE) {
        return -1;
    }
    return sEntrypoints[id].talkEntrypoint1;
}

u16 dSalvageMgr_c::getTalkEntryPoint2ForPickup(dAcObjBase_c *obj) {
    SalvageObj_e id = SALVAGE_OBJ_NONE;
    if (obj != nullptr) {
        if (obj->mProfileName == fProfile::OBJ_PROPERA) {
            id = SALVAGE_OBJ_PROPERA;
        } else if (obj->mProfileName == fProfile::OBJ_ROULETTE) {
            id = SALVAGE_OBJ_ROULETTE;
        } else if (obj->mProfileName == fProfile::OBJ_DIVINER_CRYSTAL) {
            id = SALVAGE_OBJ_DIVINER_CRYSTAL;
        } else if (obj->mProfileName == fProfile::NPC_SALBAGE_MORRY) {
            id = SALVAGE_OBJ_SALVAGE_MORRY;
        } else if (obj->mProfileName == fProfile::NPC_MOLE_ELDER) {
            id = SALVAGE_OBJ_MOLE_ELDER;
        } else if (obj->mProfileName == fProfile::OBJ_WATER_JAR) {
            id = SALVAGE_OBJ_WATER_JAR;
        } else {
            id = SALVAGE_OBJ_PROPERA;
        }
    }

    if (id == SALVAGE_OBJ_NONE) {
        return -1;
    }
    return sEntrypoints[id].talkEntrypoint2;
}

u16 dSalvageMgr_c::getTalkEntryPoint3ForPickup(dAcObjBase_c *obj) {
    SalvageObj_e id = SALVAGE_OBJ_NONE;
    if (obj != nullptr) {
        if (obj->mProfileName == fProfile::OBJ_PROPERA) {
            id = SALVAGE_OBJ_PROPERA;
        } else if (obj->mProfileName == fProfile::OBJ_ROULETTE) {
            id = SALVAGE_OBJ_ROULETTE;
        } else if (obj->mProfileName == fProfile::OBJ_DIVINER_CRYSTAL) {
            id = SALVAGE_OBJ_DIVINER_CRYSTAL;
        } else if (obj->mProfileName == fProfile::NPC_SALBAGE_MORRY) {
            id = SALVAGE_OBJ_SALVAGE_MORRY;
        } else if (obj->mProfileName == fProfile::NPC_MOLE_ELDER) {
            id = SALVAGE_OBJ_MOLE_ELDER;
        } else if (obj->mProfileName == fProfile::OBJ_WATER_JAR) {
            id = SALVAGE_OBJ_WATER_JAR;
        } else {
            id = SALVAGE_OBJ_PROPERA;
        }
    }

    if (id == SALVAGE_OBJ_NONE) {
        return -1;
    }
    return sEntrypoints[id].talkEntrypoint3;
}
