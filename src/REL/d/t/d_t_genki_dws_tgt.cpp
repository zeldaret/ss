#include "d/t/d_t_genki_dws_tgt.h"

#include "d/d_room.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "toBeSorted/room_manager.h"

SPECIAL_ACTOR_PROFILE(TAG_GENKI_DOWSING_TARGET, dTgGenkiDwsTgt_c, fProfile::TAG_GENKI_DOWSING_TARGET, 0x02A5, 0, 0);

int dTgGenkiDwsTgt_c::create() {
    u16 batQuestActivated = StoryflagManager::sInstance->getCounterOrFlag(304);
    if (batQuestActivated == 0 || !dScGame_c::sInstance->currentSpawnInfo.isNight()) {
        return FAILED;
    }

    u16 flag = getSceneFlagFromParams();
    if (flag < 0xFF) {
        bool flagSet = SceneflagManager::sInstance->checkFlag(roomid, flag);
        if (flagSet) {
            return FAILED;
        }
    } else {
        return FAILED;
    }

    int roomId_tmp = roomid;
    if (addActorToRoom(-1)) {
        roomid = roomId_tmp;
        changeLoadedEntitiesWithSet();
    } else {
        return FAILED;
    }
    mScale.x = 1.0f;
    mScale.y = 1.0f;
    mScale.z = 1.0f;

    static mVec3_c offset(0.0f, 25.0f, 0.0f);
    mDowsingTarget.initialize(DowsingTarget::SLOT_CRYSTAL, 0, &offset, 1.0f);
    return SUCCEEDED;
}

int dTgGenkiDwsTgt_c::doDelete() {
    changeLoadedEntitiesNoSet();
    return SUCCEEDED;
}

int dTgGenkiDwsTgt_c::actorExecute() {
    bool flag = SceneflagManager::sInstance->checkFlag(roomid, getSceneFlagFromParams());
    if (flag) {
        deleteRequest();
    } else {
        RoomManager *mgr = RoomManager::m_Instance;
        if (mgr != nullptr) {
            dRoom_c *room = RoomManager::m_Instance->GetRoomByIndex(roomid);
            if (room != nullptr) {
                if (room->checkFlag(8)) {
                    mDowsingTarget.doRegister();
                } else {
                    mDowsingTarget.doUnregister();
                }
            }
        }
    }
    return SUCCEEDED;
}

u16 dTgGenkiDwsTgt_c::getSceneFlagFromParams() {
    return params & 0xFF;
}
