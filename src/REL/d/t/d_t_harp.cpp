#include "d/t/d_t_harp.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_source_harp_related.h"
#include "d/snd/d_snd_source_if.h"
#include "s/s_Math.h"

SPECIAL_ACTOR_PROFILE(HARP_TAG, dTgHarp_c, fProfile::HARP_TAG, 0x1A4, 0, 0);

STATE_DEFINE(dTgHarp_c, Wait);

bool dTgHarp_c::createHeap() {
    return true;
}

int dTgHarp_c::create() {
    u32 tmp = (mParams >> 8) & 0xF;
    if (tmp != 0xF) {
        field_0x13a = tmp * 0x1E;
    } else {
        field_0x13a = 0x96;
    }

    if (initAllocatorWork1Heap(-1, const_cast<char *>("dTgHarp_c::m_allocator"), 0x20) == 0) {
        return 2;
    }

    fn_536_6C0();
    mTimer_0x13c = field_0x13a;
    byte_t lowByte = mParams & 0xFF;
    sceneflag_0x13e = lowByte;

    if (lowByte < 0xFF) {
        bool flagSet = SceneflagManager::sInstance->checkFlag(mRoomID, mParams & 0xFF);

        if (flagSet) {
            return 2;
        }
    }
    mStateMgr.changeState(StateID_Wait);
    return 1;
}

int dTgHarp_c::doDelete() {
    return 1;
}

int dTgHarp_c::actorExecute() {
    mStateMgr.executeState();
    return 1;
}

int dTgHarp_c::draw() {
    return 1;
}

void dTgHarp_c::fn_536_6C0() {
    // TODO: soundSource should be of type dSndSourceHarpRelated_c*
    dSoundSourceIf_c *soundSource = getSoundSource();
    if (soundSource != nullptr) {
        soundSource->offFlag(2);
        // fn_536_720(soundSource);
    }
}

void dTgHarp_c::fn_536_720(dSndSourceHarpRelated_c *soundSource) {
    if (field_0x138 == 0 && dAcItem_c::checkFlag(ITEM_GODDESS_HARP)) {
        soundSource->onFlag(2);
        soundSource->d_s_harp_vt_0x1F0(); // Offset to 0x1F0 is off by 4 bytes
        field_0x138 = 1;
    }
}

// Actual implementation and definition is located in d/snd/d_snd_source_harp_related.cpp
bool harpSoundRelated() {
    return true;
}

void dTgHarp_c::initializeState_Wait() {}

void dTgHarp_c::executeState_Wait() {
    // TODO: soundSource should be of type dSndSourceHarpRelated_c*
    dSoundSourceIf_c *soundSource = getSoundSource();
    if (soundSource != nullptr) {
        return;
    }

    // fn_536_720(soundSource);

    if (isWithinPlayerRadius(mScale.x)) {
        // TODO: call harpSoundRelated with from d/snd/d_snd_source_harp_related.cpp
        if (harpSoundRelated() && !sLib::calcTimer(&mTimer_0x13c)) {
            if (sceneflag_0x13e < 0xff) {
                bool flagStatus = SceneflagManager::sInstance->checkFlag(mRoomID, sceneflag_0x13e);

                if (!flagStatus) {
                    SceneflagManager::sInstance->setFlag(mRoomID, sceneflag_0x13e);
                }
            }

            deleteRequest();
        }

    } else {
        field_0x13a = 0x96;
        mTimer_0x13c = 0x96;
    }
}

void dTgHarp_c::finalizeState_Wait() {}
