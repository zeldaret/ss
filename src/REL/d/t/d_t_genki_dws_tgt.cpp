#include <d/tg/d_tg.h>
#include <toBeSorted/dowsing_target.h>
#include <toBeSorted/room_manager.h>
#include <toBeSorted/sceneflag_manager.h>
#include <toBeSorted/item_story_flag_manager.h>
#include <toBeSorted/scgame.h>


// Maybe this is dTg_c and it is empty?
class UnkIntermediateClass : public dAcBase_c {};

class dTgGenkiDwsTgt_c : public UnkIntermediateClass {
public:
    dTgGenkiDwsTgt_c() : mDowsingTarget(this, 8) {}
    virtual ~dTgGenkiDwsTgt_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;

    u16 getSceneFlagFromParams();

private:
    DowsingTarget mDowsingTarget; // 0xFC
};

SPECIAL_ACTOR_PROFILE(TAG_GENKI_DOWSING_TARGET, dTgGenkiDwsTgt_c, fProfile::TAG_GENKI_DOWSING_TARGET, 0x02A5, 0, 0);

int dTgGenkiDwsTgt_c::create() {
    u16 batQuestActivated = StoryFlagManager::sInstance->getCounterOrFlag(304);
    if (batQuestActivated == 0 || !ScGame::sInstance->currentSpawnInfo.isNight()) {
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
    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;

    static mVec3_c offset(0.0f, 25.0f, 0.0f);
    mDowsingTarget.initialize(3, 0, &offset, 1.0f);
    return SUCCEEDED;
}

int dTgGenkiDwsTgt_c::doDelete() {
    changeLoadedEntitiesNoSet();
    return 1;
}

int dTgGenkiDwsTgt_c::actorExecute() {
    bool flag = SceneflagManager::sInstance->checkFlag(roomid, getSceneFlagFromParams());
    if (flag) {
        deleteRequest();
    } else {
        RoomManager *mgr = RoomManager::m_Instance;
        if (mgr != nullptr) {
            dRoom *room = RoomManager::m_Instance->GetRoomByIndex(roomid);
            if (room != nullptr) {
                if (room->flags & 8) {
                    mDowsingTarget.doRegister();
                } else {
                    mDowsingTarget.doUnregister();
                }
            }
        }
    }
    return 1;
}

u16 dTgGenkiDwsTgt_c::getSceneFlagFromParams() {
    return params & 0xFF;
}
